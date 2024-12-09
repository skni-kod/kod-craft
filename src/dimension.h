#ifndef DIMENSION_H
#define DIMENSION_H

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

    void draw();
};

class DimensionTemplate {
public:
    int chunkSize;
    std::string name;

    DimensionTemplate(int chunkSize, std::string name);
};

extern std::vector<DimensionTemplate*> dimensionList;

void defineDimension(std::string name, int chunkSize);

#endif
