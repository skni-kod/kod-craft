#define USE_PYTHON
#include "entity.h"
#include "world.h"

#include <algorithm>
#include <cmath>

Rotation::Rotation(double pitch, double yaw, double roll) {
    this->pitch = pitch;
    this->yaw = yaw;
    this->roll = roll;
}

Rotation::Rotation() {
    this->pitch = 0;
    this->yaw = 0;
    this->roll = 0;
}

double Rotation::getPitch() {
    return this->pitch;
}

double Rotation::getYaw() {
    return this->yaw;
}

double Rotation::getRoll() {
    return this->roll;
}

EntityPosition Rotation::getVector() {
    EntityPosition vector;

    double yaw = this->getYaw();
    double pitch = this->getPitch();

    vector.x = cos(yaw) * abs(cos(pitch));
    vector.y = sin(yaw) * abs(cos(pitch));
    vector.z = sin(pitch);

    return vector;
}

EntityPosition Rotation::getXYVector() {
    EntityPosition vector;

    double yaw = this->getYaw();

    vector.x = cos(yaw);
    vector.y = sin(yaw);
    vector.z = 0;

    return vector;
}

void Rotation::setPitch(double pitch) {
    this->pitch = pitch;
}

void Rotation::setYaw(double yaw) {
    this->yaw = yaw;
}

void Rotation::setRoll(double roll) {
    this->roll = roll;
}

void Rotation::rotatePitch(double pitch) {
    this->pitch+= pitch;
}

void Rotation::rotateYaw(double yaw) {
    this->yaw+= yaw;
}

void Rotation::rotateRoll(double roll) {
    this->roll+= roll;
}

void Rotation::clamp() {
    this->pitch = fmod(this->pitch, M_PI*2);
    this->yaw = fmod(this->yaw, M_PI*2);
    this->roll = fmod(this->roll, M_PI*2);
    this->pitch = std::clamp(pitch, -M_PI/2+0.0001, M_PI/2-0.0001);
}

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
        if (entity->dimension != NULL) {
            entity->dimension->removeEntity(entity);
        }
        if (this->data.dimension != NULL) {
            this->data.dimension->addEntity(entity);
        }
        entity->dimension = this->data.dimension;
        break;
    case TASK_ENTITY_SET_POSITION:
        entity->pos = this->data.position;
        break;
    case TASK_ENTITY_MOVE:
        entity->pos.x+= this->data.position.x;
        entity->pos.y+= this->data.position.y;
        entity->pos.z+= this->data.position.z;
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

    this->rotation = new Rotation();
}

Entity::~Entity() {
    delete this->rotation;
    for (int i = 0; i < this->tasks.size(); i++) {
        delete this->tasks[i];
    }
    for (int i = 0; i < this->hitboxes.size(); i++) {
        delete this->hitboxes[i];
    }
}

void Entity::checkWorldCollision() {
    EntityPosition collision = noCollision;
 
    // check initial path
    do {
        for (int i = 0; i < this->hitboxes.size(); i++) {
            collision = this->hitboxes[i]->collideWithTerrain();
            if (collision != noCollision) {
                this->pos+=collision;
                this->vel+=collision;
                break;
            }
        }
    } while (collision!=noCollision);


    // check separated axis paths

    EntityPosition originalPosition = this->pos;

    #define checkCollisionAxis(axis) \
    collision = noCollision; \
    this->pos.axis+= this->vel.axis; \
    do { \
        for (int i = 0; i < this->hitboxes.size(); i++) { \
            collision = this->hitboxes[i]->collideWithTerrain(); \
            if (collision.axis != noCollision.axis) { \
                double distance = std::max(std::abs(collision.x), std::max(std::abs(collision.y), std::abs(collision.z))); \
                if (collision.axis<0) distance*=-1;\
                this->pos.axis+=distance; \
                this->vel.axis+=distance; \
                break; \
            } \
        } \
    } while (collision.axis!=noCollision.axis); \
    this->pos = originalPosition;

    checkCollisionAxis(x);
    checkCollisionAxis(y);
    checkCollisionAxis(z);


    // check final path
    do {
        for (int i = 0; i < this->hitboxes.size(); i++) {
            collision = this->hitboxes[i]->collideWithTerrain();
            if (collision != noCollision) {
                this->pos+=collision;
                this->vel+=collision;
                break;
            }
        }
    } while (collision!=noCollision);


}

void Entity::addTask(EntityTask* task) {
    this->tasks.push_back(task);
}

void Entity::execTasks() {
    for (int i = 0; i < this->tasks.size(); i++) {
        this->tasks[i]->exec(this);
        delete this->tasks[i];
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

    fraction = std::clamp(fraction, 0.0f, 1.0f);

    float oldPositionFraction = 1 - fraction;

    EntityPosition interpPos;

    interpPos.x = this->pos.x*fraction + this->oldPosition.x*oldPositionFraction;
    interpPos.y = this->pos.y*fraction + this->oldPosition.y*oldPositionFraction;
    interpPos.z = this->pos.z*fraction + this->oldPosition.z*oldPositionFraction;

    return interpPos;
}

Rotation* Entity::getRotation() {
    return this->rotation;
}

void Entity::processTick() {
    this->oldPosition = this->pos;
    this->pos.x+= this->vel.x;
    this->pos.y+= this->vel.y;
    this->pos.z+= this->vel.z;

    this->execTasks();
    this->checkWorldCollision();

    this->positionHasChanged =!(this->oldPosition == this->pos);

    if (this->positionHasChanged) this->onPositionChanged();
}

void Entity::initalize() {
    this->execTasks();

    if (this->dimension==NULL) return;

    this->initalized = true;
}

double length(EntityPosition vector) {
    return std::hypot(vector.x,vector.y,vector.z);
}

EntityPosition normalize(EntityPosition vector) {
    double positionLength = length(vector);
    if (positionLength<0.0001) return vector;
    vector*=1.0/positionLength;
    return vector;
}

int pyInitEntity(py_EntityClass* self, PyObject* args, PyObject* kwargs) {
    self->instance = new Entity();
    return 0;
}

PyObject * py_onTickCallback;

PyObject *py_setEntityDimension(py_EntityClass* self, PyObject *args, PyObject *kwargs) {
    py_DimensionClass* py_dimension;

    static char *kwlist[] = {(char*)"dimension", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
        "O", kwlist,
        &py_dimension
    )) return NULL;

    self->instance->setDimension(getDimensionInstance(*py_dimension));

    return PyBool_FromLong(0);
}

PyObject *py_setEntityPosition(py_EntityClass* self, PyObject *args, PyObject *kwargs) {
    double x;
    double y;
    double z;

    static char *kwlist[] = {(char*)"x", (char*)"y", (char*)"z", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
        "ddd", kwlist,
        &x, &y, &z
    )) return NULL;

    self->instance->setPosition({x, y, z});

    return PyBool_FromLong(0);
}

PyObject *py_moveEntity(py_EntityClass* self, PyObject *args, PyObject *kwargs) {
    double x;
    double y;
    double z;

    static char *kwlist[] = {(char*)"dX", (char*)"dY", (char*)"dZ", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
        "ddd", kwlist,
        &x, &y, &z
    )) return NULL;

    self->instance->move({x, y, z});

    return PyBool_FromLong(0);
}


PyObject *py_entityApplyForce(py_EntityClass* self, PyObject *args, PyObject *kwargs) {
    double x;
    double y;
    double z;

    static char *kwlist[] = {(char*)"fX", (char*)"fY", (char*)"fZ", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
        "ddd", kwlist,
        &x, &y, &z
    )) return NULL;

    self->instance->applyFoce({x, y, z});

    return PyBool_FromLong(0);
}

PyObject *py_addEntityHitbox(py_EntityClass* self, PyObject *args, PyObject *kwargs) {
    EntityPosition offset;
    EntityPosition size;

    static char *kwlist[] = {(char*)"dX", (char*)"dY", (char*)"dZ", (char*)"sX", (char*)"xY", (char*)"sZ", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
        "dddddd", kwlist,
        &offset.x, &offset.y, &offset.z, &size.x, &size.y, &size.z
    )) return NULL;

    new Hitbox(self->instance, TYPE_ENTITY, offset, size);

    return PyBool_FromLong(0);
}
