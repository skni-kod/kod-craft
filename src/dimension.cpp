#include "dimension.h"

std::vector<DimensionTemplate*> dimensionList;

Dimension::Dimension(int type) {
    this->propeties = dimensionList[type];

    this->chunkSize = this->propeties->chunkSize;
}

DimensionTemplate::DimensionTemplate(int chunkSize, std::string name) {
    this->chunkSize = chunkSize;
    this->name = name;

    dimensionList.push_back(this);
}

