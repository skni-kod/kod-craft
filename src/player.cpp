#include "player.h"

Player::Player() {
    this->x = 0;
    this->y = 0;
    this->z = 0;
}

void Player::draw() {
    this->dimension->draw();
}
