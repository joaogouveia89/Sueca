#include "UIManager.hpp"
#include <iostream>

UIManager::UIManager() 
    : scoreText(font),
      titleText(font),
      versionText(font),
      newGameText(font),
      aboutText(font),
      gameOverTitle(font),
      gameOverWinnerText(font),
      backToMenuText(font)
{}

bool UIManager::loadFont(const std::string& fontPath) {
    if (!font.openFromFile(fontPath)) {
        std::cerr << "Failed to load font: " << fontPath << ". Make sure it exists!" << std::endl;
        return false;
    }

    // Setup base score text styling
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setOutlineColor(sf::Color::Black);
    scoreText.setOutlineThickness(2.0f);
    scoreText.setPosition({980.0f, 20.0f});

    setupMainMenu();
    setupGameOverUI();

    return true;
}

void UIManager::setupMainMenu() {
    // Title
    titleText.setString("SUECA");
    titleText.setCharacterSize(80);
    titleText.setFillColor(sf::Color::White);
    titleText.setOutlineColor(sf::Color::Black);
    titleText.setOutlineThickness(4.0f);
    auto titleBounds = titleText.getLocalBounds();
    titleText.setOrigin({titleBounds.size.x / 2.0f, titleBounds.size.y / 2.0f});
    titleText.setPosition({640.0f, 200.0f});

    // Version
    versionText.setString("v1.0.0");
    versionText.setCharacterSize(20);
    versionText.setFillColor(sf::Color(200, 200, 200));
    auto verBounds = versionText.getLocalBounds();
    versionText.setOrigin({verBounds.size.x / 2.0f, verBounds.size.y / 2.0f});
    versionText.setPosition({640.0f, 260.0f});

    // Buttons Setup Helper
    auto setupButton = [](sf::RectangleShape& btn, sf::Text& txt, const std::string& str, float yPos) {
        btn.setSize({250.0f, 60.0f});
        btn.setFillColor(sf::Color(50, 50, 50, 200));
        btn.setOutlineColor(sf::Color::White);
        btn.setOutlineThickness(2.0f);
        btn.setOrigin({125.0f, 30.0f}); // Center of 250x60
        btn.setPosition({640.0f, yPos});

        txt.setString(str);
        txt.setCharacterSize(30);
        txt.setFillColor(sf::Color::White);
        auto txtBounds = txt.getLocalBounds();
        txt.setOrigin({txtBounds.size.x / 2.0f, txtBounds.size.y / 2.0f});
        txt.setPosition({640.0f, yPos - 5.0f}); // Slight offset for visual centering
    };

    setupButton(newGameBtn, newGameText, "New Game", 400.0f);
    setupButton(aboutBtn, aboutText, "About", 480.0f);
}

void UIManager::setupGameOverUI() {
    // Overlay to darken the game screen behind the Game Over text
    overlay.setSize({1280.0f, 720.0f});
    overlay.setFillColor(sf::Color(0, 0, 0, 180)); // Semi-transparent black
    overlay.setPosition({0.0f, 0.0f});

    // Game Over Title
    gameOverTitle.setString("GAME OVER");
    gameOverTitle.setCharacterSize(80);
    gameOverTitle.setFillColor(sf::Color::White);
    gameOverTitle.setOutlineColor(sf::Color::Black);
    gameOverTitle.setOutlineThickness(4.0f);
    auto titleBounds = gameOverTitle.getLocalBounds();
    gameOverTitle.setOrigin({titleBounds.size.x / 2.0f, titleBounds.size.y / 2.0f});
    gameOverTitle.setPosition({640.0f, 150.0f});

    // Winner Text (Text and color will be set dynamically in updateGameOverUI)
    gameOverWinnerText.setCharacterSize(40);
    gameOverWinnerText.setOutlineColor(sf::Color::Black);
    gameOverWinnerText.setOutlineThickness(3.0f);

    // Back to Menu Button
    backToMenuBtn.setSize({300.0f, 60.0f});
    backToMenuBtn.setFillColor(sf::Color(50, 50, 50, 200));
    backToMenuBtn.setOutlineColor(sf::Color::White);
    backToMenuBtn.setOutlineThickness(2.0f);
    backToMenuBtn.setOrigin({150.0f, 30.0f}); // Center of 300x60
    backToMenuBtn.setPosition({640.0f, 450.0f});

    backToMenuText.setString("Back to Menu");
    backToMenuText.setCharacterSize(30);
    backToMenuText.setFillColor(sf::Color::White);
    auto txtBounds = backToMenuText.getLocalBounds();
    backToMenuText.setOrigin({txtBounds.size.x / 2.0f, txtBounds.size.y / 2.0f});
    backToMenuText.setPosition({640.0f, 445.0f});
}

void UIManager::updateScoreText(int team0Score, int team1Score) {
    scoreText.setString("Team Us: " + std::to_string(team0Score) + "\nTeam CPU: " + std::to_string(team1Score));
}

void UIManager::updateGameOverUI(int team0Score, int team1Score) {
    std::string resultStr;
    
    // Total points in Sueca is 120. > 60 wins.
    if (team0Score > 60) {
        resultStr = "Your Team Wins! (" + std::to_string(team0Score) + " pts)";
        gameOverWinnerText.setFillColor(sf::Color::Green);
    } else if (team1Score > 60) {
        resultStr = "CPU Team Wins! (" + std::to_string(team1Score) + " pts)";
        gameOverWinnerText.setFillColor(sf::Color::Red);
    } else {
        resultStr = "It's a Draw! (60 pts each)";
        gameOverWinnerText.setFillColor(sf::Color::Yellow);
    }

    gameOverWinnerText.setString(resultStr);
    auto bounds = gameOverWinnerText.getLocalBounds();
    gameOverWinnerText.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
    gameOverWinnerText.setPosition({640.0f, 280.0f});
}

MenuAction UIManager::handleMenuClick(sf::Vector2f mousePos) {
    if (newGameBtn.getGlobalBounds().contains(mousePos)) {
        return MenuAction::START_GAME;
    } 
    else if (aboutBtn.getGlobalBounds().contains(mousePos)) {
        return MenuAction::SHOW_ABOUT;
    }
    return MenuAction::NONE;
}

MenuAction UIManager::handleGameOverClick(sf::Vector2f mousePos) {
    if (backToMenuBtn.getGlobalBounds().contains(mousePos)) {
        return MenuAction::BACK_TO_MENU;
    }
    return MenuAction::NONE;
}

void UIManager::renderMainMenu(sf::RenderWindow& window) {
    window.draw(titleText);
    window.draw(versionText);
    window.draw(newGameBtn);
    window.draw(newGameText);
    window.draw(aboutBtn);
    window.draw(aboutText);
}

void UIManager::renderGameUI(sf::RenderWindow& window) {
    window.draw(scoreText);
}

void UIManager::renderGameOverUI(sf::RenderWindow& window) {
    // Draws the semi-transparent black overlay and the game over texts
    window.draw(overlay);
    window.draw(gameOverTitle);
    window.draw(gameOverWinnerText);
    window.draw(backToMenuBtn);
    window.draw(backToMenuText);
}