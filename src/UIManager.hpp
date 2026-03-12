#ifndef UI_MANAGER_HPP
#define UI_MANAGER_HPP

#include <SFML/Graphics.hpp>
#include <string>

// Enum to communicate user actions from the UI back to the Game engine
enum class MenuAction {
    NONE,
    START_GAME,
    SHOW_ABOUT,
    BACK_TO_MENU
};

class UIManager {
private:
    sf::Font font;
    
    // --- In-Game UI ---
    sf::Text scoreText;

    // --- Main Menu UI ---
    sf::Text titleText;
    sf::Text versionText;
    sf::RectangleShape newGameBtn;
    sf::Text newGameText;
    sf::RectangleShape aboutBtn;
    sf::Text aboutText;

    // --- Game Over UI ---
    sf::RectangleShape overlay;
    sf::Text gameOverTitle;
    sf::Text gameOverWinnerText;
    sf::RectangleShape backToMenuBtn;
    sf::Text backToMenuText;

public:
    UIManager();
    ~UIManager() = default;

    // Initialization
    bool loadFont(const std::string& fontPath);
    void setupMainMenu();
    void setupGameOverUI();

    // Updates
    void updateScoreText(int team0Score, int team1Score);
    void updateGameOverUI(int team0Score, int team1Score);
    
    MenuAction handleMenuClick(sf::Vector2f mousePos);
    MenuAction handleGameOverClick(sf::Vector2f mousePos);

    // Rendering
    void renderMainMenu(sf::RenderWindow& window);
    void renderGameUI(sf::RenderWindow& window);
    void renderGameOverUI(sf::RenderWindow& window);
};

#endif