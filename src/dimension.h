#ifndef DIMENSION_H
#define DIMENSION_H

typedef signed long WorldPos;

class Dimension;

#include<vector>
#include<string>

#include "chunk.h"
#include "entity.h"

class DimensionTemplate;

class Dimension {
private:
    std::vector<Entity*> entities;
    std::vector<Chunk*> chunks;
    int chunkSize;
    DimensionTemplate * propeties;

    Chunk* createChunk(ChunkPos x, ChunkPos y, ChunkPos z);
public:
    Dimension(int chunkSize);
    ~Dimension();

    void draw();

    DimensionTemplate* getTemplate();
    int getChunkSize();
    std::string getName();

    Chunk* findChunk(WorldPos x, WorldPos y, WorldPos z);

    void setBlock(Block block, WorldPos x, WorldPos y, WorldPos z);
    Block& getBlock(WorldPos x, WorldPos y, WorldPos z);

    ChunkPos worldToChunkPos(WorldPos pos);

    void loadChunk(ChunkPos x, ChunkPos y, ChunkPos z);
    void loadAroundPosition(WorldPos x, WorldPos y, WorldPos z, int distance);
    void processTick();

    void addEntity(Entity * entity);
    void removeEntity(Entity * entity);
};

class DimensionTemplate {
public:
    int chunkSize;
    std::string name;

    Dimension * dimension;

    PyObject * generateChunkCallback;

    DimensionTemplate(int chunkSize, PyObject * generateChunkCallback, std::string name);
};

extern std::vector<DimensionTemplate*> dimensionList;

DimensionTemplate * defineDimension(std::string name, PyObject * generateChunkCallback, int chunkSize);
void unloadDimensions();

#ifdef PYTHON_DEFINTION
#define USE_PYTHON
#endif

#ifdef USE_PYTHON

struct py_DimensionClass {
    PyObject_HEAD
    DimensionTemplate* instance;
};

Dimension * getDimensionInstance(py_DimensionClass self);


#endif

#ifdef PYTHON_DEFINTION

PyTypeObject py_DimensionClassType;
int *py_defineDimension(py_DimensionClass * self, PyObject *args, PyObject *kwargs);

#endif
#endif
