#if !defined(PLAYER_HPP)
#define PLAYER_HPP

#include<string>
#include<vector>
#include<memory>
#include "Card.hpp"

class Player
{
private:
    int id;
    std::string name;
    std::vector<std::shared_ptr<Card>> earnedCards;
    std::vector<std::shared_ptr<Card>> playableCards;
public:
    Player(int _id, std::string _name);
    ~Player();

    void SetCards(std::vector<std::shared_ptr<Card>> cards);
};

#endif // PLAYER_HPP
