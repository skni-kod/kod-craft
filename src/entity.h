#ifndef ENTITY_H
#define ENTITY_H

#include "dimension.h"
#include <string>

class Entity {
    Dimension* dimension;
    double x, y, z;

    bool initalized;
public:
    void setDimension(Dimension* dimension);
    void setDimension(std::string dimension);

    Entity();
};

#endif
