#include "Player.hpp"
#include <iostream>

Player::Player(int _id, std::string _name, bool _isCPU) 
    : id(_id), name(_name), isCPU(_isCPU) {}

Player::~Player() {}

void Player::setCards(std::vector<std::shared_ptr<Card>> cards) {
    this->hand = std::move(cards);
}

std::shared_ptr<Card> Player::playCard(int index) {
    if (index < 0 || static_cast<size_t>(index) >= hand.size()) {
        return nullptr;
    }

    auto chosenCard = hand[index];
    hand.erase(hand.begin() + index);
    return chosenCard;
}

int Player::findCardMatchingSuit(Suit leadSuit) const {
    for (size_t i = 0; i < hand.size(); ++i) {
        if (hand[i]->getSuit() == leadSuit) {
            return static_cast<int>(i);
        }
    }
    return -1; // Suit not found
}

std::shared_ptr<Card> Player::thinkAndPlay(Suit leadSuit, bool isFirstToPlay) {
    if (!isCPU || hand.empty()) return nullptr;

    int chosenIndex = 0;

    if (!isFirstToPlay) {
        int matchingSuitIndex = findCardMatchingSuit(leadSuit);
        if (matchingSuitIndex != -1) {
            chosenIndex = matchingSuitIndex;
        }
    }

    return playCard(chosenIndex);
}