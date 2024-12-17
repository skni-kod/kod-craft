#ifndef CHUNK_H
#define CHUNK_H

class Chunk;

#include<Python.h>
#include "block.h"

#include "dimension.h"

class Chunk {
private:
    Dimension* dimension;
    Block* blocks;

    int x, y, z;
public:
    Chunk(Dimension* dimension, int x, int y, int z);
};

#endif
