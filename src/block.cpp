#include "block.h"

std::vector<BlockTemplate*> blockList;

BlockTemplate* findBlockTemplate(std::string name) {
    for (int i = 0; i < blockList.size(); i++) {
        if (blockList[i]->name == name) return blockList[i];
    }
    return nullptr;
}

Block::Block(std::string name) {
    this->propeties = findBlockTemplate(name);
}

Block::Block(int id) {
    this->propeties = blockList[id];
}


std::string Block::getName() {
    return this->propeties->name;
}

bool Block::isSolid() {
    return this->propeties->solid;
}

BlockTemplate::BlockTemplate(std::string name) {
    this->name = name;
}

void defineBlock(std::string name, bool solid) {
    BlockTemplate* newBlock = new BlockTemplate(name);

    newBlock->solid = solid;

    blockList.push_back(newBlock);
}

int getDefinedBlockCount() {
    return blockList.size();
}
