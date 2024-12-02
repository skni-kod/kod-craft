#ifndef DIMENSION_H
#define DIMENSION_H

#include<vector>

#include "chunk.h"

class Dimension {
private:
    std::vector<Chunk*> chunks;
    int chunkSize;
public:
    Dimension(int chunkSize);
}

#endif
