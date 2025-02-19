#define USE_PYTHON
#include "dimension.h"

#include <stdexcept>
#include <algorithm>

std::vector<DimensionTemplate*> dimensionList;

Dimension::Dimension(int type) {
    this->propeties = dimensionList[type];
    this->propeties->dimension = this;

    this->chunkSize = this->propeties->chunkSize;
}

Dimension::~Dimension() {
    for (int i = 0; i < this->chunks.size(); i++) {
        delete this->chunks[i];
    }
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

Chunk* Dimension::findChunk(WorldPos x, WorldPos y, WorldPos z) {
    ChunkPos chunkX = this->worldToChunkPos(x);
    ChunkPos chunkY = this->worldToChunkPos(y);
    ChunkPos chunkZ = this->worldToChunkPos(z);

    for (int i = 0; i < this->chunks.size(); i++) {
        Chunk* chunk = this->chunks[i];

        if (chunkX!=chunk->getX()) continue;
        if (chunkY!=chunk->getY()) continue;
        if (chunkZ!=chunk->getZ()) continue;

        return chunk;
    }

    return this->createChunk(chunkX, chunkY, chunkZ);
}

Chunk* Dimension::createChunk(ChunkPos x, ChunkPos y, ChunkPos z) {
    Chunk* chunk = new Chunk(this, x, y, z);

    this->chunks.push_back(chunk);
    return chunk;
}

void Dimension::setBlock(Block block, WorldPos x, WorldPos y, WorldPos z) {
    Chunk* chunk = this->findChunk(x, y, z);
    chunk->setBlock(block, x, y, z);
}

DimensionTemplate::DimensionTemplate(int chunkSize, PyObject * generateChunkCallback, std::string name) {
    this->chunkSize = chunkSize;
    this->name = name;
    this->generateChunkCallback = generateChunkCallback;
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
    this->findChunk(x*chunkSize, y*chunkSize, z*chunkSize);
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

void Dimension::addEntity(Entity * entity) {
    this->entities.push_back(entity);
}

void Dimension::removeEntity(Entity * entity) {
    this->entities.erase(std::find(this->entities.begin(), this->entities.end(), entity));
}

DimensionTemplate * defineDimension(std::string name, PyObject * generateChunkCallback, int chunkSize) {
    DimensionTemplate * definedDimension = new DimensionTemplate(chunkSize, generateChunkCallback, name);
    dimensionList.push_back( definedDimension );
    return definedDimension;
}

void unloadDimensions() {
    for (int i = 0; i < dimensionList.size(); i++) {
        delete dimensionList[i];
    }

    dimensionList.clear();
}


Dimension * getDimensionInstance(py_DimensionClass self) {
    return self.instance->dimension;
}

int *py_defineDimension(py_DimensionClass * self, PyObject *args, PyObject *kwargs) {
    char *name;
    PyObject* generateChunkCallback = NULL;
    int chunkSize = 8;

    static char *kwlist[] = {(char*)"name", (char*)"generateChunkCallback", (char*)"chunkSize", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
        "sO|i", kwlist,
        &name, &generateChunkCallback, &chunkSize
    )) return NULL;


    self->instance = defineDimension(name, generateChunkCallback, chunkSize);

    return 0;
}
