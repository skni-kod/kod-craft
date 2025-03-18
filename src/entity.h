#ifndef ENTITY_H
#define ENTITY_H

class Entity;

struct EntityPosition {
    double x, y, z;

    EntityPosition& operator+=(const EntityPosition& other) {
        this->x+=other.x;
        this->y+=other.y;
        this->z+=other.z;
        return *this;
    }

    EntityPosition& operator-=(const EntityPosition& other) {
        this->x-=other.x;
        this->y-=other.y;
        this->z-=other.z;
        return *this;
    }

    EntityPosition& operator-=(const double scalar) {
        this->x-=scalar;
        this->y-=scalar;
        this->z-=scalar;
        return *this;
    }

    EntityPosition& operator+=(const double scalar) {
        this->x+=scalar;
        this->y+=scalar;
        this->z+=scalar;
        return *this;
    }

    EntityPosition& operator*=(const double scalar) {
        this->x*=scalar;
        this->y*=scalar;
        this->z*=scalar;
        return *this;
    }

    bool operator==(const EntityPosition& other) {
        if (this->x != other.x) return false;
        if (this->y != other.y) return false;
        if (this->z != other.z) return false;
        return true;
    }

    bool operator!=(const EntityPosition& other) {
        return !(*this==other);
    }

    EntityPosition operator+(EntityPosition other) {
        other+=*this;
        return other;
    }

    EntityPosition operator-(EntityPosition other) {
        other-=*this;
        other*=-1;
        return other;
    }

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

    EntityPosition getVector();
    EntityPosition getXYVector();

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

    void checkWorldCollision();

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
    friend class Hitbox;
};

#ifdef PYTHON_DEFINTION
#define USE_PYTHON
#endif

#ifdef USE_PYTHON

struct py_EntityClass {
    PyObject_HEAD
    Entity* instance;
};

#endif

#ifdef PYTHON_DEFINTION

PyTypeObject py_EntityClassType;

int pyInitEntity(py_EntityClass* self, PyObject* args, PyObject* kwargs);

PyObject *py_setEntityDimension(py_EntityClass* self, PyObject *args, PyObject *kwargs);
PyObject *py_setEntityPosition(py_EntityClass* self, PyObject *args, PyObject *kwargs);
PyObject *py_moveEntity(py_EntityClass* self, PyObject *args, PyObject *kwargs);
PyObject *py_entityApplyForce(py_EntityClass* self, PyObject *args, PyObject *kwargs);
PyObject *py_addEntityHitbox(py_EntityClass* self, PyObject *args, PyObject *kwargs);

static PyMethodDef pyMethodsEntity[] = {
    {"setDimension", (PyCFunction)py_setEntityDimension, METH_VARARGS | METH_KEYWORDS,
     "Set object's dimension."},
     {"setPosition", (PyCFunction)py_setEntityPosition, METH_VARARGS | METH_KEYWORDS,
     "Set object's position directly."},
     {"move", (PyCFunction)py_moveEntity, METH_VARARGS | METH_KEYWORDS,
     "Change object's position by a delta."},
     {"applyForce", (PyCFunction)py_entityApplyForce, METH_VARARGS | METH_KEYWORDS,
     "Change object's velocity by a delta."},
     {"addHitbox", (PyCFunction)py_addEntityHitbox, METH_VARARGS | METH_KEYWORDS,
     "Add a hitbox to an entity."},
    {NULL, NULL, 0, NULL}
};

#endif
#endif
