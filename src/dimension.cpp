#include "dimension.h"

std::vector<DimensionTemplate*> dimensionList;

DimensionTemplate::DimensionTemplate(int chunkSize, std::string name) {
    this->chunkSize = chunkSize;
    this->name = name;

    dimensionList.push_back(this);
}

