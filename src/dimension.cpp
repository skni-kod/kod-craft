#include "dimension.h"

std::vector<DimensionTemplate*> dimensionList;

Dimension::Dimension(int type) {
    this->propeties = dimensionList[type];

    this->chunkSize = this->propeties->chunkSize;
}

int Dimension::getChunkSize() {
    return this->chunkSize;
}

DimensionTemplate::DimensionTemplate(int chunkSize, PyObject * generateChunkCallback, std::string name) {
    this->chunkSize = chunkSize;
    this->name = name;
    this->generateChunkCallback = generateChunkCallback;

    dimensionList.push_back(this);
}

void defineDimension(std::string name, PyObject * generateChunkCallback, int chunkSize) {
    dimensionList.push_back( new DimensionTemplate(chunkSize, generateChunkCallback, name) );
}
