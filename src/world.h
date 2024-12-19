#ifndef WORLD_H
#define WORLD_H

#include "dimension.h"

class World {
private:
    std::vector<Dimension*> dimensions;
public:
    World();

    Dimension* findDimension(std::string name);
};

void loadWorld();

extern PyObject * onWorldLoadCallback;
extern World * world;

#endif
