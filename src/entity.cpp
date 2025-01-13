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
    case TASK_ENTITY_ADD_VELOCITY:
        this->data.position = *(EntityPosition*)data;
        break;
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
    case TASK_ENTITY_ADD_VELOCITY:
        entity->vel.x+= this->data.position.x;
        entity->vel.y+= this->data.position.y;
        entity->vel.z+= this->data.position.z;
    }
}

Entity::Entity() {
    this->pos.x = 0;
    this->pos.y = 0;
    this->pos.z = 0;

    this->vel.x = 0;
    this->vel.y = 0;
    this->vel.z = 0;

    this->dimension = NULL;
    this->initalized = false;

    this->positionHasChanged = true;
}

void Entity::addTask(EntityTask* task) {
    this->tasks.push_back(task);
}

void Entity::execTasks() {
    this->oldPosition = this->pos;

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

void Entity::setPosition(EntityPosition position) {
    EntityTask* task = new EntityTask(TASK_ENTITY_SET_POSITION, &position);
    this->addTask(task);
}

void Entity::move(EntityPosition movement) {
    EntityTask* task = new EntityTask(TASK_ENTITY_MOVE, &movement);
    this->addTask(task);
}

void Entity::applyFoce(EntityPosition force) {
    float weight = 1;

    force.x/=weight;
    force.y/=weight;
    force.z/=weight;

    EntityTask* task = new EntityTask(TASK_ENTITY_ADD_VELOCITY, &force);
    this->addTask(task);
}

EntityPosition Entity::getInterpPosition() {
    std::chrono::duration<double> timeSinceLastTick = std::chrono::high_resolution_clock::now() - lastTickDoneTime;
    std::chrono::duration<double> timeUntillNextTick = tickDoneTargetTime - lastTickDoneTime;

    float fraction = timeSinceLastTick/timeUntillNextTick;
    float oldPositionFraction = 1 - fraction;

    EntityPosition interpPos;

    interpPos.x = this->pos.x*fraction + this->oldPosition.x*oldPositionFraction;
    interpPos.y = this->pos.y*fraction + this->oldPosition.y*oldPositionFraction;
    interpPos.z = this->pos.z*fraction + this->oldPosition.z*oldPositionFraction;

    return interpPos;
}

void Entity::processTick() {
    this->pos.x+= this->vel.x;
    this->pos.y+= this->vel.y;
    this->pos.z+= this->vel.z;

    this->execTasks();

    if (this->positionHasChanged) this->onPositionChanged();

    this->positionHasChanged = false;
}

void Entity::initalize() {
    this->execTasks();

    if (this->dimension==NULL) return;

    this->initalized = true;
}
