#include "block.h"

std::vector<Block*> blockList;

void defineBlock(Block* block) {
    blockList.push_back(block);

std::string Block::getName() {
    return this->name;
}

bool Block::isSolid() {
    return this->solid;
}
}
