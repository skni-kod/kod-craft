#define USE_PYTHON

#include "block.h"
#include "raylib.h"

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

void Block::draw(WorldPos x, WorldPos y, WorldPos z) {
    if (this->propeties->visible == false) return;
    
    DrawCube({(float)(x), (float)(y), (float)(z)}, 1, 1, 1, GOLD);
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

void BlockTemplate::setTexture(std::string fileName) {
    std::string filePath = "./data/textures/blocks/"+fileName;
    this->texture = LoadTexture(filePath.c_str());
}

BlockTemplate * defineBlock(std::string name, bool solid, bool visible) {
    BlockTemplate* newBlock = new BlockTemplate(name);

    newBlock->solid = solid;
    newBlock->visible = visible;

    blockList.push_back(newBlock);

    return newBlock;
}

int getDefinedBlockCount() {
    return blockList.size();
}

void unloadBlocks() {
    for (int i = 0; i < blockList.size(); i++) {
        delete blockList[i];
    }

    blockList.clear();
}

int pyInitBlock(py_BlockClass* self, PyObject* args, PyObject* kwargs) {
    char *name;
    int solid = 1;
    int visible = 1;
    char *texture = NULL;

    static char *kwlist[] = {(char*)"name", (char*)"texture", (char*)"solid", (char*)"visible", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
        "s|spp", kwlist,
        &name, &texture, &solid, &visible
    )) return NULL;

    BlockTemplate * createdBlock = defineBlock(name, solid, visible);
    
    if (texture) {
        createdBlock->setTexture(texture);
    }

    self->instance = createdBlock;

    return 0;
}

int pyInitBlockInstance(py_BlockInstanceClass* self, PyObject* args, PyObject* kwargs) {
    // don't allow BlockInstance creation directly in python
    return -1;
}
