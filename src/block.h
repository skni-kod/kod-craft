#ifndef BLOCK_H
#define BLOCK_H

#include <vector>

class Block {
private:
    int id;
public:
    Block();
};

extern std::vector<Block*> blockList;

#endif
