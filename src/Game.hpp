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

    void run(); // Vamos usar 'run' em vez de 'gameLoop' para seguir o padrão moderno

private:
    private:
    std::vector<std::shared_ptr<Card>> tableCards;
    Suit leadSuit;
    Suit trumpSuit;
    int firstPlayer;

    void processEvents();
    void update();
    void render();

    // SFML 3 usa Vector2u para tamanhos de janela
    const sf::Vector2u windowSize{1280, 720};
    const std::string gameTitle = "Sueca SFML 3";

    sf::RenderWindow window;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    std::unique_ptr<CardDeck> deck;
    std::vector<std::unique_ptr<Player>> players;
};

#endif