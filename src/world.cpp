
#include "states.h"
#include "world.h"
#include "dimension.h"
#include "player.h"

PyObject * onWorldLoadCallback = NULL;

World::World() {
    for (int i = 0; i < dimensions.size(); i++) {
        this->dimensions.push_back( new Dimension(i) );
    }
}

World * world;

void loadWorld() {
    gameState = STATE_LOADING_GAME;

    player = new Player();

    world = new World();

    PyObject_CallObject(onWorldLoadCallback, Py_None);

    gameState = STATE_IN_GAME;
}