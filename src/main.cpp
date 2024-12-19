#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "lib/raygui.h"

#include "py.h"
#include "block.h"
#include "world.h"
#include "states.h"
#include "player.h"

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

        world->processTick(); //TODO: run every n ms instead of every frame
        if (gameState == STATE_IN_GAME) player->draw();

        DrawFPS(10, 10);

        EndDrawing();
    }

    CloseWindow();


    return 0;
}