#ifndef CARDDECK_HPP
#define CARDDECK_HPP

#include <map>
#include <memory>
#include <vector>
#include <algorithm>
#include <random>
#include "Card.hpp"

class CardDeck {
private:
    std::vector<std::shared_ptr<Card>> deck;
    std::map<std::string, sf::Texture> textureCache;
    
    // Constants for asset paths
    const std::string ASSETS_PATH = "data/";
    const std::string CARD_BACK_FILE = "back.png";
    const std::string FILE_EXTENSION = ".png";

    std::string buildFileName(Suit s, char sym);
    void loadBackTexture();
    void generateDeck();

public:
    CardDeck();
    void shuffle();
    std::vector<std::shared_ptr<Card>> drawCards(int count);
};

#endif