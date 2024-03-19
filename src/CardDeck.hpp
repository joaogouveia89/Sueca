#if !defined(CARD_DECK_HPP)
#define CARD_DECK_HPP

#include <vector>
#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Card.hpp"

class CardDeck
{
private:
    std::vector<std::shared_ptr<Card>> deck;
    char symbolsList[10];

public:
    CardDeck(SDL_Renderer* gRenderer);

    std::shared_ptr<Card> GetCard(int idx){ return deck.at(idx); }

    ~CardDeck();
};


#endif // CARD_DECK_HPP
