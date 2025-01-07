#ifndef WORLD_H
#define WORLD_H

const int tickRate = 25; // 25 tps

#include "dimension.h"
#include <thread>

class World {
private:
    std::vector<Dimension*> dimensions;
    std::thread tickProcessingTrhead;
public:
    World();

    Dimension* findDimension(std::string name);

    void startTickProcessing();
    void processTick();
};

void loadWorld();

extern PyObject * onWorldLoadCallback;
extern World * world;

#endif
