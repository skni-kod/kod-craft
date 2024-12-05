#define PY_SSIZE_T_CLEAN
#include<Python.h>

#include "block.h"

bool isPythonInitalized = false;


static PyObject *py_defineBlock(PyObject *self, PyObject *args, PyObject *kwargs) {
    char *name;
    int solid = 1;

    static char *kwlist[] = {(char*)"name", (char*)"solid", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
        "s|p", kwlist,
        &name, &solid
    )) return NULL;


    defineBlock(name, solid);

    return PyBool_FromLong(0);
}

static PyMethodDef pyMethods[] = {
    {"defineBlock", (PyCFunction)py_defineBlock, METH_VARARGS | METH_KEYWORDS,
     "Define a block in the game to be used."},
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
