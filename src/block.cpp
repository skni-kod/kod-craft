#define USE_PYTHON

#include "block.h"
#include "raylib.h"
#include <mutex>

std::vector<BlockTemplate*> blockList;
std::mutex blockListLock{};

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

EntityPosition Block::checkCollision(EntityPosition position, EntityPosition velocity, Hitbox* hitbox, WorldPos x, WorldPos y, WorldPos z) {
    for (int i = 0; i < this->propeties->hitboxes.size(); i++) {
        EntityPosition delta = hitbox->collideWithBlock(position, velocity, this->propeties->hitboxes[i], x, y, z);
        if (delta == noCollision) continue;
        return delta;
    }

    return noCollision;
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

WorldPos BlockInstance::getX() {
    return this->x;
}

WorldPos BlockInstance::getY() {
    return this->y;
}

WorldPos BlockInstance::getZ() {
    return this->z;
}

BlockInstance BlockInstance::getInstanceAt(BlockFace face) {
    BlockInstance newInstance = *this;

    switch(face) {
    case Xpos:
        newInstance.x++;
        break;
    case Ypos:
        newInstance.y++;
        break;
    case Zpos:
        newInstance.z++;
        break;
    case Xneg:
        newInstance.x--;
        break;
    case Yneg:
        newInstance.y--;
        break;
    case Zneg:
        newInstance.z--;
        break;
    }

    if (newInstance.chunk->worldPositionInsideChunk(newInstance.x, newInstance.y, newInstance.z)) return newInstance;

    newInstance.chunk = newInstance.dimension->findChunk(newInstance.x, newInstance.y, newInstance.z);

    return newInstance;
}

BlockTemplate::BlockTemplate(std::string name) {
    this->name = name;
}

BlockTemplate::~BlockTemplate() {
    for (int i = 0; i < this->hitboxes.size(); i++) {
        delete this->hitboxes[i];
    }
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
    std::lock_guard<std::mutex> lock{ blockListLock };
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
