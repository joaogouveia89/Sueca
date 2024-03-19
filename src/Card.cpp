#include "Card.hpp"

Card::Card(SDL_Renderer* gRenderer, Suit suit, char symbol){

    this->suit = suit;
    this->symbol = symbol;

    createCardTexture(gRenderer);
}

Card::~Card(){}

void Card::createCardTexture(SDL_Renderer* gRenderer){
    std::stringstream fullImageFilePath;

     fullImageFilePath << IMAGE_FILE_PATH << getFileIdentifier() << IMAGE_FILE_EXTENSION;

    this->texture = std::make_unique<CardTexture>(gRenderer, fullImageFilePath.str());
}

std::string Card::getFileIdentifier(){
    std::stringstream fileIdentifier;

    switch (symbol) {
        case 'J':
            fileIdentifier << "jack_of_";
            break;
        case 'Q':
            fileIdentifier << "queen_of_";
            break;
        case 'K':
            fileIdentifier << "king_of_";
            break;
        case 'A':
            fileIdentifier << "ace_of_";
            break;
    }

    auto asciiSymbol = (int) symbol;

    if(asciiSymbol >= 50 && asciiSymbol <= 55){
        fileIdentifier << symbol << "_of_";
    }


    switch (suit) {
        case Suit::SPADES:
            fileIdentifier <<  "spades";
            break;
        case Suit::HEARTS:
            fileIdentifier << "hearts";
            break;
        case Suit::DIAMONDS:
            fileIdentifier << "diamonds";
            break;
        case Suit::CLUBS:
            fileIdentifier << "clubs";
            break;
    }

    return fileIdentifier.str();
}

void Card::render(){
    if(texture != NULL){
        texture->render(400, 400);
    }
}