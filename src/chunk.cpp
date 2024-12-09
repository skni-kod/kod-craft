
#include "chunk.h"

Chunk::Chunk(Dimension* dimension, int x, int y, int z) {
    this->dimension = dimension;

    this->x = x;
    this->y = y;
    this->z = z;

    int chunkSize = dimension->getChunkSize();


    // allocate memory
    this->blocks = (Block***) malloc(chunkSize * sizeof(Block**));

    for (int bX = 0; bX < chunkSize; bX++) {
        this->blocks[bX] = (Block**) malloc(chunkSize * sizeof(Block*));
        for (int bY = 0; bY < chunkSize; bY++) {
            this->blocks[bX][bY] = (Block*) malloc(chunkSize * sizeof(Block));

            for (int bY = 0; bY < chunkSize; bY++) {
                
            }
        }
    }
}