#ifndef ENTITY_H
#define ENTITY_H

class Entity;

#include "dimension.h"
#include <string>

enum TaskType {
    TASK_ENTITY_SET_POSITION,
    TASK_ENTITY_MOVE,
    TASK_ENTITY_SET_DIMENSION
};

struct EntityPosition {
    double x, y, z;
};

class EntityTask {
private:
    TaskType type;
    union {
        EntityPosition position;
        Dimension* dimension;
    } data;
public:
    EntityTask(TaskType type, void * data);

    void exec(Entity* entity);
};

class Entity {
protected:
    Dimension* dimension;
    EntityPosition pos;

    bool initalized;

    std::vector<EntityTask*> tasks;

    virtual void onPositionChanged() {};
private:
    void addTask(EntityTask* task);
    void execTasks();

    bool positionHasChanged;
public:
    void setDimension(Dimension* dimension);
    void setDimension(std::string dimension);

    void setPosition(EntityPosition position);
    void move(EntityPosition movement);

    void draw();
    void processTick();

    void initalize();

    Entity();

    friend class EntityTask;
};

#endif
