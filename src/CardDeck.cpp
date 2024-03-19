#include "CardDeck.hpp"

CardDeck::CardDeck(SDL_Renderer* gRenderer): 
    symbolsList{'2', '3', '4', '5', '6', '7', 'J', 'Q', 'K', 'A'}
{
    // creating cards

    for(auto currentSymbol : symbolsList){
        deck.push_back(std::make_shared<Card>(gRenderer, Suit::SPADES, currentSymbol));
        deck.push_back(std::make_shared<Card>(gRenderer, Suit::CLUBS, currentSymbol));
        deck.push_back(std::make_shared<Card>(gRenderer, Suit::HEARTS, currentSymbol));
        deck.push_back(std::make_shared<Card>(gRenderer, Suit::DIAMONDS, currentSymbol));
    }
    std::cout << "Deck created" << std::endl;
}

CardDeck::~CardDeck(){}