#define PY_SSIZE_T_CLEAN
#include<Python.h>

#define PYTHON_DEFINTION

#include "block.h"
#include "dimension.h"
#include "world.h"
#include "player.h"

bool isPythonInitalized = false;


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
    {"setOnWorldLoadCallback", (PyCFunction)py_setOnWorldLoadCallback, METH_VARARGS | METH_KEYWORDS,
     "Set a callback to run on world load."},
     {"setOnPlayerPositionChangedCallback", (PyCFunction)py_setOnPlayerPositionChangedCallback, METH_VARARGS | METH_KEYWORDS,
     "Set a callback to run on player position change."},
     {"setPlayerCameraOffset", (PyCFunction)py_setPlayerCameraOffset, METH_VARARGS | METH_KEYWORDS,
     "Set an offset for the player camera relative to the player's origin."},
     {"getPlayer", (PyCFunction)py_getPlayer, METH_VARARGS | METH_KEYWORDS,
     "Get the player class."},
    {NULL, NULL, 0, NULL}
};


static PyModuleDef gamePyModule = {
    PyModuleDef_HEAD_INIT, "game", NULL, -1, pyMethods,
    NULL, NULL, NULL, NULL
};

#define addClassToModule(cls, name)\
    PyType_Ready(&cls);\
    if (PyModule_AddObjectRef(module, #name, (PyObject *) &cls) < 0) {\
        Py_DECREF(module);\
        return NULL;\
    }

static PyObject* PyInit_Game() {
    PyObject *module = PyModule_Create(&gamePyModule);

    addClassToModule(py_DimensionClassType, Dimension);
    addClassToModule(py_EntityClassType, Entity);
    addClassToModule(py_BlockClassType, Block);
    addClassToModule(py_BlockInstanceClassType, BlockInstance);

    return module;
}

#define setClassDefaults(cls) \
    cls.ob_base = PyVarObject_HEAD_INIT(nullptr, 0)\
    cls.tp_itemsize = 0;\
    cls.tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE;\
    cls.tp_new = PyType_GenericNew;

void initPython() {
    if (isPythonInitalized) return;

    setClassDefaults(py_DimensionClassType)
    py_DimensionClassType.tp_name = "game.Dimension";
    py_DimensionClassType.tp_basicsize = sizeof(py_DimensionClass);
    py_DimensionClassType.tp_init = reinterpret_cast<initproc>(py_defineDimension);

    setClassDefaults(py_EntityClassType)
    py_EntityClassType.tp_name = "game.Entity";
    py_EntityClassType.tp_basicsize = sizeof(py_EntityClass);
    py_EntityClassType.tp_init = reinterpret_cast<initproc>(pyInitEntity);
    py_EntityClassType.tp_methods = pyMethodsEntity;

    setClassDefaults(py_BlockClassType)
    py_BlockClassType.tp_name = "game.Block";
    py_BlockClassType.tp_basicsize = sizeof(py_BlockInstanceClass);
    py_BlockClassType.tp_init = reinterpret_cast<initproc>(pyInitBlock);

    setClassDefaults(py_BlockInstanceClassType)
    py_BlockInstanceClassType.tp_name = "game.BlockInstance";
    py_BlockInstanceClassType.tp_basicsize = sizeof(py_BlockClass);
    py_BlockInstanceClassType.tp_init = reinterpret_cast<initproc>(pyInitBlockInstance);


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
