#include "lib/raylib/src/raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "lib/raygui.h"

#include "py.h"

enum GameState {
    STATE_MAIN_MENU,
    STATE_LOADING_GAME,
    STATE_IN_GAME
};

GameState gameState;

int main() {
    InitWindow(800, 450, "Kod Craft");

    gameState = STATE_IN_GAME;

    //load data from python files
    defineGameData();

    while (!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(RAYWHITE);

        EndDrawing();
    }

    CloseWindow();


    return 0;
}