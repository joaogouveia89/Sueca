#ifndef SUECA_AI_HPP
#define SUECA_AI_HPP

#include <vector>
#include <memory>
#include <deque>
#include "Card.hpp"

// Moved from Game.hpp to avoid Circular Dependencies
struct GameContext {
    Suit leadSuit;
    Suit trumpSuit;
    bool isFirstToPlay;
    const std::vector<std::shared_ptr<Card>>& tableCards;
};

class SuecaAI {
public:
    // Main decision entry point
    static int chooseBestCardIndex(
        const std::vector<std::shared_ptr<Card>>& hand, 
        const std::deque<std::shared_ptr<Card>>& memory, 
        const GameContext& ctx
    );

private:
    // Strategy handlers
    static int playAsFirst(const std::vector<std::shared_ptr<Card>>& hand, const GameContext& ctx);
    static int playAsFollower(const std::vector<std::shared_ptr<Card>>& hand, const std::deque<std::shared_ptr<Card>>& memory, const GameContext& ctx);

    // AI Utilities
    static std::vector<int> getValidCardIndices(const std::vector<std::shared_ptr<Card>>& hand, Suit leadSuit);
    static int getLowestCardIndex(const std::vector<std::shared_ptr<Card>>& hand, const std::vector<int>& indicesToConsider);
    static int getHighestCardIndex(const std::vector<std::shared_ptr<Card>>& hand, const std::vector<int>& indicesToConsider);
    static bool hasCardBeenPlayed(char symbol, Suit suit, const std::deque<std::shared_ptr<Card>>& memory, const std::vector<std::shared_ptr<Card>>& tableCards);
};

#endif