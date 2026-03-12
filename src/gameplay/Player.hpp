#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <vector>
#include <memory>
#include <string>
#include <deque>
#include "Card.hpp"
#include "SuecaAI.hpp"

class Player {
protected:
    // --- Attributes ---
    int id;
    std::string name;
    bool isCPU;
    int memoryCapacity; 
    std::vector<std::shared_ptr<Card>> hand;
    std::deque<std::shared_ptr<Card>> personalMemory;

public:
    // --- Constructor & Destructor ---
    Player(int _id, std::string _name, bool _isCPU = true, int _memoryCapacity = 40);
    virtual ~Player();

    // --- Getters & Setters ---
    bool checkIsCPU() const { return isCPU; }
    std::string getName() const { return name; }
    const std::vector<std::shared_ptr<Card>>& getHand() const { return hand; }
    const std::deque<std::shared_ptr<Card>>& getMemory() const { return personalMemory; }
    void setCards(std::vector<std::shared_ptr<Card>> cards);

    // --- Core Player Actions ---
    void sortHand();
    virtual std::shared_ptr<Card> playCard(int index = 0);
    
    // --- AI & Memory Logic ---
    std::shared_ptr<Card> thinkAndPlay(const GameContext& ctx);
    void memorizeCard(std::shared_ptr<Card> card);
};

#endif