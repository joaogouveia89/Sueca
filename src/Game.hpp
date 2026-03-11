#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <string>
#include "CardDeck.hpp"
#include "Player.hpp"

class Game {
public:
    Game();
    ~Game();

    void run(); 

private:

    enum class GameState {
        SHOWING_TRUMP,
        PLAYING
    };

    GameState currentState;
    float stateTimer;
    std::shared_ptr<Card> trumpCardRef;

    // Constants
    const sf::Vector2u WINDOW_SIZE{1280, 720};
    const std::string GAME_TITLE = "Sueca";
    const std::string BG_PATH = "data/bg.png";
    const sf::Vector2f DECK_SPAWN_POS{640.0f, 360.0f};
    
    // Core game components
    sf::RenderWindow window;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    std::unique_ptr<CardDeck> deck;
    std::vector<std::unique_ptr<Player>> players;
    std::vector<std::shared_ptr<Card>> tableCards;

    // Game state variables
    Suit leadSuit;
    Suit trumpSuit;
    int firstPlayer;

    // Initialization helpers
    void setupMacOSPath();
    void loadBackground();
    void initializePlayers();
    void dealCards();

    // Loop helpers
    void processEvents();
    void update(float deltaTime);
    void render();

    // State machine helpers
    void updateShowingTrumpState(float deltaTime);
    void updatePlayingState(float deltaTime);
    void transitionToPlayingState();

    void handleMouseClick(sf::Vector2f mousePos);
    void playHumanCard(int cardIndex);
    
    // Animation helpers
    void updatePlayerCardPositions(float deltaTime);
    void updateTableCardPositions(float deltaTime);
};

#endif