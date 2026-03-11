#include "Card.hpp"

// Correção 1: Inicializar o sprite explicitamente na lista de inicialização
Card::Card(Suit s, char sym, const sf::Texture& front, const sf::Texture& back) 
    : suit(s), symbol(sym), frontTexture(front), backTexture(back), sprite(front) 
{
    // Agora o sprite já existe, podemos configurar o resto
    auto bounds = sprite.getLocalBounds();
    sprite.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
    sprite.setScale({0.2f, 0.2f});
    
    faceUp = true; // Por padrão começa visível
    points = calculatePoints(sym);
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

void Card::setPosition(float x, float y) {
    // Correção 3: setPosition agora pede um sf::Vector2f
    sprite.setPosition({x, y});
}

void Card::render(sf::RenderWindow& window) {
    window.draw(sprite);
}