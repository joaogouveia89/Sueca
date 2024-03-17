#if !defined(CARD_HPP)
#define CARD_HPP
#include <memory>
#include <string>
#include <sstream>
#include "CardTexture.hpp"

enum class Suit{
    SPADES, HEARTS, DIAMONDS, CLUBS
};

class Card
{
private:
    Suit suit;
    char symbol;
    int points;
    
    std::unique_ptr<CardTexture> texture;
    int x;
    int y;
    const std::string IMAGE_FILE_PATH = "../data/";
    const std::string IMAGE_FILE_EXTENSION = ".png";

    void createCardTexture(SDL_Renderer* gRenderer);
    std::string getFileIdentifier();

public:
    Card(SDL_Renderer* gRenderer, Suit suit, char symbol);
    ~Card();

    void render();

    Suit GetSuit() const{ return suit; }
    char GetSymbol() const{ return symbol; }
};


#endif // CARD_HPP
