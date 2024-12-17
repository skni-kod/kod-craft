#ifndef CHUNK_H
#define CHUNK_H

#include <vector>

class Chunk;

#include<Python.h>

#include "block.h"

#include "dimension.h"

class Chunk {
private:
    Dimension* dimension;
    std::vector<Block> blocks;

    inline Block& getBlock(int x, int y, int z) {
        auto chunkSize = dimension->getChunkSize();
        return this->blocks[(z * chunkSize * chunkSize) + (y * chunkSize) + x];
    }

    int x, y, z;
public:
    Chunk(Dimension* dimension, int x, int y, int z);
};

#endif
