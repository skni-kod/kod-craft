#include "raylib.h"

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
    Camera3D camera = { 0 };

    //TODO
    camera.position = (Vector3){ (float)this->x, (float)this->y, (float)this->z };
    camera.target = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 0.0f, 1.0f };
    camera.fovy = 90.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    BeginMode3D(camera);
    this->dimension->draw();
    EndMode3D();
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
