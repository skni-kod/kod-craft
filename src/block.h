#ifndef BLOCK_H
#define BLOCK_H

#include <vector>
#include <string>

class BlockTemplate;
class BlockInstance;
class Block;

enum BlockFace {
    Xpos, Ypos, Zpos,
    Xneg, Yneg, Zneg
};

#include "dimension.h"
#include "hitbox.h"
#include "raylib.h"

class Block {
private:
    BlockTemplate* propeties;
public:
    Block(int id);
    Block(std::string name);
    Block(PyObject* self);

    std::string getName();
    bool isSolid();

    EntityPosition checkCollision(Hitbox* hitbox, WorldPos x, WorldPos y, WorldPos z);

    void draw(WorldPos x, WorldPos y, WorldPos z);
};

class BlockInstance {
private:
    WorldPos x;
    WorldPos y;
    WorldPos z;
    Chunk* chunk;
    Dimension* dimension;
public:
    BlockInstance(Dimension* dimension, WorldPos x, WorldPos y, WorldPos z);

    Block& get();
    WorldPos getX();
    WorldPos getY();
    WorldPos getZ();

    BlockInstance getInstanceAt(BlockFace face);
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


#ifdef PYTHON_DEFINTION
#define USE_PYTHON
#endif

#ifdef USE_PYTHON

struct py_BlockClass {
    PyObject_HEAD
    BlockTemplate* instance;
};

struct py_BlockInstanceClass {
    PyObject_HEAD
    Dimension * dimension;
    WorldPos x;
    WorldPos y;
    WorldPos z;
};


#endif

#ifdef PYTHON_DEFINTION

PyTypeObject py_BlockClassType;
PyTypeObject py_BlockInstanceClassType;

int pyInitBlock(py_BlockClass* self, PyObject* args, PyObject* kwargs);
int pyInitBlockInstance(py_BlockInstanceClass* self, PyObject* args, PyObject* kwargs);

#endif
#endif
