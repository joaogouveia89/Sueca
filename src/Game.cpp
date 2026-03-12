#include "Game.hpp"
#include <iostream>
#include <algorithm>

#ifdef __APPLE__
    #include <mach-o/dyld.h>
    #include <libgen.h>
    #include <unistd.h>
#endif

// ----------------------------------------------------------------------------
// Constructor & Destructor
// ----------------------------------------------------------------------------
Game::Game() 
    : window(sf::VideoMode(WINDOW_SIZE), GAME_TITLE),
      backgroundSprite(backgroundTexture)
{
    setupMacOSPath();
    loadBackground();
    initializePlayers();
    
    deck = std::make_unique<CardDeck>();
    deck->shuffle();
    dealCards();
}

Game::~Game() {}

// ----------------------------------------------------------------------------
// Initialization Helpers
// ----------------------------------------------------------------------------

// Fixes the working directory issue when running from a macOS .app bundle
void Game::setupMacOSPath() {
#ifdef __APPLE__
    char path[1024];
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) == 0) {
        char realPath[1024];
        if (realpath(path, realPath) != NULL) {
            char* dir = dirname(realPath); 
            chdir(dir);
            chdir("../Resources"); 
        }
    }
#endif
}

// Loads and scales the background texture to fit the window perfectly
void Game::loadBackground() {
    if (backgroundTexture.loadFromFile(BG_PATH)) {
        backgroundSprite.setTexture(backgroundTexture, true); 
        
        sf::Vector2u textureSize = backgroundTexture.getSize();
        sf::Vector2u winSize = window.getSize();

        float scaleX = static_cast<float>(winSize.x) / textureSize.x;
        float scaleY = static_cast<float>(winSize.y) / textureSize.y;
        
        backgroundSprite.setScale({scaleX, scaleY});
        backgroundSprite.setPosition({0, 0}); 
        
        std::cout << "Background loaded successfully. Scale: " << scaleX << "x" << scaleY << std::endl;
    } else {
        backgroundSprite.setTexture(backgroundTexture);
        backgroundSprite.setScale({1.0f, 1.0f}); 
        std::cerr << "Failed to load background image: " << BG_PATH << std::endl;
    }
}

// Sets up the 4 players, placing the Human at index 0 and CPU memory capacities
void Game::initializePlayers() {
    players.push_back(std::make_unique<Player>(0, "You", false, 40));   // Human
    players.push_back(std::make_unique<Player>(1, "CPU 1", true, 4));   // Low memory
    players.push_back(std::make_unique<Player>(2, "CPU 2", true, 20));  // Medium memory
    players.push_back(std::make_unique<Player>(3, "CPU 3", true, 40));  // Perfect memory
}

// Distributes 10 cards to each player and establishes the initial state (Trump showing)
void Game::dealCards() {
    const int CARDS_PER_PLAYER = 10;

    for (size_t i = 0; i < players.size(); i++) {
        auto dealtCards = deck->drawCards(CARDS_PER_PLAYER);
        
        for (auto& card : dealtCards) {
            card->setPosition(DECK_SPAWN_POS, true); 
            card->setRotation(0.0f, true);
            card->setFaceUp(false); 
        }
        players[i]->setCards(dealtCards);
    }

    players[0]->sortHand();

    // Define the trump card as the last card dealt to CPU 3 (Left player).
    // Because CPU 3 is rendered last, this card will appear visually on top of the deck!
    trumpCardRef = players[3]->getHand().back();
    trumpCardRef->setFaceUp(true); 
    trumpSuit = trumpCardRef->getSuit();

    currentState = GameState::SHOWING_TRUMP;
    stateTimer = 0.0f;
}

// ----------------------------------------------------------------------------
// Main Game Loop Methods
// ----------------------------------------------------------------------------

// Core game loop: keeps the window open, tracks time, and ticks the engine
void Game::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        sf::Time dt = clock.restart();
        float deltaTime = dt.asSeconds();

        processEvents();
        update(deltaTime); 
        render();
    }
}

// Intercepts OS events (closing window, keyboard, mouse)
void Game::processEvents() {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        
        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->code == sf::Keyboard::Key::Escape) {
                window.close();
            }
        }

        if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mousePressed->button == sf::Mouse::Button::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords(mousePressed->position);
                handleMouseClick(mousePos);
            }
        }
    }
}

// Routes game logic based on the current active state
void Game::update(float deltaTime) {
    switch (currentState) {
        case GameState::SHOWING_TRUMP:
            updateShowingTrumpState(deltaTime);
            break;
        case GameState::PLAYING:
            updatePlayingState(deltaTime);
            break;
        case GameState::RESOLVING_TRICK:
            updateResolvingTrickState(deltaTime);
            break;
    }
}

// Clears the screen and draws all entities (background, hands, table)
void Game::render() {
    window.clear(sf::Color::Black);
    window.draw(backgroundSprite);

    for (const auto& player : players) {
        for (const auto& card : player->getHand()) {
            card->render(window);
        }
    }

    for (const auto& card : tableCards) {
        card->render(window);
    }

    window.display();
}

// ----------------------------------------------------------------------------
// State Machine Handlers
// ----------------------------------------------------------------------------

// Handles the initial 2-second delay where the trump card is presented
void Game::updateShowingTrumpState(float deltaTime) {
    stateTimer += deltaTime;
    
    if (stateTimer >= 2.0f) {
        transitionToPlayingState();
    }
    
    for (const auto& player : players) {
        for (const auto& card : player->getHand()) {
            card->update(deltaTime); 
        }
    }
    updateTableCardPositions(deltaTime);
}

// Prepares the game data to start receiving player inputs
void Game::transitionToPlayingState() {
    currentState = GameState::PLAYING;
    trumpCardRef->setFaceUp(false); 
    
    for (auto& card : players[0]->getHand()) {
        card->setFaceUp(true);
    }

    firstPlayer = 0; 
    currentPlayer = 0; 
    cardsPlayedInTrick = 0;
    cpuTimer = 0.0f;
}

// Updates animations and triggers the CPU turn if applicable
void Game::updatePlayingState(float deltaTime) {
    updatePlayerCardPositions(deltaTime);
    updateTableCardPositions(deltaTime);

    if (currentPlayer != 0 && cardsPlayedInTrick < 4) {
        cpuTimer += deltaTime;
        if (cpuTimer >= 1.0f) { 
            cpuTimer = 0.0f;
            playCpuTurn();
        }
    }
}

// Freezes the game for 2 seconds to allow players to see the completed trick
void Game::updateResolvingTrickState(float deltaTime) {
    stateTimer += deltaTime;
    
    updatePlayerCardPositions(deltaTime);
    updateTableCardPositions(deltaTime);

    if (stateTimer >= 2.0f) {
        resolveTrick();
    }
}

// ----------------------------------------------------------------------------
// Input & Turn Logic
// ----------------------------------------------------------------------------

// Processes user click, validates Z-index overdraw, and checks Sueca rules
void Game::handleMouseClick(sf::Vector2f mousePos) {
    if (currentState != GameState::PLAYING || currentPlayer != 0) return;

    const auto& hand = players[0]->getHand();

    // Reverse Z-index sweep ensures we only click the visually "top" card
    for (int j = static_cast<int>(hand.size()) - 1; j >= 0; --j) {
        if (hand[j]->getBounds().contains(mousePos)) {
            
            if (!isValidHumanPlay(hand[j])) {
                std::cout << "Invalid move! You must follow the lead suit." << std::endl;
                break; 
            }

            playHumanCard(j);
            break; 
        }
    }
}

// Executes the human player's choice and moves the card to the table
void Game::playHumanCard(int cardIndex) {
    std::shared_ptr<Card> playedCard = players[0]->playCard(cardIndex);
    
    if (playedCard) {
        if (cardsPlayedInTrick == 0) {
            leadSuit = playedCard->getSuit();
        }

        tableCards.push_back(playedCard);
        playedCard->setPosition(getTableTargetPosition(0));
        playedCard->setRotation(0.0f);
        
        notifyPlayersCardPlayed(playedCard);
        cardsPlayedInTrick++;
        advanceTurn();
    }
}

// Asks the AI for a decision and moves the chosen CPU card to the table
void Game::playCpuTurn() {
    bool isFirst = (cardsPlayedInTrick == 0);
    
    // TEMPORARY: using basic logic until SuecaAI is implemented
    auto playedCard = players[currentPlayer]->thinkAndPlay(leadSuit, isFirst);

    if (playedCard) {
        playedCard->setFaceUp(true); 
        
        if (isFirst) {
            leadSuit = playedCard->getSuit();
        }

        tableCards.push_back(playedCard);
        playedCard->setPosition(getTableTargetPosition(currentPlayer));
        playedCard->setRotation(0.0f);
        
        notifyPlayersCardPlayed(playedCard);
        cardsPlayedInTrick++;
        advanceTurn();
    }
}

// Passes the turn to the next player, or triggers resolution if table is full
void Game::advanceTurn() {
    if (cardsPlayedInTrick == 4) {
        currentState = GameState::RESOLVING_TRICK;
        stateTimer = 0.0f; 
    } else {
        currentPlayer = (currentPlayer + 1) % 4;
    }
}

// Evaluates the trick, declares the winner, clears the table, and resets for next trick
void Game::resolveTrick() {
    int trickWinner = determineTrickWinner();

    std::cout << "Player " << players[trickWinner]->getName() << " won the trick!" << std::endl;

    tableCards.clear();
    cardsPlayedInTrick = 0;
    
    firstPlayer = trickWinner; 
    currentPlayer = trickWinner; 
    currentState = GameState::PLAYING;
}

// ----------------------------------------------------------------------------
// Game Rules & Validations
// ----------------------------------------------------------------------------

// Checks if the user's card selection respects the "Follow Suit" rule
bool Game::isValidHumanPlay(const std::shared_ptr<Card>& playedCard) const {
    if (cardsPlayedInTrick == 0) return true;

    Suit clickedSuit = playedCard->getSuit();
    if (clickedSuit != leadSuit) {
        const auto& hand = players[0]->getHand();
        bool hasLeadSuit = std::any_of(hand.begin(), hand.end(), 
            [this](const std::shared_ptr<Card>& c) { return c->getSuit() == leadSuit; });
        
        if (hasLeadSuit) return false;
    }
    
    return true;
}

// Calculates the winner of the 4 cards currently on the table
int Game::determineTrickWinner() const {
    int winningCardIndex = 0;

    for (int i = 1; i < 4; ++i) {
        auto currentWinningCard = tableCards[winningCardIndex];
        auto challengingCard = tableCards[i];

        bool currentIsTrump = (currentWinningCard->getSuit() == trumpSuit);
        bool challengerIsTrump = (challengingCard->getSuit() == trumpSuit);

        if (challengerIsTrump && !currentIsTrump) {
            winningCardIndex = i;
        } 
        else if (challengerIsTrump && currentIsTrump) {
            if (Card::getSuecaPower(challengingCard->getSymbol()) > Card::getSuecaPower(currentWinningCard->getSymbol())) {
                winningCardIndex = i;
            }
        } 
        else if (!challengerIsTrump && !currentIsTrump) {
            if (challengingCard->getSuit() == leadSuit) {
                if (Card::getSuecaPower(challengingCard->getSymbol()) > Card::getSuecaPower(currentWinningCard->getSymbol())) {
                    winningCardIndex = i;
                }
            }
        }
    }

    return (firstPlayer + winningCardIndex) % 4;
}

// Broadcasts the played card to all players so they can update their memory
void Game::notifyPlayersCardPlayed(std::shared_ptr<Card> card) {
    for (auto& player : players) {
        player->memorizeCard(card);
    }
}

// ----------------------------------------------------------------------------
// Animation & View Mapping
// ----------------------------------------------------------------------------

// Calculates the smooth LERP targets for cards currently in players' hands
void Game::updatePlayerCardPositions(float deltaTime) {
    const float SPACING = 40.0f; 

    for (size_t i = 0; i < players.size(); ++i) {
        const auto& hand = players[i]->getHand();

        for (size_t j = 0; j < hand.size(); ++j) {
            sf::Vector2f target;
            float rot = 0.0f;

            switch (i) {
                case 0: // Bottom (Human)
                    target.x = 400.0f + (j * 50.0f);
                    target.y = 600.0f;
                    break;
                case 1: // Right (CPU 1)
                    target.x = 1100.0f;
                    target.y = 200.0f + (j * SPACING);
                    rot = 270.0f;
                    break;
                case 2: // Top (CPU 2)
                    target.x = 800.0f - (j * 50.0f); 
                    target.y = 120.0f;
                    rot = 180.0f;
                    break;
                case 3: // Left (CPU 3)
                    target.x = 180.0f;
                    target.y = 500.0f - (j * SPACING);
                    rot = 90.0f;
                    break;
            }

            hand[j]->setPosition(target);
            hand[j]->setRotation(rot);
            hand[j]->update(deltaTime);
        }
    }
}

// Triggers the animation cycle for cards currently placed on the table
void Game::updateTableCardPositions(float deltaTime) {
    for (auto& card : tableCards) {
        card->update(deltaTime);
    }
}

// Maps player IDs to screen coordinates forming a cross pattern in the center
sf::Vector2f Game::getTableTargetPosition(int playerId) const {
    switch (playerId) {
        case 0: return {640.0f, 400.0f}; // Human (Bottom)
        case 1: return {690.0f, 360.0f}; // Right CPU
        case 2: return {640.0f, 320.0f}; // Top CPU
        case 3: return {590.0f, 360.0f}; // Left CPU
        default: return DECK_SPAWN_POS;
    }
}