#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "lib/raygui.h"

#include "py.h"
#include "block.h"
#include "world.h"
#include "states.h"

GameState gameState;

int main() {
    InitWindow(800, 450, "Kod Craft");

    gameState = STATE_MAIN_MENU;

    //load data from python files
    defineGameData();

    printf("Defined %d blocks.\n", getDefinedBlockCount());

    loadWorld();

    while (!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(RAYWHITE);

        EndDrawing();
    }

    CloseWindow();


    return 0;
}