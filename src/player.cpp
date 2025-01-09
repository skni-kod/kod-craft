#include "raylib.h"

#include "player.h"
#include "world.h"

#define renderDistance 2 //todo: replace with a setting

Player* player;

PyObject * onPlayerPositionChangedCallback = NULL;

Player::Player() : Entity() {
}

void Player::render() {
    if (!this->initalized) return;

    Camera3D camera = { 0 };

    //TODO
    camera.position = (Vector3){ (float)this->pos.x, (float)this->pos.y, (float)this->pos.z };
    camera.target = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 0.0f, 1.0f };
    camera.fovy = 90.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    BeginMode3D(camera);
    this->dimension->draw();
    EndMode3D();
}

void Player::processTick() {
    if (!this->initalized) return;

    this->Entity::processTick();

    this->dimension->loadAroundPosition((WorldPos)this->pos.x, (WorldPos)this->pos.y, (WorldPos)this->pos.z, renderDistance);
}

void Player::onPositionChanged() {
    if (onPlayerPositionChangedCallback) PyObject_CallObject(onPlayerPositionChangedCallback, NULL);
}
