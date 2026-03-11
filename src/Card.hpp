#ifndef CARD_HPP
#define CARD_HPP

#include <SFML/Graphics.hpp>
#include <string>

enum class Suit { SPADES, HEARTS, DIAMONDS, CLUBS };

class Card {
private:
    Suit suit;
    char symbol;
    int points;
    sf::Sprite sprite; // No SFML 3, isso será inicializado no construtor da classe

public:
    // O construtor agora passa a textura adiante para o sprite
    Card(Suit s, char sym, const sf::Texture& tex);
    
    void setPosition(float x, float y);
    void render(sf::RenderWindow& window);
    
    Suit getSuit() const { return suit; }
    char getSymbol() const { return symbol; }
    int getPoints() const { return points; }
    
    static int calculatePoints(char sym);
};

#endif