#ifndef DIMENSION_H
#define DIMENSION_H

typedef signed long WorldPos;

class Dimension;

#include<vector>
#include<string>

#include "chunk.h"

class DimensionTemplate;

class Dimension {
private:
    std::vector<Chunk*> chunks;
    int chunkSize;
    DimensionTemplate * propeties;

    Chunk* findChunk(WorldPos x, WorldPos y, WorldPos z);
    Chunk* createChunk(ChunkPos x, ChunkPos y, ChunkPos z);
public:
    Dimension(int chunkSize);
    ~Dimension();

    void draw();

    DimensionTemplate* getTemplate();
    int getChunkSize();
    std::string getName();

    void setBlock(Block block, WorldPos x, WorldPos y, WorldPos z);

    ChunkPos worldToChunkPos(WorldPos pos);

    void loadChunk(ChunkPos x, ChunkPos y, ChunkPos z);
    void loadAroundPosition(WorldPos x, WorldPos y, WorldPos z, int distance);
    void processTick();
};

class DimensionTemplate {
public:
    int chunkSize;
    std::string name;

    PyObject * generateChunkCallback;

    DimensionTemplate(int chunkSize, PyObject * generateChunkCallback, std::string name);
};

extern std::vector<DimensionTemplate*> dimensionList;

void defineDimension(std::string name, PyObject * generateChunkCallback, int chunkSize);

#endif
