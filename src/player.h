#ifndef PLAYER_H
#define PLAYER_H

class Player;

extern Player* player;

#include "dimension.h"
#include "entity.h"

class Player : public Entity {
private:

public:
    Player();

    void render();

    void processTick();
};

#endif
