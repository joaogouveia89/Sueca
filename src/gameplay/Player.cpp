#include "Player.hpp"
#include <iostream>
#include <algorithm>

// ----------------------------------------------------------------------------
// Constructor & Destructor
// ----------------------------------------------------------------------------

Player::Player(int _id, std::string _name, bool _isCPU, int _memoryCapacity) 
    : id(_id), name(_name), isCPU(_isCPU), memoryCapacity(_memoryCapacity) {}

Player::~Player() {}

// ----------------------------------------------------------------------------
// Hand Management
// ----------------------------------------------------------------------------

// Assigns a dealt hand to the player
void Player::setCards(std::vector<std::shared_ptr<Card>> cards) {
    this->hand = std::move(cards);
}

// Sorts the player's hand primarily by Suit, and secondarily by Sueca Power
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

// Removes and returns the card at the specified index from the player's hand
std::shared_ptr<Card> Player::playCard(int index) {
    if (index < 0 || static_cast<size_t>(index) >= hand.size()) {
        return nullptr;
    }

    auto chosenCard = hand[index];
    hand.erase(hand.begin() + index);
    return chosenCard;
}

// ----------------------------------------------------------------------------
// AI & Memory Logic
// ----------------------------------------------------------------------------

// Delegates decision making to SuecaAI and plays the chosen card
std::shared_ptr<Card> Player::thinkAndPlay(const GameContext& ctx) {
    if (!isCPU || hand.empty()) return nullptr;

    // Ask the AI for the best move based on the hand, memory, and current table state
    int chosenIndex = SuecaAI::chooseBestCardIndex(hand, personalMemory, ctx);

    // Fallback in case of an unexpected AI error
    if (chosenIndex == -1) {
        chosenIndex = 0;
    }

    return playCard(chosenIndex);
}

// Adds a played card to the player's memory, forgetting the oldest if capacity is reached
void Player::memorizeCard(std::shared_ptr<Card> card) {
    if (memoryCapacity <= 0) return; // Player with goldfish memory (e.g., Dory)

    personalMemory.push_back(card);

    // If capacity is exceeded, forget the oldest card (the one at the front of the queue)
    if (personalMemory.size() > static_cast<size_t>(memoryCapacity)) {
        personalMemory.pop_front(); 
    }
}