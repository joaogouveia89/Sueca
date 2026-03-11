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

    currentPos = {640.0f, 360.0f};
    targetPos = {640.0f, 360.0f};
    
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

void Card::update(float deltaTime) {
    // Fator de suavização (ajuste conforme o gosto)
    float moveSpeed = 8.0f;
    float rotSpeed = 10.0f;

    // Aproxima a posição atual da posição alvo (Lerp)
    currentPos += (targetPos - currentPos) * moveSpeed * deltaTime;
    
    // Aproxima a rotação (suavização simples)
    rotation += (targetRotation - rotation) * rotSpeed * deltaTime;

    sprite.setPosition(currentPos);
    sprite.setRotation(sf::degrees(rotation));
}


void Card::render(sf::RenderWindow& window) {
    window.draw(sprite);
}