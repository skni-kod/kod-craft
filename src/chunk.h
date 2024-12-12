#ifndef CHUNK_H
#define CHUNK_H

typedef signed int ChunkPos;

class Chunk;

#include<Python.h>
#include "block.h"

#include "dimension.h"

class Chunk {
private:
    Dimension* dimension;
    Block*** blocks;

    ChunkPos x, y, z;
public:
    Chunk(Dimension* dimension, ChunkPos x, ChunkPos y, ChunkPos z);

    ChunkPos getX();
    ChunkPos getY();
    ChunkPos getZ();

    void setBlock(Block block, WorldPos x, WorldPos y, WorldPos z);
};

#endif
