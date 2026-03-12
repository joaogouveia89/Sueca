#include "Player.hpp"
#include <iostream>

Player::Player(int _id, std::string _name, bool _isCPU, int _memoryCapacity) 
    : id(_id), name(_name), isCPU(_isCPU), memoryCapacity(_memoryCapacity) {}

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

void Player::sortHand() {
    std::sort(hand.begin(), hand.end(), [](const std::shared_ptr<Card>& a, const std::shared_ptr<Card>& b) {
        // 1st Criterion: Group by Suit
        if (a->getSuit() != b->getSuit()) {
            return a->getSuit() < b->getSuit();
        }
        // 2nd Criterion: Descending Order of Power (from greatest to least)
        return Card::getSuecaPower(a->getSymbol()) > Card::getSuecaPower(b->getSymbol());
    });
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

void Player::memorizeCard(std::shared_ptr<Card> card) {
    if (memoryCapacity <= 0) return; // Jogador com memória de peixe (Dory)

    personalMemory.push_back(card);

    // Se excedeu a capacidade, esquece a carta mais antiga (a que está na frente da fila)
    if (personalMemory.size() > memoryCapacity) {
        personalMemory.pop_front(); 
    }
}