#ifndef ENTITY_H
#define ENTITY_H

class Entity;

struct EntityPosition {
    double x, y, z;
};

double length(EntityPosition vector);
EntityPosition normalize(EntityPosition vector);

#include "dimension.h"
#include "hitbox.h"
#include <string>

enum TaskType {
    TASK_ENTITY_SET_POSITION,
    TASK_ENTITY_MOVE,
    TASK_ENTITY_SET_DIMENSION,
    TASK_ENTITY_ADD_VELOCITY
};

class Rotation {
private:
    double pitch, yaw, roll;
public:
    Rotation(double pitch, double yaw, double roll);
    Rotation();

    double getPitch();
    double getYaw();
    double getRoll();

    void setPitch(double pitch);
    void setYaw(double yaw);
    void setRoll(double roll);

    void rotatePitch(double pitch);
    void rotateYaw(double yaw);
    void rotateRoll(double roll);

    void clamp();
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
    EntityPosition vel;

    Rotation* rotation;

    bool initalized;

    std::vector<EntityTask*> tasks;
    std::vector<Hitbox*> hitboxes;

    virtual void onPositionChanged() {};
private:
    void addTask(EntityTask* task);
    void execTasks();

    EntityPosition oldPosition;

    bool positionHasChanged;
public:
    void setDimension(Dimension* dimension);
    void setDimension(std::string dimension);

    void setPosition(EntityPosition position);
    void move(EntityPosition movement);
    void applyFoce(EntityPosition force);

    EntityPosition getInterpPosition();
    Rotation* getRotation();

    void draw();
    void processTick();

    void initalize();

    Entity();
    ~Entity();

    friend class EntityTask;
};

#endif
