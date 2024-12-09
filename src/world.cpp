
#include "states.h"
#include "world.h"

World * world;

void loadWorld() {
    gameState = STATE_LOADING_GAME;

    gameState = STATE_IN_GAME;
}