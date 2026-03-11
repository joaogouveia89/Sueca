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

std::shared_ptr<Card> Player::thinkAndPlay(Suit leadSuit, bool isFirstToPlay) {
    if (!isCPU) return nullptr;

    int chosenIndex = 0;

    if (isFirstToPlay) {
        // Se for o primeiro a jogar, joga qualquer uma (ou a maior que tiver)
        chosenIndex = 0; 
    } else {
        // Tenta encontrar uma carta que siga o naipe inicial
        bool foundSuit = false;
        for (int i = 0; i < hand.size(); ++i) {
            if (hand[i]->getSuit() == leadSuit) {
                chosenIndex = i;
                foundSuit = true;
                break;
            }
        }
        // Se não tiver o naipe, joga a primeira carta (corta ou descarta lixo)
        if (!foundSuit) chosenIndex = 0;
    }

    return playCard(chosenIndex);
}