#include "entity.h"

void Entity::setDimension(Dimension* dimension) {
    this->dimension = dimension;
}

void Entity::setDimension(std::string dimension) {
    Dimension* dimensionFound = world->findDimension(dimension);

    if (dimensionFound==NULL) return;

    this->setDimension(dimensionFound);
}
