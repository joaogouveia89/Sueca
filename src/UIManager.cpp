#include "UIManager.hpp"
#include <iostream>

#include "UIManager.hpp"
#include <iostream>

UIManager::UIManager() 
    : scoreText(font),
      titleText(font),
      versionText(font),
      newGameText(font),
      aboutText(font) 
{}

bool UIManager::loadFont(const std::string& fontPath) {
    if (!font.openFromFile(fontPath)) {
        std::cerr << "Failed to load font: " << fontPath << ". Make sure it exists!" << std::endl;
        return false;
    }

    // Setup base score text styling
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setOutlineColor(sf::Color::Black);
    scoreText.setOutlineThickness(2.0f);
    scoreText.setPosition({980.0f, 20.0f});

    return true;
}

void UIManager::setupMainMenu() {
    // Title
    titleText.setFont(font);
    titleText.setString("SUECA");
    titleText.setCharacterSize(80);
    titleText.setFillColor(sf::Color::White);
    titleText.setOutlineColor(sf::Color::Black);
    titleText.setOutlineThickness(4.0f);
    auto titleBounds = titleText.getLocalBounds();
    titleText.setOrigin({titleBounds.size.x / 2.0f, titleBounds.size.y / 2.0f});
    titleText.setPosition({640.0f, 200.0f});

    // Version
    versionText.setFont(font);
    versionText.setString("v1.0.0");
    versionText.setCharacterSize(20);
    versionText.setFillColor(sf::Color(200, 200, 200));
    auto verBounds = versionText.getLocalBounds();
    versionText.setOrigin({verBounds.size.x / 2.0f, verBounds.size.y / 2.0f});
    versionText.setPosition({640.0f, 260.0f});

    // Buttons Setup Helper
    auto setupButton = [this](sf::RectangleShape& btn, sf::Text& txt, const std::string& str, float yPos) {
        btn.setSize({250.0f, 60.0f});
        btn.setFillColor(sf::Color(50, 50, 50, 200));
        btn.setOutlineColor(sf::Color::White);
        btn.setOutlineThickness(2.0f);
        btn.setOrigin({125.0f, 30.0f}); // Center of 250x60
        btn.setPosition({640.0f, yPos});

        txt.setFont(font);
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

void UIManager::updateScoreText(int team0Score, int team1Score) {
    scoreText.setString("Team Us: " + std::to_string(team0Score) + "\nTeam CPU: " + std::to_string(team1Score));
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