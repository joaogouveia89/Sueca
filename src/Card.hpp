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
    
    // Texture references for front and back
    const sf::Texture& frontTexture;
    const sf::Texture& backTexture;
    bool faceUp = true;
    
    // Animation properties
    sf::Vector2f currentPos;
    sf::Vector2f targetPos;
    float rotation;
    float targetRotation;

    // Constants for animation
    static constexpr float MOVE_SPEED = 8.0f;
    static constexpr float ROTATION_SPEED = 10.0f;
    static constexpr float DEFAULT_SCALE = 0.2f;

public:
    Card(Suit s, char sym, const sf::Texture& front, const sf::Texture& back);
    
    void setFaceUp(bool up);
    void render(sf::RenderWindow& window);
    
    Suit getSuit() const { return suit; }
    char getSymbol() const { return symbol; }
    int getPoints() const { return points; }
    
    static int calculatePoints(char sym);
    
    void setPosition(sf::Vector2f pos, bool immediate = false);
    void setPosition(float x, float y, bool immediate = false) { 
        setPosition({x, y}, immediate); 
    }
    
    void setRotation(float angle, bool immediate = false);
    void update(float deltaTime);
};

#endif