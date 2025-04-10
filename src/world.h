#ifndef WORLD_H
#define WORLD_H

const int tickRate = 25; // 25 tps

#include "dimension.h"
#include <thread>
#include <chrono>

class World {
private:
    std::vector<Dimension*> dimensions;
    std::thread tickProcessingTrhead;
public:
    World();
    ~World();

    Dimension* findDimension(std::string name);

    void startTickProcessing();
    void processTick();
};

void loadWorld();
void unloadWorld();

extern std::chrono::time_point<std::chrono::high_resolution_clock> lastTickDoneTime;
extern std::chrono::time_point<std::chrono::high_resolution_clock> tickDoneTargetTime;

extern PyObject * onWorldLoadCallback;
extern PyObject* py_onTickCallback;
extern World * world;

#ifdef PYTHON_DEFINTION
#define USE_PYTHON
#endif

#ifdef PYTHON_DEFINTION
PyObject *py_setOnTickCallback(PyObject *self, PyObject *args, PyObject *kwargs);
#endif
#endif
