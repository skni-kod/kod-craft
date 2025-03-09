#include "chunk.h"

void Chunk::draw() {
    if (this->loaded == false) return;

    int chunkSize = this->dimension->getChunkSize();

    for (int x = 0; x < chunkSize; x++)
    for (int y = 0; y < chunkSize; y++)
    for (int z = 0; z < chunkSize; z++) {
        this->getBlock(x, y, z).draw(x + this->x*chunkSize, y + this->y*chunkSize, z + this->z*chunkSize);
    }
}

Chunk::Chunk(Dimension* dimension, ChunkPos x, ChunkPos y, ChunkPos z) {
    this->loaded = false;
    this->dimension = dimension;

    this->x = x;
    this->y = y;
    this->z = z;

    int chunkSize = dimension->getChunkSize();

    PyObject* generateChunkCallback = dimension->getTemplate()->generateChunkCallback;
    PyObject* chunkArray = PyObject_CallObject(generateChunkCallback, Py_BuildValue("(iiii)", x, y, z, chunkSize));

    // allocate memory
    this->blocks.reserve(chunkSize*chunkSize*chunkSize);

    for (int bX = 0; bX < chunkSize; bX++) {
        PyObject* chunkArrayX = PyList_GetItem(chunkArray, bX);

        for (int bY = 0; bY < chunkSize; bY++) {
            PyObject* chunkArrayXY = PyList_GetItem(chunkArrayX, bY);

            for (int bZ = 0; bZ < chunkSize; bZ++) {
                PyObject* chunkBlock = PyList_GetItem(chunkArrayXY, bZ);

                this->getBlock(bX, bY, bZ) = Block(chunkBlock);
            }
        }
    }
    Py_DecRef(chunkArray);
    this->loaded = true;
}

void Chunk::setBlock(Block block, WorldPos x, WorldPos y, WorldPos z) {
    this->getBlock(x, y, z) = block;
}

Block& Chunk::getBlock(WorldPos x, WorldPos y, WorldPos z) {
    auto chunkSize = dimension->getChunkSize();

    x = x%chunkSize;
    y = y%chunkSize;
    z = z%chunkSize;

    if (x<0) x+=chunkSize;
    if (y<0) y+=chunkSize;
    if (z<0) z+=chunkSize;

    return this->blocks[(z * chunkSize * chunkSize) + (y * chunkSize) + x];
}

ChunkPos Chunk::getX() {
    return this->x;
}

ChunkPos Chunk::getY() {
    return this->y;
}

ChunkPos Chunk::getZ() {
    return this->z;
}

void Chunk::processTick() {
}
