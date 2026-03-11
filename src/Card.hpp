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
    sf::Sprite sprite;
    
    // Adicionamos referências para as duas texturas
    const sf::Texture& frontTexture;
    const sf::Texture& backTexture;
    bool faceUp = true;
    sf::Vector2f currentPos;
    sf::Vector2f targetPos;
    float rotation;
    float targetRotation;

public:
    // O construtor agora passa a textura adiante para o sprite
    Card(Suit s, char sym, const sf::Texture& front, const sf::Texture& back);
    
    void setFaceUp(bool up);
    void render(sf::RenderWindow& window);
    
    Suit getSuit() const { return suit; }
    char getSymbol() const { return symbol; }
    int getPoints() const { return points; }
    
    static int calculatePoints(char sym);
    // Card.hpp
    void setPosition(sf::Vector2f pos, bool immediate = false);
    // Adicione esta linha para suportar o código antigo (x, y):
    void setPosition(float x, float y, bool immediate = false) { 
        setPosition({x, y}, immediate); 
    }
    void setRotation(float angle, bool immediate = false);

    void update(float deltaTime);
};

#endif