#define PY_SSIZE_T_CLEAN
#include<Python.h>

#include "block.h"
#include "dimension.h"

bool isPythonInitalized = false;


static PyObject *py_defineBlock(PyObject *self, PyObject *args, PyObject *kwargs) {
    char *name;
    int solid = 1;
    int visible = 1;

    static char *kwlist[] = {(char*)"name", (char*)"solid", (char*)"visible", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
        "s|pp", kwlist,
        &name, &solid, &visible
    )) return NULL;


    defineBlock(name, solid, visible);

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

static PyMethodDef pyMethods[] = {
    {"defineBlock", (PyCFunction)py_defineBlock, METH_VARARGS | METH_KEYWORDS,
     "Define a block in the game to be used."},
    {"defineDimension", (PyCFunction)py_defineDimension, METH_VARARGS | METH_KEYWORDS,
     "Define a dimension defineDimension(name, generateChunkCallback, chunkSize=32)."},
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
