#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <string>
#include "CardDeck.hpp"
#include "Player.hpp"
#include "GameContext.hpp"

class Game {
public:
    Game();
    ~Game();

    // Starts the main game loop
    void run(); 

private:
    // --- Constants ---
    const sf::Vector2u WINDOW_SIZE{1280, 720};
    const std::string GAME_TITLE = "Sueca";
    const std::string BG_PATH = "data/bg.png";
    const sf::Vector2f DECK_SPAWN_POS{640.0f, 360.0f};
    
    // --- Enums & Structs ---
    enum class GameState {
        SHOWING_TRUMP,
        PLAYING,
        RESOLVING_TRICK
    };

    // --- Core Game Components ---
    sf::RenderWindow window;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    std::unique_ptr<CardDeck> deck;
    std::vector<std::unique_ptr<Player>> players;
    std::vector<std::shared_ptr<Card>> tableCards;

    // --- Game State Variables ---
    GameState currentState;
    float stateTimer;         // General purpose timer for state transitions
    float cpuTimer;           // Artificial delay for CPU thinking
    std::shared_ptr<Card> trumpCardRef;
    Suit leadSuit;            // The suit of the first card played in the trick
    Suit trumpSuit;           // The dominant suit of the match
    int firstPlayer;          // The player who started the current trick
    int currentPlayer;        // The player whose turn it is right now
    int cardsPlayedInTrick;   // Counter for cards on the table (0 to 4)

    // --- Initialization Helpers ---
    void setupMacOSPath();
    void loadBackground();
    void initializePlayers();
    void dealCards();

    // --- Main Loop Helpers ---
    void processEvents();
    void update(float deltaTime);
    void render();

    // --- State Machine Helpers ---
    void updateShowingTrumpState(float deltaTime);
    void updatePlayingState(float deltaTime);
    void updateResolvingTrickState(float deltaTime);
    void transitionToPlayingState();

    // --- Input & Turn Logic ---
    void handleMouseClick(sf::Vector2f mousePos);
    void playHumanCard(int cardIndex);
    void playCpuTurn();
    void advanceTurn();
    void resolveTrick();

    // --- Game Rules & Validation ---
    bool isValidHumanPlay(const std::shared_ptr<Card>& playedCard) const;
    int determineTrickWinner() const;
    void notifyPlayersCardPlayed(std::shared_ptr<Card> card);

    // --- Animation & View ---
    void updatePlayerCardPositions(float deltaTime);
    void updateTableCardPositions(float deltaTime);
    sf::Vector2f getTableTargetPosition(int playerId) const;
};

#endif