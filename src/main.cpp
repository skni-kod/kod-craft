#include "lib/raylib/src/raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "lib/raygui.h"

#include "py.h"
#include "block.h"

#include "states.h"

GameState gameState;

int main() {
    InitWindow(800, 450, "Kod Craft");

    gameState = STATE_IN_GAME;

    //load data from python files
    defineGameData();

    printf("Defined %d blocks.\n", getDefinedBlockCount());

    while (!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(RAYWHITE);

        EndDrawing();
    }

    CloseWindow();


    return 0;
}