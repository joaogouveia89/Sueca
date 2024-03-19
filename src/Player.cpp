#include "Player.hpp"

Player::Player(int _id, std::string _name){
    this->id = _id;
    this->name = _name;
}

Player::~Player(){}

void Player::SetCards(std::vector<std::shared_ptr<Card>> cards){
    this->playableCards = cards;
}