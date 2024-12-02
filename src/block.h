#ifndef BLOCK_H
#define BLOCK_H

#include <vector>

class Block {
private:
    int id;
    std::string name;
    bool solid;
public:
    Block();

    std::string getName();
    bool isSolid();
};

extern std::vector<Block*> blockList;

#endif
