
#include "states.h"
#include "world.h"
#include "dimension.h"
#include "player.h"

PyObject * onWorldLoadCallback = NULL;

World::World() {
    for (int i = 0; i < dimensionList.size(); i++) {
        this->dimensions.push_back( new Dimension(i) );
    }
}

World * world;

void loadWorld() {
    gameState = STATE_LOADING_GAME;

    player = new Player();

    world = new World();

    PyObject_CallObject(onWorldLoadCallback, NULL);

    gameState = STATE_IN_GAME;
}

Dimension* World::findDimension(std::string name) {
    for (int i = 0; i < this->dimensions.size(); i++) {
        if (this->dimensions[i]->getName() == name) return this->dimensions[i];
    }

    return NULL;
}

void World::processTick() {
    for (int i = 0; i < this->dimensions.size(); i++) {
        this->dimensions[i]->processTick();
    }
}
