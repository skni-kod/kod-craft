#ifndef CHUNK_H
#define CHUNK_H


typedef signed int ChunkPos;

class Chunk;

#include<Python.h>
#include <vector>

#include "block.h"

#include "dimension.h"

class Chunk {
private:
    Dimension* dimension;
    std::vector<Block> blocks;

    inline Block& getBlock(WorldPos x, WorldPos y, WorldPos z);

    ChunkPos x, y, z;
public:
    Chunk(Dimension* dimension, ChunkPos x, ChunkPos y, ChunkPos z);

    void draw();

    ChunkPos getX();
    ChunkPos getY();
    ChunkPos getZ();

    void setBlock(Block block, WorldPos x, WorldPos y, WorldPos z);

    void processTick();
};

#endif
