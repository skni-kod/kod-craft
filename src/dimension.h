#ifndef DIMENSION_H
#define DIMENSION_H

typedef signed long WorldPos;

class Dimension;

#include<vector>
#include<string>
#include <memory>

#include "chunk.h"

class DimensionTemplate;

using chunk_ptr = std::shared_ptr<Chunk>;
using weak_chunk = std::weak_ptr<Chunk>;

class Dimension {
private:
    
    std::vector < chunk_ptr > chunks;
    int chunkSize;
    DimensionTemplate * propeties;

    chunk_ptr findChunk(WorldPos x, WorldPos y, WorldPos z);
    chunk_ptr createChunk(ChunkPos x, ChunkPos y, ChunkPos z);
public:
    Dimension(int chunkSize);

    void draw();

    DimensionTemplate* getTemplate();
    int getChunkSize();

    void setBlock(Block block, WorldPos x, WorldPos y, WorldPos z);

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
