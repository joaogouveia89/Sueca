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
    players.push_back(std::make_unique<Player>(0, "You", false)); // Human
    players.push_back(std::make_unique<Player>(1, "CPU 1", true));
    players.push_back(std::make_unique<Player>(2, "CPU 2", true));
    players.push_back(std::make_unique<Player>(3, "CPU 3", true));
}

void Game::dealCards() {
    const int CARDS_PER_PLAYER = 10;

    for (int i = 0; i < players.size(); i++) {
        auto dealtCards = deck->drawCards(CARDS_PER_PLAYER);
        
        for (auto& card : dealtCards) {
            card->setPosition(DECK_SPAWN_POS, true); 
            card->setRotation(0.0f, true);
        }
        players[i]->setCards(dealtCards);
    }
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
    }
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
    updatePlayerCardPositions(deltaTime);
    updateTableCardPositions(deltaTime);
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