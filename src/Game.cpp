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
      backgroundSprite(backgroundTexture),
      team0Score(0),
      team1Score(0)
{
    setupMacOSPath();
    loadAssets();
    currentState = GameState::MAIN_MENU;
}

Game::~Game() {}

// ----------------------------------------------------------------------------
// Initialization Helpers
// ----------------------------------------------------------------------------

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

void Game::loadAssets() {
    if (backgroundTexture.loadFromFile(BG_PATH)) {
        backgroundSprite.setTexture(backgroundTexture, true); 
        
        sf::Vector2u textureSize = backgroundTexture.getSize();
        sf::Vector2u winSize = window.getSize();

        float scaleX = static_cast<float>(winSize.x) / textureSize.x;
        float scaleY = static_cast<float>(winSize.y) / textureSize.y;
        
        backgroundSprite.setScale({scaleX, scaleY});
        backgroundSprite.setPosition({0, 0}); 
    }

    if (uiManager.loadFont(FONT_PATH)) {
        uiManager.setupMainMenu();
        uiManager.updateScoreText(0, 0);
    }
}

void Game::startNewGame() {
    team0Score = 0;
    team1Score = 0;
    uiManager.updateScoreText(team0Score, team1Score);

    team0Pile.clear();
    team1Pile.clear();
    tableCards.clear();
    
    players.clear();
    initializePlayers();

    deck = std::make_unique<CardDeck>();
    deck->shuffle();
    dealCards();
}

void Game::initializePlayers() {
    players.push_back(std::make_unique<Player>(0, "You", false, 40));   
    players.push_back(std::make_unique<Player>(1, "CPU 1", true, 4));   
    players.push_back(std::make_unique<Player>(2, "CPU 2", true, 20));  
    players.push_back(std::make_unique<Player>(3, "CPU 3", true, 40));  
}

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
    trumpCardRef = players[3]->getHand().back();
    trumpCardRef->setFaceUp(true); 
    trumpSuit = trumpCardRef->getSuit();

    currentState = GameState::SHOWING_TRUMP;
    stateTimer = 0.0f;
}

// ----------------------------------------------------------------------------
// Main Game Loop Methods
// ----------------------------------------------------------------------------

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
                sf::Vector2f mousePos = window.mapPixelToCoords(mousePressed->position);
                
                if (currentState == GameState::MAIN_MENU) {
                    handleMainMenuClick(mousePos);
                } else {
                    handleMouseClick(mousePos);
                }
            }
        }
    }
}

void Game::update(float deltaTime) {
    switch (currentState) {
        case GameState::MAIN_MENU: break;
        case GameState::SHOWING_TRUMP: updateShowingTrumpState(deltaTime); break;
        case GameState::PLAYING: updatePlayingState(deltaTime); break;
        case GameState::RESOLVING_TRICK: updateResolvingTrickState(deltaTime); break;
    }
}

void Game::render() {
    window.clear(sf::Color::Black);
    window.draw(backgroundSprite);

    if (currentState == GameState::MAIN_MENU) {
        uiManager.renderMainMenu(window);
    } else {
        for (const auto& card : team0Pile) card->render(window);
        for (const auto& card : team1Pile) card->render(window);
        for (const auto& player : players) {
            for (const auto& card : player->getHand()) card->render(window);
        }
        for (const auto& card : tableCards) card->render(window);
        
        uiManager.renderGameUI(window);
    }

    window.display();
}

// ----------------------------------------------------------------------------
// State Machine Handlers
// ----------------------------------------------------------------------------

void Game::updateShowingTrumpState(float deltaTime) {
    stateTimer += deltaTime;
    if (stateTimer >= 2.0f) transitionToPlayingState();
    
    for (const auto& player : players) {
        for (const auto& card : player->getHand()) card->update(deltaTime); 
    }
    updateTableCardPositions(deltaTime);
    updatePileCardPositions(deltaTime);
}

void Game::transitionToPlayingState() {
    currentState = GameState::PLAYING;
    trumpCardRef->setFaceUp(false); 
    for (auto& card : players[0]->getHand()) card->setFaceUp(true);

    firstPlayer = 0; 
    currentPlayer = 0; 
    cardsPlayedInTrick = 0;
    cpuTimer = 0.0f;
}

void Game::updatePlayingState(float deltaTime) {
    updatePlayerCardPositions(deltaTime);
    updateTableCardPositions(deltaTime);
    updatePileCardPositions(deltaTime);

    if (currentPlayer != 0 && cardsPlayedInTrick < 4) {
        cpuTimer += deltaTime;
        if (cpuTimer >= 1.0f) { 
            cpuTimer = 0.0f;
            playCpuTurn();
        }
    }
}

void Game::updateResolvingTrickState(float deltaTime) {
    stateTimer += deltaTime;
    updatePlayerCardPositions(deltaTime);
    updateTableCardPositions(deltaTime);
    updatePileCardPositions(deltaTime);
    if (stateTimer >= 2.0f) resolveTrick();
}

// ----------------------------------------------------------------------------
// Input & Turn Logic
// ----------------------------------------------------------------------------

void Game::handleMainMenuClick(sf::Vector2f mousePos) {
    MenuAction action = uiManager.handleMenuClick(mousePos);
    
    if (action == MenuAction::START_GAME) {
        startNewGame();
    } else if (action == MenuAction::SHOW_ABOUT) {
        std::cout << "About clicked! We will implement this screen next." << std::endl;
    }
}

void Game::handleMouseClick(sf::Vector2f mousePos) {
    if (currentState != GameState::PLAYING || currentPlayer != 0) return;
    const auto& hand = players[0]->getHand();

    for (int j = static_cast<int>(hand.size()) - 1; j >= 0; --j) {
        if (hand[j]->getBounds().contains(mousePos)) {
            if (!isValidHumanPlay(hand[j])) break; 
            playHumanCard(j);
            break; 
        }
    }
}

void Game::playHumanCard(int cardIndex) {
    std::shared_ptr<Card> playedCard = players[0]->playCard(cardIndex);
    if (playedCard) {
        if (cardsPlayedInTrick == 0) leadSuit = playedCard->getSuit();
        tableCards.push_back(playedCard);
        playedCard->setPosition(getTableTargetPosition(0));
        playedCard->setRotation(0.0f);
        notifyPlayersCardPlayed(playedCard);
        cardsPlayedInTrick++;
        advanceTurn();
    }
}

void Game::playCpuTurn() {
    bool isFirst = (cardsPlayedInTrick == 0);
    GameContext ctx = { leadSuit, trumpSuit, isFirst, tableCards };
    auto playedCard = players[currentPlayer]->thinkAndPlay(ctx);

    if (playedCard) {
        playedCard->setFaceUp(true); 
        if (isFirst) leadSuit = playedCard->getSuit();
        tableCards.push_back(playedCard);
        playedCard->setPosition(getTableTargetPosition(currentPlayer));
        playedCard->setRotation(0.0f);
        notifyPlayersCardPlayed(playedCard);
        cardsPlayedInTrick++;
        advanceTurn();
    }
}

void Game::advanceTurn() {
    if (cardsPlayedInTrick == 4) {
        currentState = GameState::RESOLVING_TRICK;
        stateTimer = 0.0f; 
    } else {
        currentPlayer = (currentPlayer + 1) % 4;
    }
}

void Game::resolveTrick() {
    int trickWinner = determineTrickWinner();
    int winningTeam = trickWinner % 2; 
    int trickPoints = 0;

    for (const auto& card : tableCards) {
        trickPoints += card->getPoints();
        card->setFaceUp(false); 
        
        if (winningTeam == 0) {
            card->setRotation(0.0f); 
            sf::Vector2f target = TEAM0_PILE_POS;
            target.x += team0Pile.size() * 0.2f;
            target.y -= team0Pile.size() * 0.2f;
            card->setPosition(target);
            team0Pile.push_back(card);
        } else {
            card->setRotation(90.0f); 
            sf::Vector2f target = TEAM1_PILE_POS;
            target.x += team1Pile.size() * 0.2f;
            target.y -= team1Pile.size() * 0.2f;
            card->setPosition(target);
            team1Pile.push_back(card);
        }
    }

    if (winningTeam == 0) team0Score += trickPoints;
    else team1Score += trickPoints;

    uiManager.updateScoreText(team0Score, team1Score);

    tableCards.clear();
    cardsPlayedInTrick = 0;
    firstPlayer = trickWinner; 
    currentPlayer = trickWinner; 
    currentState = GameState::PLAYING;
}

// ----------------------------------------------------------------------------
// Game Rules & Validations
// ----------------------------------------------------------------------------

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

int Game::determineTrickWinner() const {
    int winningCardIndex = 0;
    for (int i = 1; i < 4; ++i) {
        auto currentWinningCard = tableCards[winningCardIndex];
        auto challengingCard = tableCards[i];
        bool currentIsTrump = (currentWinningCard->getSuit() == trumpSuit);
        bool challengerIsTrump = (challengingCard->getSuit() == trumpSuit);

        if (challengerIsTrump && !currentIsTrump) winningCardIndex = i;
        else if (challengerIsTrump && currentIsTrump) {
            if (Card::getSuecaPower(challengingCard->getSymbol()) > Card::getSuecaPower(currentWinningCard->getSymbol())) winningCardIndex = i;
        } 
        else if (!challengerIsTrump && !currentIsTrump) {
            if (challengingCard->getSuit() == leadSuit) {
                if (Card::getSuecaPower(challengingCard->getSymbol()) > Card::getSuecaPower(currentWinningCard->getSymbol())) winningCardIndex = i;
            }
        }
    }
    return (firstPlayer + winningCardIndex) % 4;
}

void Game::notifyPlayersCardPlayed(std::shared_ptr<Card> card) {
    for (auto& player : players) player->memorizeCard(card);
}

// ----------------------------------------------------------------------------
// Animation & View Mapping
// ----------------------------------------------------------------------------

void Game::updatePlayerCardPositions(float deltaTime) {
    const float SPACING = 40.0f; 
    for (size_t i = 0; i < players.size(); ++i) {
        const auto& hand = players[i]->getHand();
        for (size_t j = 0; j < hand.size(); ++j) {
            sf::Vector2f target;
            float rot = 0.0f;
            switch (i) {
                case 0: target.x = 400.0f + (j * 50.0f); target.y = 600.0f; break;
                case 1: target.x = 1100.0f; target.y = 200.0f + (j * SPACING); rot = 270.0f; break;
                case 2: target.x = 800.0f - (j * 50.0f); target.y = 120.0f; rot = 180.0f; break;
                case 3: target.x = 180.0f; target.y = 500.0f - (j * SPACING); rot = 90.0f; break;
            }
            hand[j]->setPosition(target);
            hand[j]->setRotation(rot);
            hand[j]->update(deltaTime);
        }
    }
}

void Game::updateTableCardPositions(float deltaTime) {
    for (auto& card : tableCards) card->update(deltaTime);
}

void Game::updatePileCardPositions(float deltaTime) {
    for (auto& card : team0Pile) card->update(deltaTime);
    for (auto& card : team1Pile) card->update(deltaTime);
}

sf::Vector2f Game::getTableTargetPosition(int playerId) const {
    switch (playerId) {
        case 0: return {640.0f, 400.0f}; 
        case 1: return {690.0f, 360.0f}; 
        case 2: return {640.0f, 320.0f}; 
        case 3: return {590.0f, 360.0f}; 
        default: return DECK_SPAWN_POS;
    }
}