#define USE_PYTHON

#include "states.h"
#include "world.h"
#include "dimension.h"
#include "player.h"
#include "spinlock.h"

#include <chrono>

PyObject * onWorldLoadCallback = NULL;
extern PyThreadState* mainThreadState;

bool keepTickProcessingGoing;

spinlock lock{};

World::World() {
    for (int i = 0; i < dimensionList.size(); i++) {
        this->dimensions.push_back( new Dimension(i) );
    }
}

World::~World() {
    keepTickProcessingGoing = false;
    lock.unlock();
    tickProcessingTrhead.join();
    for (int i = 0; i < this->dimensions.size(); i++) {
        delete this->dimensions[i];
    }
}

World * world;

std::chrono::time_point<std::chrono::high_resolution_clock> lastTickDoneTime;
std::chrono::time_point<std::chrono::high_resolution_clock> tickDoneTargetTime;


void processTicksThreadFunction(World * world) {
    
    PyGILState_STATE state;
    while (keepTickProcessingGoing) {
        //lock.unlock();
        state = PyGILState_Ensure();
        tickDoneTargetTime = std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(1000 / tickRate);
        world->processTick();
        lastTickDoneTime = std::chrono::high_resolution_clock::now();
        PyGILState_Release(state);
        lock.lock();
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

    printf("Initializing Player\n");
    player->initalize();
    printf("Multithreading\n");
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
    if (PyCallable_Check(py_onTickCallback)) {
        PyObject_CallObject(py_onTickCallback, NULL);
    }

    player->processTick();

    for (int i = 0; i < this->dimensions.size(); i++) {
        this->dimensions[i]->processTick();
    }
}

PyObject *py_setOnTickCallback(PyObject *self, PyObject *args, PyObject *kwargs) {
    static char *kwlist[] = {(char*)"onTickCallback", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
        "O", kwlist,
        &py_onTickCallback
    )) return NULL;


    return PyBool_FromLong(0);
}
