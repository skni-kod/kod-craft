#ifndef BLOCK_H
#define BLOCK_H

#include <vector>
#include <string>

class BlockTemplate;

class Block {
private:
    BlockTemplate* propeties;
public:
    Block(int id);
    Block(std::string name);

    std::string getName();
    bool isSolid();
};

class BlockTemplate {
public:
    int id;
    std::string name;
    bool solid;

    Block(std::string name);
};

extern std::vector<Block*> blockList;

void defineBlock(std::string name, bool solid = true);

int getDefinedBlockCount();

#endif
