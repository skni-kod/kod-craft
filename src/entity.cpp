#include "entity.h"
#include "world.h"

Entity::Entity() {
    x = 0;
    y = 0;
    z = 0;

    this->dimension = NULL;
    this->initalized = false;
}

void Entity::addTask(EntityTask* task) {
    this->tasks.push_back(task);
}

void Entity::setDimension(Dimension* dimension) {
    this->dimension = dimension;
}

void Entity::setDimension(std::string dimension) {
    Dimension* dimensionFound = world->findDimension(dimension);

    if (dimensionFound==NULL) return;

    this->setDimension(dimensionFound);
}
