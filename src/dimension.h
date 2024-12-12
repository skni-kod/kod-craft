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

    DimensionTemplate* getTemplate();
    int getChunkSize();

    ChunkPos worldToChunkPos(WorldPos pos);
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
