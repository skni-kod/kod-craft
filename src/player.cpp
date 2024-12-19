#include "player.h"
#include "world.h"

#define renderDistance 10 //todo: replace with a setting

Player* player;

Player::Player() {
    this->x = 0;
    this->y = 0;
    this->z = 0;
}

void Player::draw() {
    this->dimension->draw();
}

void Player::setDimension(Dimension* dimension) {
    this->dimension = dimension;
}

void Player::setDimension(std::string dimension) {
    Dimension* dimensionFound = world->findDimension(dimension);

    if (dimensionFound==NULL) return;

    this->setDimension(dimensionFound);
}

void Player::processTick() {
    this->dimension->loadAroundPosition((WorldPos)this->x, (WorldPos)this->y, (WorldPos)this->z, renderDistance);
}
