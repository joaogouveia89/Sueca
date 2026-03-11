#include "Player.hpp"
#include <iostream>

Player::Player(int _id, std::string _name, bool _isCPU) 
    : id(_id), name(_name), isCPU(_isCPU) {}

Player::~Player() {}

void Player::setCards(std::vector<std::shared_ptr<Card>> cards) {
    this->hand = std::move(cards);
}

std::shared_ptr<Card> Player::playCard(int index) {
    if (index < 0 || (size_t)index >= hand.size()) return nullptr;

    auto chosenCard = hand[index];
    hand.erase(hand.begin() + index);
    return chosenCard;
}

std::shared_ptr<Card> Player::thinkAndPlay(Suit leadSuit, Suit trumpSuit) {
    if (!isCPU) return nullptr;
    // Por enquanto, apenas joga a primeira carta válida
    return playCard(0);
}