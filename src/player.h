#ifndef PLAYER_H
#define PLAYER_H

class Player;

extern Player* player;

#include "dimension.h"
#include "entity.h"

class Player : public Entity {
private:
    void onPositionChanged();

    EntityPosition cameraOffset;
public:
    Player();

    void render();

    void processTick();
};

extern PyObject * onPlayerPositionChangedCallback;

#endif
