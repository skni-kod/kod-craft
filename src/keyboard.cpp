#include<Python.h>
#define USE_PYTHON

#include "keyboard.h"

KeyboardKey::KeyboardKey(int keyCode) {
    this->keyCode = keyCode;
    this->state = KEY_DEPRESSED;
}

void KeyboardKey::onPressed() {
    this->state = KEY_PRESSED;
}

void KeyboardKey::onDepressed() {
    this->state = KEY_DEPRESSED;
}

bool KeyboardKey::getState() {
    return this->state;
}

int pyInitEntity(py_KeyboardKeyClass* self, PyObject* args, PyObject* kwargs) {
    int keyCode;

    static char *kwlist[] = {(char*)"keyCode", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
        "i", kwlist,
        &keyCode
    )) return -1;


    self->instance = new KeyboardKey(keyCode);
    return 0;
}
