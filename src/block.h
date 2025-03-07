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
    Block(PyObject* self);

    std::string getName();
    bool isSolid();
    bool isVisible();

    void draw(WorldPos x, WorldPos y, WorldPos z);
};

class BlockTemplate {
public:
    int id;
    std::string name;
    struct {
        bool solid : 1;
        bool visible : 1;
    };

    std::vector<Hitbox*> hitboxes;

    Texture2D texture;

    explicit BlockTemplate(std::string name);

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
