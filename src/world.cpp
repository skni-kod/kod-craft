
#include "states.h"
#include "world.h"
#include "dimension.h"
#include "player.h"

#include <chrono>

PyObject * onWorldLoadCallback = NULL;

bool keepTickProcessingGoing;

World::World() {
    for (int i = 0; i < dimensionList.size(); i++) {
        this->dimensions.push_back( new Dimension(i) );
    }
}

World::~World() {
    keepTickProcessingGoing = false;
    tickProcessingTrhead.join();
    for (int i = 0; i < dimensionList.size(); i++) {
        delete this->dimensions[i];
    }
}

World * world;

std::chrono::time_point<std::chrono::high_resolution_clock> lastTickDoneTime;
std::chrono::time_point<std::chrono::high_resolution_clock> tickDoneTargetTime;


void processTicksThreadFunction(World * world) {
    while (keepTickProcessingGoing) {
        tickDoneTargetTime = std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(1000 / tickRate);
        world->processTick();
        lastTickDoneTime = std::chrono::high_resolution_clock::now();
        std::this_thread::sleep_until(tickDoneTargetTime);
    }
}

void World::startTickProcessing() {
    if (gameState == STATE_LOADING_GAME) gameState = STATE_IN_GAME;
    else return;
    keepTickProcessingGoing = true;

    lastTickDoneTime = std::chrono::high_resolution_clock::now();
    
    tickProcessingTrhead = std::thread(&processTicksThreadFunction, this);
}

void loadWorld() {
    gameState = STATE_LOADING_GAME;

    player = new Player();

    world = new World();

    if (onWorldLoadCallback!=NULL) PyObject_CallObject(onWorldLoadCallback, NULL);

    player->initalize();
    world->startTickProcessing();
}

void unloadWorld() {
    gameState = STATE_MAIN_MENU;
    delete world;
    delete player;
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
