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

class EntityTask {
private:
    TaskType type;
    union {
        struct { double x, y, z; } position;
        Dimension* dimension;
    } data;
public:
    EntityTask(TaskType type, void * data);

    void exec(Entity* entity);
};

class Entity {
protected:
    Dimension* dimension;
    double x, y, z;

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

    void draw();
    void processTick();

    void initalize();

    Entity();

    friend class EntityTask;
};

#endif
