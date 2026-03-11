#include "Card.hpp"

Card::Card(Suit s, char sym, const sf::Texture& front, const sf::Texture& back) 
    : suit(s), symbol(sym), frontTexture(front), backTexture(back), sprite(front)
{
    // Setup sprite origin to its center
    auto bounds = sprite.getLocalBounds();
    sprite.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
    sprite.setScale({DEFAULT_SCALE, DEFAULT_SCALE});
    
    faceUp = true;
    points = calculatePoints(sym);

    // Default spawn position (Center of a 1280x720 screen)
    currentPos = {640.0f, 360.0f};
    targetPos = {640.0f, 360.0f};
    rotation = 0.0f;
    targetRotation = 0.0f;
    
    sprite.setPosition(currentPos);
}

void Card::setFaceUp(bool up) {
    faceUp = up;
    sprite.setTexture(faceUp ? frontTexture : backTexture);
}

int Card::calculatePoints(char sym) {
    switch(sym) {
        case 'A': return 11;
        case '7': return 10;
        case 'K': return 4;
        case 'J': return 3;
        case 'Q': return 2;
        default:  return 0;
    }
}

void Card::setPosition(sf::Vector2f pos, bool immediate) {
    targetPos = pos;
    if (immediate) {
        currentPos = pos;
        sprite.setPosition(currentPos);
    }
}

void Card::setRotation(float angle, bool immediate) {
    targetRotation = angle;
    if (immediate) {
        rotation = angle;
        sprite.setRotation(sf::degrees(rotation));
    }
}

sf::FloatRect Card::getBounds() const {
    return sprite.getGlobalBounds();
}

void Card::update(float deltaTime) {
    // Linear Interpolation (Lerp) for smooth movement
    currentPos += (targetPos - currentPos) * MOVE_SPEED * deltaTime;
    
    // Simple smoothing for rotation
    rotation += (targetRotation - rotation) * ROTATION_SPEED * deltaTime;

    sprite.setPosition(currentPos);
    sprite.setRotation(sf::degrees(rotation));
}

void Card::render(sf::RenderWindow& window) {
    window.draw(sprite);
}