#ifndef BLOCK_H
#define BLOCK_H

#include <vector>
#include <string>

class Block {
private:
    int id;
    std::string name;
    bool solid;
public:
    Block(std::string name, bool solid);
    Block(int id);

    std::string getName();
    bool isSolid();
};

extern std::vector<Block*> blockList;

void defineBlock(std::string name, bool solid = true);

int getBlockCount();

#endif
