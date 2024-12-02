#include "block.h"

std::vector<Block*> blockList;

void defineBlock(Block* block) {
    blockList.push_back(block);
}
