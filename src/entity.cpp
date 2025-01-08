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
        this->data.position = *(EntityPosition*)data;
    }
}

void EntityTask::exec(Entity* entity) {
    switch(this->type) {
    case TASK_ENTITY_SET_DIMENSION:
        entity->dimension = this->data.dimension;
        break;
    case TASK_ENTITY_SET_POSITION:
        entity->pos = this->data.position;

        entity->positionHasChanged = true;
        break;
    case TASK_ENTITY_MOVE:
        entity->pos.x+= this->data.position.x;
        entity->pos.y+= this->data.position.y;
        entity->pos.z+= this->data.position.z;

        entity->positionHasChanged = true;
        break;
    }
}

Entity::Entity() {
    this->pos.x = 0;
    this->pos.y = 0;
    this->pos.z = 0;

    this->dimension = NULL;
    this->initalized = false;

    this->positionHasChanged = true;
}

void Entity::addTask(EntityTask* task) {
    this->tasks.push_back(task);
}

void Entity::execTasks() {
    for (int i = 0; i < this->tasks.size(); i++) {
        this->tasks[i]->exec(this);
    }
    this->tasks.clear();
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

void Entity::processTick() {
    this->execTasks();

    if (this->positionHasChanged) this->onPositionChanged();

    this->positionHasChanged = false;
}

void Entity::initalize() {
    this->execTasks();

    if (this->dimension==NULL) return;

    this->initalized = true;
}
