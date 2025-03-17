#pragma once

const bool KEY_PRESSED = true;
const bool KEY_DEPRESSED = false;

class KeyboardKey {
    int keyCode;
    bool state;
public:
    KeyboardKey(int keyCode);
    ~KeyboardKey();
    
    void onPressed();
    void onDepressed();

    bool getState();
};

#ifdef PYTHON_DEFINTION
#define USE_PYTHON
#endif

#ifdef USE_PYTHON

struct py_KeyboardKeyClass {
    PyObject_HEAD
    KeyboardKey* instance;
};

#endif


#ifdef PYTHON_DEFINTION

PyTypeObject py_KeyboardKeyClassType;

int pyInitKeyboardKey(py_KeyboardKeyClass* self, PyObject* args, PyObject* kwargs);

#endif
