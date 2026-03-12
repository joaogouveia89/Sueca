#include "SuecaAI.hpp"
#include <algorithm>

int SuecaAI::chooseBestCardIndex(const std::vector<std::shared_ptr<Card>>& hand, 
                                 const std::deque<std::shared_ptr<Card>>& memory, 
                                 const GameContext& ctx) 
{
    if (hand.empty()) return -1; // Should never happen

    if (ctx.isFirstToPlay) {
        return playAsFirst(hand, ctx);
    } else {
        return playAsFollower(hand, memory, ctx);
    }
}

int SuecaAI::playAsFirst(const std::vector<std::shared_ptr<Card>>& hand, const GameContext& ctx) {
    // When playing first, a basic strategy is to play the lowest card to see what others do.
    // (We consider all cards since there's no lead suit restriction yet)
    std::vector<int> allIndices(hand.size());
    for(size_t i = 0; i < hand.size(); ++i) allIndices[i] = i;
    
    return getLowestCardIndex(hand, allIndices);
}

int SuecaAI::playAsFollower(const std::vector<std::shared_ptr<Card>>& hand, 
                            const std::deque<std::shared_ptr<Card>>& memory, 
                            const GameContext& ctx) 
{
    std::vector<int> validIndices = getValidCardIndices(hand, ctx.leadSuit);

    if (!validIndices.empty()) {
        // --- WE HAVE THE LEAD SUIT (Must follow suit) ---
        
        // The "Malice" Check: Is the Ace of this suit already out?
        bool isAceOut = hasCardBeenPlayed('A', ctx.leadSuit, memory, ctx.tableCards);

        if (isAceOut) {
            // The coast is clear! We can safely play our highest card to try and win.
            return getHighestCardIndex(hand, validIndices);
        } else {
            // DANGER: The Ace is still hiding somewhere! 
            // Don't waste a 7 or a King. Play the lowest card we have.
            return getLowestCardIndex(hand, validIndices);
        }
    } else {
        // --- WE DON'T HAVE THE LEAD SUIT (Can cut with Trump or discard) ---
        
        std::vector<int> trumpIndices = getValidCardIndices(hand, ctx.trumpSuit);
        
        if (!trumpIndices.empty()) {
            // Cut with the lowest trump possible to win the trick cheaply
            return getLowestCardIndex(hand, trumpIndices);
        } else {
            // No trumps either. Just discard the absolute lowest card from our hand
            std::vector<int> allIndices(hand.size());
            for(size_t i = 0; i < hand.size(); ++i) allIndices[i] = i;
            return getLowestCardIndex(hand, allIndices);
        }
    }
}

// ----------------------------------------------------------------------------
// Utility Methods
// ----------------------------------------------------------------------------

std::vector<int> SuecaAI::getValidCardIndices(const std::vector<std::shared_ptr<Card>>& hand, Suit suit) {
    std::vector<int> indices;
    for (size_t i = 0; i < hand.size(); ++i) {
        if (hand[i]->getSuit() == suit) {
            indices.push_back(i);
        }
    }
    return indices;
}

int SuecaAI::getLowestCardIndex(const std::vector<std::shared_ptr<Card>>& hand, const std::vector<int>& indicesToConsider) {
    int bestIndex = indicesToConsider[0];
    int lowestPower = Card::getSuecaPower(hand[bestIndex]->getSymbol());

    for (int idx : indicesToConsider) {
        int power = Card::getSuecaPower(hand[idx]->getSymbol());
        if (power < lowestPower) {
            lowestPower = power;
            bestIndex = idx;
        }
    }
    return bestIndex;
}

int SuecaAI::getHighestCardIndex(const std::vector<std::shared_ptr<Card>>& hand, const std::vector<int>& indicesToConsider) {
    int bestIndex = indicesToConsider[0];
    int highestPower = Card::getSuecaPower(hand[bestIndex]->getSymbol());

    for (int idx : indicesToConsider) {
        int power = Card::getSuecaPower(hand[idx]->getSymbol());
        if (power > highestPower) {
            highestPower = power;
            bestIndex = idx;
        }
    }
    return bestIndex;
}

bool SuecaAI::hasCardBeenPlayed(char symbol, Suit suit, const std::deque<std::shared_ptr<Card>>& memory, const std::vector<std::shared_ptr<Card>>& tableCards) {
    // 1. Check current table
    for (const auto& card : tableCards) {
        if (card->getSymbol() == symbol && card->getSuit() == suit) return true;
    }
    // 2. Check player's memory
    for (const auto& card : memory) {
        if (card->getSymbol() == symbol && card->getSuit() == suit) return true;
    }
    return false;
}