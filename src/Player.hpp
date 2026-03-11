#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <vector>
#include <memory>
#include <string>
#include "Card.hpp"

class Player {
protected:
    int id;
    std::string name;
    std::vector<std::shared_ptr<Card>> hand;
    bool isCPU;

    int findCardMatchingSuit(Suit leadSuit) const;

public:
    Player(int _id, std::string _name, bool _isCPU = true);
    virtual ~Player();

    void setCards(std::vector<std::shared_ptr<Card>> cards);
    const std::vector<std::shared_ptr<Card>>& getHand() const { return hand; }

    virtual std::shared_ptr<Card> playCard(int index = 0);
    std::shared_ptr<Card> thinkAndPlay(Suit leadSuit, bool isFirstToPlay);

    bool checkIsCPU() const { return isCPU; }
    std::string getName() const { return name; }
};

#endif