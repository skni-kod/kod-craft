#include "lib/raylib/src/raylib.h"

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