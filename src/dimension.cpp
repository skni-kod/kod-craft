#include "dimension.h"

#include <stdexcept>

std::vector<DimensionTemplate*> dimensionList;

Dimension::Dimension(int type) {
    this->propeties = dimensionList[type];

    this->chunkSize = this->propeties->chunkSize;
}

int Dimension::getChunkSize() {
    return this->chunkSize;
}

std::string Dimension::getName() {
    return this->propeties->name;
}

ChunkPos Dimension::worldToChunkPos(WorldPos pos) {
    int chunkSize = this->chunkSize;

    if (pos<0 && !(pos % chunkSize == 0)) pos-=chunkSize;
    pos/=chunkSize;

    return pos;
}

DimensionTemplate* Dimension::getTemplate() {
    return this->propeties;
}

chunk_ptr Dimension::findChunk(WorldPos x, WorldPos y, WorldPos z) {
    ChunkPos chunkX = this->worldToChunkPos(x);
    ChunkPos chunkY = this->worldToChunkPos(y);
    ChunkPos chunkZ = this->worldToChunkPos(z);

    for (int i = 0; i < this->chunks.size(); i++) {
        weak_chunk tmp = this->chunks[i];
        if (tmp.expired()) throw std::runtime_error("Chunk pointer expired");
        chunk_ptr chunk = tmp.lock();

        if (chunkX!=chunk->getX()) continue;
        if (chunkY!=chunk->getY()) continue;
        if (chunkZ!=chunk->getZ()) continue;

        return chunk;
    }

    return this->createChunk(chunkX, chunkY, chunkZ);
}

chunk_ptr Dimension::createChunk(ChunkPos x, ChunkPos y, ChunkPos z) {
    chunk_ptr chunk = std::make_shared<Chunk>(this, x, y, z);

    this->chunks.push_back(chunk);
    return chunk;
}

void Dimension::setBlock(Block block, WorldPos x, WorldPos y, WorldPos z) {
    chunk_ptr chunk = this->findChunk(x, y, z);
    chunk->setBlock(block, x, y, z);
}

DimensionTemplate::DimensionTemplate(int chunkSize, PyObject * generateChunkCallback, std::string name) {
    this->chunkSize = chunkSize;
    this->name = name;
    this->generateChunkCallback = generateChunkCallback;

    dimensionList.push_back(this);
}

void Dimension::draw() {
    for (int i = 0; i < chunks.size(); i++) {
        chunks[i]->draw();
    }
}

void Dimension::processTick() {
    for (int i = 0; i < this->chunks.size(); i++) {
        this->chunks[i]->processTick();
    }
}

void Dimension::loadChunk(ChunkPos x, ChunkPos y, ChunkPos z) {
    this->findChunk(x, y, z);
}

void Dimension::loadAroundPosition(WorldPos x, WorldPos y, WorldPos z, int distance) {
    x = this->worldToChunkPos(x);
    y = this->worldToChunkPos(y);
    z = this->worldToChunkPos(z);

    for (int dX = -distance; dX<=distance; dX++)
    for (int dY = -distance; dY<=distance; dY++)
    for (int dZ = -distance; dZ<=distance; dZ++) {
        this->loadChunk(x + dX, y + dY, z + dZ);
    }
}

void defineDimension(std::string name, PyObject * generateChunkCallback, int chunkSize) {
    dimensionList.push_back( new DimensionTemplate(chunkSize, generateChunkCallback, name) );
}
