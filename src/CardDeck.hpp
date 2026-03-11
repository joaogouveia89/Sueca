// CardDeck.hpp
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
    
    std::string getFileName(Suit s, char sym);

public:
    CardDeck();
    void shuffle();
    std::vector<std::shared_ptr<Card>> drawCards(int count);
};