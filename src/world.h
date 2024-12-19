#ifndef WORLD_H
#define WORLD_H

#include "dimension.h"

class World {
private:
    std::vector<Dimension*> dimensions;
public:
    World();
};

void loadWorld();

extern PyObject * onWorldLoadCallback;

#endif
