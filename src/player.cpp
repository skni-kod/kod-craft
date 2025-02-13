#include "raylib.h"

#include "player.h"
#include "world.h"

#define renderDistance 2 //todo: replace with a setting

Player* player;

PyObject * onPlayerPositionChangedCallback = NULL;

Player::Player() : Entity() {
    this->cameraOffset = {0, 0, 1.75};
}

double sensitivity = 0.01;

void Player::render() {
    if (!this->initalized) return;

    Camera3D camera = { 0 };

    EntityPosition playerPosition = player->getInterpPosition();
    EntityPosition cameraPosition = playerPosition;
    cameraPosition += this->cameraOffset;

    Rotation* playerRotation = player->getRotation();
    Vector2 mouseDelta = GetMouseDelta();

    playerRotation->rotateYaw(-mouseDelta.x*sensitivity);
    playerRotation->rotatePitch(-mouseDelta.y*sensitivity);

    playerRotation->clamp();

    Vector3 lookVector;

    double playerYaw = playerRotation->getYaw();
    double playerPitch = playerRotation->getPitch();

    lookVector.x = cos(playerYaw) * abs(cos(playerPitch));
    lookVector.y = sin(playerYaw) * abs(cos(playerPitch));
    lookVector.z = sin(playerPitch);

    lookVector.x+= (float)cameraPosition.x;
    lookVector.y+= (float)cameraPosition.y;
    lookVector.z+= (float)cameraPosition.z;

    //TODO
    camera.position = (Vector3){ (float)cameraPosition.x, (float)cameraPosition.y, (float)cameraPosition.z };
    camera.target = lookVector;
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
