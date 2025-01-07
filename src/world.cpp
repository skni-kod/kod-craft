
#include "states.h"
#include "world.h"
#include "dimension.h"
#include "player.h"

#include <chrono>

PyObject * onWorldLoadCallback = NULL;

World::World() {
    for (int i = 0; i < dimensionList.size(); i++) {
        this->dimensions.push_back( new Dimension(i) );
    }
}

World * world;

void processTicksThreadFunction(World * world) {
    while (1) {
        auto targetTime = std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(1000 / tickRate);
        world->processTick();
        std::this_thread::sleep_until(targetTime);
    }
}

void World::startTickProcessing() {
    if (gameState == STATE_LOADING_GAME) gameState = STATE_IN_GAME;
    else return;
    
    tickProcessingTrhead = std::thread(&processTicksThreadFunction, this);
}

void loadWorld() {
    gameState = STATE_LOADING_GAME;

    player = new Player();

    world = new World();

    PyObject_CallObject(onWorldLoadCallback, NULL);

    world->startTickProcessing();
}

Dimension* World::findDimension(std::string name) {
    for (int i = 0; i < this->dimensions.size(); i++) {
        if (this->dimensions[i]->getName() == name) return this->dimensions[i];
    }

    return NULL;
}

void World::processTick() {
    player->processTick();

    for (int i = 0; i < this->dimensions.size(); i++) {
        this->dimensions[i]->processTick();
    }
}
