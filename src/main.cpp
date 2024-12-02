#include "lib/raylib/src/raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "lib/raygui.h"

#include<Python.h>

int main() {
    InitWindow(800, 450, "Kod Craft");

    while (!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(RAYWHITE);

        EndDrawing();
    }

    CloseWindow();


    return 0;
}