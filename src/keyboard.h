#pragma once

const bool KEY_PRESSED = true;
const bool KEY_DEPRESSED = false;

class Keyboard {
    int keyCode;
    bool state;
public:
    Keyboard(int keyCode);
    ~Keyboard();
    
    void onPressed();
    void onDepressed();

    bool getState();
};
