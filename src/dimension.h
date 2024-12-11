#ifndef DIMENSION_H
#define DIMENSION_H

class Dimension;

#include<vector>
#include<string>

#include "chunk.h"

class DimensionTemplate;

class Dimension {
private:
    std::vector<Chunk*> chunks;
    int chunkSize;
    DimensionTemplate * propeties;
public:
    Dimension(int chunkSize);

    DimensionTemplate* getTemplate();
    int getChunkSize();

    int worldToChunkPos(int pos);
};

class DimensionTemplate {
public:
    int chunkSize;
    std::string name;

    PyObject * generateChunkCallback;

    DimensionTemplate(int chunkSize, PyObject * generateChunkCallback, std::string name);
};

extern std::vector<DimensionTemplate*> dimensionList;

void defineDimension(std::string name, PyObject * generateChunkCallback, int chunkSize);

#endif
