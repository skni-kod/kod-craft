#ifndef DIMENSION_H
#define DIMENSION_H

#include<vector>
#include<string>

#include "chunk.h"

class Dimension {
private:
    std::vector<Chunk*> chunks;
    int chunkSize;
    DimensionTemplate * propeties;
public:
    Dimension(int chunkSize);
};

class DimensionTemplate {
public:
    int chunkSize;
    std::string name;
};

extern std::vector<DimensionTemplate*> dimensionList;

#endif
