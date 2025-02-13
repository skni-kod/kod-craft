#ifndef BLOCK_H
#define BLOCK_H

#include <vector>
#include <string>

class BlockTemplate;
class Block;

#include "dimension.h"
#include "hitbox.h"
#include "raylib.h"

class Block {
private:
    BlockTemplate* propeties;
public:
    Block(int id);
    Block(std::string name);

    std::string getName();
    bool isSolid();

    void draw(WorldPos x, WorldPos y, WorldPos z);
};

class BlockTemplate {
public:
    int id;
    std::string name;
    bool solid, visible;

    std::vector<Hitbox*> hitboxes;

    Texture2D texture;

    BlockTemplate(std::string name);

    void setTexture(std::string fileName);
};

extern std::vector<BlockTemplate*> blockList;

BlockTemplate * defineBlock(std::string name, bool solid = true, bool visible = true);

int getDefinedBlockCount();
void unloadBlocks();

#endif
