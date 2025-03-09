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

Block::Block(PyObject* self) {
    py_BlockClass* blockTemplate = (py_BlockClass*) self;
    this->propeties = blockTemplate->instance;
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

BlockInstance::BlockInstance(Dimension* dimension, WorldPos x, WorldPos y, WorldPos z) {
    this->dimension = dimension;
    this->chunk = dimension->findChunk(x, y, z);

    this->x = x;
    this->y = y;
    this->z = z;
}

Block& BlockInstance::get() {
    return this->chunk->getBlock(this->x, this->y, this->z);
}

void BlockInstance::move(BlockFace face) {
    switch(face) {
    case Xpos:
        this->x++;
        break;
    case Ypos:
        this->y++;
        break;
    case Zpos:
        this->z++;
        break;
    case Xneg:
        this->x--;
        break;
    case Yneg:
        this->y--;
        break;
    case Zneg:
        this->z--;
        break;
    }

    if (this->chunk->worldPositionInsideChunk(this->x, this->y, this->z)) return;

    this->chunk = this->dimension->findChunk(this->x, this->y, this->z);
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

    if (newBlock->solid) {
        new Hitbox(newBlock, TYPE_BLOCK, {0, 0, 0}, {1, 1, 1});
    }

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
    )) return -1;

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
