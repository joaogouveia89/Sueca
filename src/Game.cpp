#include "Game.hpp"
#include <iostream>

#ifdef __APPLE__
    #include <mach-o/dyld.h>
    #include <libgen.h>
    #include <unistd.h>
#endif

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

void Game::initializePlayers() {
    players.push_back(std::make_unique<Player>(0, "You", false, 40)); // Human
    players.push_back(std::make_unique<Player>(1, "CPU 1", true, 4));
    players.push_back(std::make_unique<Player>(2, "CPU 2", true, 20));
    players.push_back(std::make_unique<Player>(3, "CPU 3", true, 40));
}

void Game::dealCards() {
    const int CARDS_PER_PLAYER = 10;

    for (size_t i = 0; i < players.size(); i++) {
        auto dealtCards = deck->drawCards(CARDS_PER_PLAYER);
        
        for (auto& card : dealtCards) {
            // Keep cards in the center initially
            card->setPosition(DECK_SPAWN_POS, true); 
            card->setRotation(0.0f, true);
            
            // All cards start face down
            card->setFaceUp(false); 
        }
        players[i]->setCards(dealtCards);
    }

    players[0]->sortHand();

    // Define the trump card as the last card dealt to CPU 3 (Left player).
    // Because CPU 3 is rendered last, this card will appear visually on top of the deck!
    trumpCardRef = players[3]->getHand().back();
    trumpCardRef->setFaceUp(true); // Reveal the trump
    trumpSuit = trumpCardRef->getSuit();

    // Initialize state machine
    currentState = GameState::SHOWING_TRUMP;
    stateTimer = 0.0f;
}

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
                // Convert screen click pixel to 2D world coordinates
                sf::Vector2f mousePos = window.mapPixelToCoords(mousePressed->position);
                handleMouseClick(mousePos);
            }
        }
    }
}

void Game::handleMouseClick(sf::Vector2f mousePos) {
    // Clicking is only allowed if we are playing AND it is the Human's turn
    if (currentState != GameState::PLAYING || currentPlayer != 0) return;

    const auto& hand = players[0]->getHand();
    
    for (int j = static_cast<int>(hand.size()) - 1; j >= 0; --j) {
        if (hand[j]->getBounds().contains(mousePos)) {
            playHumanCard(j);
            break; 
        }
    }
}

void Game::playHumanCard(int cardIndex) {
    std::shared_ptr<Card> playedCard = players[0]->playCard(cardIndex);
    
    if (playedCard) {
        // If it is the first card of the round, it determines the suit that should be followed (lead suit).
        if (cardsPlayedInTrick == 0) {
            leadSuit = playedCard->getSuit();
        }

        tableCards.push_back(playedCard);
        playedCard->setPosition(getTableTargetPosition(0));
        playedCard->setRotation(0.0f);
        
        cardsPlayedInTrick++;
        advanceTurn(); // Pass the turn!§
    }
}

sf::Vector2f Game::getTableTargetPosition(int playerId) const {
    switch (playerId) {
        case 0: return {640.0f, 400.0f}; // Human (Further below)
        case 1: return {690.0f, 360.0f}; // Right CPU
        case 2: return {640.0f, 320.0f}; // CPU Top
        case 3: return {590.0f, 360.0f}; // Left CPU
        default: return DECK_SPAWN_POS;
    }
}


void Game::playCpuTurn() {
    bool isFirst = (cardsPlayedInTrick == 0);
    
    // The CPU intelligence you already had in Player.cpp
    auto playedCard = players[currentPlayer]->thinkAndPlay(leadSuit, isFirst);

    if (playedCard) {
        playedCard->setFaceUp(true); // Turn the CPU card face up!
        
        if (isFirst) {
            leadSuit = playedCard->getSuit();
        }

        tableCards.push_back(playedCard);
        playedCard->setPosition(getTableTargetPosition(currentPlayer));
        playedCard->setRotation(0.0f);
        
        cardsPlayedInTrick++;
        advanceTurn();
    }
}

void Game::advanceTurn() {
    if (cardsPlayedInTrick == 4) {
        // All 4 cards have been played! Pause for the human to see the result.
        currentState = GameState::RESOLVING_TRICK;
        stateTimer = 0.0f; // We reused the status timer.
    } else {
        // Pass to the next player (0 -> 1 -> 2 -> 3 -> 0)
        currentPlayer = (currentPlayer + 1) % 4;
    }
}

void Game::resolveTrick() {
    // For now, we're just cleaning the table.
    // In the future, we'll decide here who won the trick and hand over the points!
    tableCards.clear();
    cardsPlayedInTrick = 0;
    
    // For now, the human always starts the next round.
    currentPlayer = 0; 
    currentState = GameState::PLAYING;
}

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
                    rot = 0.0f;
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

void Game::updateTableCardPositions(float deltaTime) {
    for (auto& card : tableCards) {
        card->update(deltaTime);
    }
}

void Game::update(float deltaTime) {
    // The main update loop now acts as a clean state router
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

void Game::updateResolvingTrickState(float deltaTime) {
    stateTimer += deltaTime;
    
    updatePlayerCardPositions(deltaTime);
    updateTableCardPositions(deltaTime);

    // Wait 2 seconds with the cards on the table, then resolve the round.
    if (stateTimer >= 2.0f) {
        resolveTrick();
    }
}

void Game::updateShowingTrumpState(float deltaTime) {
    stateTimer += deltaTime;
    
    // Check if it's time to deal the cards
    if (stateTimer >= 2.0f) {
        transitionToPlayingState();
    }
    
    // Keep updating animations (they will stay smoothly at DECK_SPAWN_POS)
    for (const auto& player : players) {
        for (const auto& card : player->getHand()) {
            card->update(deltaTime); 
        }
    }
    updateTableCardPositions(deltaTime);
}

void Game::transitionToPlayingState() {
    currentState = GameState::PLAYING;
    trumpCardRef->setFaceUp(false); 
    
    for (auto& card : players[0]->getHand()) {
        card->setFaceUp(true);
    }

    // Initialize turns system
    currentPlayer = 0; // The human starts the first turn
    cardsPlayedInTrick = 0;
    cpuTimer = 0.0f;
}

void Game::updatePlayingState(float deltaTime) {
    updatePlayerCardPositions(deltaTime);
    updateTableCardPositions(deltaTime);

    // If it's the CPU's turn, count the time before it plays.
    if (currentPlayer != 0 && cardsPlayedInTrick < 4) {
        cpuTimer += deltaTime;
        if (cpuTimer >= 1.0f) { // 1 second of "thought"
            cpuTimer = 0.0f;
            playCpuTurn();
        }
    }
}


void Game::render() {
    window.clear(sf::Color::Black);
    window.draw(backgroundSprite);

    // Only render the cards, positions are handled by update() now.
    for (const auto& player : players) {
        for (const auto& card : player->getHand()) {
            card->render(window);
        }
    }

    // Render table cards
    for (const auto& card : tableCards) {
        card->render(window);
    }

    window.display();
}

void Game::notifyPlayersCardPlayed(std::shared_ptr<Card> card) {
    for (auto& player : players) {
        player->memorizeCard(card);
    }
}