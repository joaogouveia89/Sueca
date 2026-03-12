#ifndef GAME_CONTEXT_HPP
#define GAME_CONTEXT_HPP

#include <vector>
#include <memory>
#include "Card.hpp"

// A neutral DTO (Data Transfer Object) that represents the current table state
struct GameContext {
    Suit leadSuit;
    Suit trumpSuit;
    bool isFirstToPlay;
    const std::vector<std::shared_ptr<Card>>& tableCards;
};

#endif