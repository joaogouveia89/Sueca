#include "CardDeck.hpp"
#include <iostream>

CardDeck::CardDeck() {
    loadBackTexture();
    generateDeck();
}

void CardDeck::loadBackTexture() {
    std::string fullPath = ASSETS_PATH + CARD_BACK_FILE;
    if (!textureCache["back"].loadFromFile(fullPath)) {
        std::cerr << "Error loading card back texture: " << fullPath << std::endl;
    }
}

void CardDeck::generateDeck() {
    char symbols[] = {'2', '3', '4', '5', '6', '7', 'J', 'Q', 'K', 'A'};
    Suit suits[] = {Suit::SPADES, Suit::HEARTS, Suit::DIAMONDS, Suit::CLUBS};

    for (auto s : suits) {
        for (auto sym : symbols) {
            std::string name = buildFileName(s, sym);
            
            // Load texture if not already cached
            if (textureCache.find(name) == textureCache.end()) {
                std::string fullPath = ASSETS_PATH + name + FILE_EXTENSION;
                if (!textureCache[name].loadFromFile(fullPath)) {
                    std::cerr << "Failed to load texture: " << fullPath << std::endl;
                }
            }
            
            deck.push_back(std::make_shared<Card>(s, sym, textureCache[name], textureCache["back"]));
        }
    }
}

std::string CardDeck::buildFileName(Suit s, char sym) {
    std::string fileName;
    
    if (sym == 'J') fileName = "jack";
    else if (sym == 'Q') fileName = "queen";
    else if (sym == 'K') fileName = "king";
    else if (sym == 'A') fileName = "ace";
    else fileName = std::string(1, sym);

    fileName += "_of_";
    
    if (s == Suit::SPADES) fileName += "spades";
    else if (s == Suit::HEARTS) fileName += "hearts";
    else if (s == Suit::DIAMONDS) fileName += "diamonds";
    else fileName += "clubs";
    
    return fileName;
}

void CardDeck::shuffle() {
    std::random_device rd;
    std::default_random_engine rng(rd());
    std::shuffle(deck.begin(), deck.end(), rng);
}

std::vector<std::shared_ptr<Card>> CardDeck::drawCards(int count) {
    std::vector<std::shared_ptr<Card>> hand;
    
    // Ensure we don't draw more cards than available
    int actualCount = std::min(count, static_cast<int>(deck.size()));

    for (int i = 0; i < actualCount; ++i) {
        hand.push_back(deck.back());
        deck.pop_back();
    }
    
    return hand;
}