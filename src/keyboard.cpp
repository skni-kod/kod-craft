#include<Python.h>
#define USE_PYTHON

#include "keyboard.h"

std::vector<Keyboard::Key*> keyList;

Keyboard::Key::Key(int keyCode) {
    this->keyCode = keyCode;
    this->state = KEY_DEPRESSED;

    keyList.push_back(this);
}

void Keyboard::Key::onPressed() {
    this->state = KEY_PRESSED;
}

void Keyboard::Key::onDepressed() {
    this->state = KEY_DEPRESSED;
}

bool Keyboard::Key::getState() {
    return this->state;
}

int pyInitKeyboardKey(py_KeyboardKeyClass* self, PyObject* args, PyObject* kwargs) {
    int keyCode;

    static char *kwlist[] = {(char*)"keyCode", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
        "i", kwlist,
        &keyCode
    )) return -1;


    self->instance = new Keyboard::Key(keyCode);
    return 0;
}

PyObject *py_KeyboardKeyGet(py_KeyboardKeyClass* self, PyObject *args, PyObject *kwargs) {
    return PyBool_FromLong(self->instance->getState());
}
