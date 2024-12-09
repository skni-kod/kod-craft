
#include "states.h"
#include "world.h"

World::World() {}

World * world;

void loadWorld() {
    gameState = STATE_LOADING_GAME;

    world = new World();

    gameState = STATE_IN_GAME;
}