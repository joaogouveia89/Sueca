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

void CardDeck::Shuffle() {
    auto rd = std::random_device {}; 
    auto rng = std::default_random_engine { rd() };
    std::shuffle(std::begin(deck), std::end(deck), rng);
}

std::vector<std::shared_ptr<Card>> CardDeck::Get10Cards(){
    std::vector<std::shared_ptr<Card>> subDeck = {deck.begin(), deck.begin() + 10};
    // Remove the first 10 cards from the main deck
    deck.erase(deck.begin(), deck.begin() + 10);
    return std::move(subDeck);
}