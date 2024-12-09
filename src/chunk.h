#ifndef CHUNK_H
#define CHUNK_H

#include<Python.h>
#include "block.h"

class Dimension;

class Chunk {
private:
    Dimension* parent;
    Block*** blocks;
public:
    Chunk(int size);
};

#endif
