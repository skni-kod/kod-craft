#include "block.h"

int nextID = 0;

std::vector<BlockTemplate*> blockList;

Block::Block(std::string name, bool solid) {
    this->id = nextID++;
    this->name = name;
    this->solid = solid;
}

Block::Block(int id) {
    this->id = id;

    Block *refrenceBlock = blockList[id];

    this->name = refrenceBlock->getName();
    this->solid = refrenceBlock->isSolid();
}


std::string Block::getName() {
    return this->name;
}

bool Block::isSolid() {
    return this->solid;
}

void defineBlock(std::string name, bool solid) {
    blockList.push_back(new Block(name, solid));
}

int getDefinedBlockCount() {
    return nextID;
}
