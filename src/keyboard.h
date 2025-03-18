#pragma once

const bool KEY_PRESSED = true;
const bool KEY_DEPRESSED = false;

namespace Keyboard {
    class Key {
        int keyCode;
        bool state;
    public:
        Key(int keyCode);
        ~Key();
        
        void onPressed();
        void onDepressed();

        bool getState();
    };
}

#ifdef PYTHON_DEFINTION
#define USE_PYTHON
#endif

#ifdef USE_PYTHON

struct py_KeyboardKeyClass {
    PyObject_HEAD
    Keyboard::Key* instance;
};

#endif


#ifdef PYTHON_DEFINTION

PyTypeObject py_KeyboardKeyClassType;

int pyInitKeyboardKey(py_KeyboardKeyClass* self, PyObject* args, PyObject* kwargs);

#endif
