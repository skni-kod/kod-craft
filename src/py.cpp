#define PY_SSIZE_T_CLEAN
#include<Python.h>

#include "block.h"
#include "dimension.h"
#include "world.h"
#include "player.h"

bool isPythonInitalized = false;


static PyObject *py_defineBlock(PyObject *self, PyObject *args, PyObject *kwargs) {
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

    return PyBool_FromLong(0);
}

static PyObject *py_defineDimension(PyObject *self, PyObject *args, PyObject *kwargs) {
    char *name;
    PyObject* generateChunkCallback = NULL;
    int chunkSize = 8;

    static char *kwlist[] = {(char*)"name", (char*)"generateChunkCallback", (char*)"chunkSize", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
        "sO|i", kwlist,
        &name, &generateChunkCallback, &chunkSize
    )) return NULL;


    defineDimension(name, generateChunkCallback, chunkSize);

    return PyBool_FromLong(0);
}

static PyObject *py_setPlayerDimension(PyObject *self, PyObject *args, PyObject *kwargs) {
    char *dimensionName;

    static char *kwlist[] = {(char*)"dimensionName", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
        "s", kwlist,
        &dimensionName
    )) return NULL;

    player->setDimension(dimensionName);

    return PyBool_FromLong(0);
}

static PyObject *py_setPlayerPosition(PyObject *self, PyObject *args, PyObject *kwargs) {
    double x;
    double y;
    double z;

    static char *kwlist[] = {(char*)"new Player X position", (char*)"new Player Y position", (char*)"new Player Z position", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
        "ddd", kwlist,
        &x, &y, &z
    )) return NULL;

    player->setPosition({x, y, z});

    return PyBool_FromLong(0);
}

static PyObject *py_movePlayer(PyObject *self, PyObject *args, PyObject *kwargs) {
    double x;
    double y;
    double z;

    static char *kwlist[] = {(char*)"player X position delta", (char*)"player Y position delta", (char*)"player Z position delta", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
        "ddd", kwlist,
        &x, &y, &z
    )) return NULL;

    player->move({x, y, z});

    return PyBool_FromLong(0);
}

static PyObject *py_setOnWorldLoadCallback(PyObject *self, PyObject *args, PyObject *kwargs) {
    static char *kwlist[] = {(char*)"onWorldLoadCallback", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
        "O", kwlist,
        &onWorldLoadCallback
    )) return NULL;


    return PyBool_FromLong(0);
}

static PyObject *py_setOnPlayerPositionChangedCallback(PyObject *self, PyObject *args, PyObject *kwargs) {
    static char *kwlist[] = {(char*)"onPlayerPositionChangedCallback", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
        "O", kwlist,
        &onPlayerPositionChangedCallback
    )) return NULL;


    return PyBool_FromLong(0);
}

static PyMethodDef pyMethods[] = {
    {"defineBlock", (PyCFunction)py_defineBlock, METH_VARARGS | METH_KEYWORDS,
     "Define a block in the game to be used."},
    {"defineDimension", (PyCFunction)py_defineDimension, METH_VARARGS | METH_KEYWORDS,
     "Define a dimension defineDimension(name, generateChunkCallback, chunkSize=32)."},
    {"setPlayerDimension", (PyCFunction)py_setPlayerDimension, METH_VARARGS | METH_KEYWORDS,
     "Set player's dimension."},
     {"setPlayerPosition", (PyCFunction)py_setPlayerPosition, METH_VARARGS | METH_KEYWORDS,
     "Set player's position directly."},
     {"movePlayer", (PyCFunction)py_movePlayer, METH_VARARGS | METH_KEYWORDS,
     "Change player's position by a delta."},
    {"setOnWorldLoadCallback", (PyCFunction)py_setOnWorldLoadCallback, METH_VARARGS | METH_KEYWORDS,
     "Set a callback to run on world load."},
     {"setOnPlayerPositionChangedCallback", (PyCFunction)py_setOnPlayerPositionChangedCallback, METH_VARARGS | METH_KEYWORDS,
     "Set a callback to run on player position change."},
    {NULL, NULL, 0, NULL}
};


static PyModuleDef gamePyModule = {
    PyModuleDef_HEAD_INIT, "game", NULL, -1, pyMethods,
    NULL, NULL, NULL, NULL
};

static PyObject* PyInit_Game() {
    return PyModule_Create(&gamePyModule);
}


void initPython() {
    if (isPythonInitalized) return;


    PyImport_AppendInittab("game", &PyInit_Game);

    Py_Initialize();

    PyObject *obj = Py_BuildValue("s", "data/py/init.py");
    FILE* fp = _Py_fopen_obj(obj, "r+");;
    PyRun_SimpleFile(fp, "data/py/init.py");



    isPythonInitalized = true;
}


void defineGameData() {
    initPython();
}
