
#include "chunk.h"

Chunk::Chunk(Dimension* parent, int x, int y, int z) {
    this->parent = parent;

    this->x = x;
    this->y = y;
    this->z = z;

    int chunkSize = parent.getChunkSize();


    // allocate memory
    this->blocks = (Block***) malloc(chunkSize * sizeof(Block**));

    for (int bX = 0; bX < chunkSize; bX++) {
        this->blocks[bX] = (Block**) malloc(chunkSize * sizeof(Block*));
        for (int bY = 0; bY < chunkSize; bY++) {
            this->blocks[bY] = (Block*) malloc(chunkSize * sizeof(Block));

            for (int bY = 0; bY < chunkSize; bY++) {
                
            }
        }
    }
}