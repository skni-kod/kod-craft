#include "entity.h"
#include "world.h"

EntityTask::EntityTask(TaskType type, void * data) {
    this->type = type;

    switch(type) {
    case TASK_ENTITY_SET_DIMENSION:
        this->data.dimension = (Dimension*)data;
        break;
    case TASK_ENTITY_SET_POSITION:
    case TASK_ENTITY_MOVE:
        this->data.position = *(typeof(EntityTask::data.position)*)data;
    }
}

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
    EntityTask* task = new EntityTask(TASK_ENTITY_SET_DIMENSION, dimension);
    this->addTask(task);
}

void Entity::setDimension(std::string dimension) {
    Dimension* dimensionFound = world->findDimension(dimension);

    if (dimensionFound==NULL) return;

    EntityTask* task = new EntityTask(TASK_ENTITY_SET_DIMENSION, dimensionFound);
    this->addTask(task);
}

void Entity::initalize() {
    if (this->dimension==NULL) return;

    this->initalized = true;
}
