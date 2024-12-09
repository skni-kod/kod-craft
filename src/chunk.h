#ifndef CHUNK_H
#define CHUNK_H

#include<Python.h>
#include "block.h"

class Dimension;

class Chunk {
private:
    Dimension* parent;
    Block*** blocks;

    int x, y, z;
public:
    Chunk(Dimension* parent, int x, int y, int z);
};

#endif
