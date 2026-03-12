#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <string>
#include "CardDeck.hpp"
#include "Player.hpp"
#include "GameContext.hpp"
#include "UIManager.hpp"

class Game {
public:
    Game();
    ~Game();

    void run(); 

private:
    const sf::Vector2u WINDOW_SIZE{1280, 720};
    const std::string GAME_TITLE = "Sueca";
    const std::string BG_PATH = "data/bg.png";
    const std::string FONT_PATH = "data/arial.ttf";
    const sf::Vector2f DECK_SPAWN_POS{640.0f, 360.0f};
    
    const sf::Vector2f TEAM0_PILE_POS{920.0f, 620.0f}; 
    const sf::Vector2f TEAM1_PILE_POS{200.0f, 100.0f}; 
    
    enum class GameState {
        MAIN_MENU,
        SHOWING_TRUMP,
        PLAYING,
        RESOLVING_TRICK,
        GAME_OVER
    };

    sf::RenderWindow window;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    
    // --- Sub-Systems ---
    UIManager uiManager;

    std::unique_ptr<CardDeck> deck;
    std::vector<std::unique_ptr<Player>> players;
    std::vector<std::shared_ptr<Card>> tableCards;
    
    std::vector<std::shared_ptr<Card>> team0Pile;
    std::vector<std::shared_ptr<Card>> team1Pile;

    GameState currentState;
    float stateTimer;         
    float cpuTimer;           
    std::shared_ptr<Card> trumpCardRef;
    Suit leadSuit;            
    Suit trumpSuit;           
    int firstPlayer;          
    int currentPlayer;        
    int cardsPlayedInTrick;   

    int team0Score;           
    int team1Score;           

    void setupMacOSPath();
    void loadAssets();
    void initializePlayers();
    void startNewGame();
    void dealCards();

    void processEvents();
    void update(float deltaTime);
    void render();

    void updateShowingTrumpState(float deltaTime);
    void transitionToPlayingState();
    void updatePlayingState(float deltaTime);
    void updateResolvingTrickState(float deltaTime);
    void transitionToGameOverState();

    void handleMainMenuClick(sf::Vector2f mousePos);
    void handleGameOverClick(sf::Vector2f mousePos);
    void handleMouseClick(sf::Vector2f mousePos);
    
    void playHumanCard(int cardIndex);
    void playCpuTurn();
    void advanceTurn();
    void resolveTrick();

    bool isValidHumanPlay(const std::shared_ptr<Card>& playedCard) const;
    int determineTrickWinner() const;
    void notifyPlayersCardPlayed(std::shared_ptr<Card> card);

    void updatePlayerCardPositions(float deltaTime);
    void updateTableCardPositions(float deltaTime);
    void updatePileCardPositions(float deltaTime);
    sf::Vector2f getTableTargetPosition(int playerId) const;
};

#endif