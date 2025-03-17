#include "keyboard.h"

Keyboard::Keyboard(int keyCode) {
    this->keyCode = keyCode;
    this->state = KEY_DEPRESSED;
}

void Keyboard::onPressed() {
    this->state = KEY_PRESSED;
}

void Keyboard::onDepressed() {
    this->state = KEY_DEPRESSED;
}

bool Keyboard::getState() {
    return this->state;
}
