#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "lib/raygui.h"

#include "py.h"
#include "block.h"
#include "world.h"
#include "states.h"
#include "player.h"
#include "spinlock.h"

GameState gameState;
extern spinlock lock;

inline void exitGame() {
    unloadWorld();
    unloadBlocks();
    unloadDimensions();
    CloseWindow();
}

int main() {
    InitWindow(800, 450, "Kod Craft");

    gameState = STATE_MAIN_MENU;
    //load data from python files
    defineGameData();

    printf("Defined %d blocks.\nLoading World...\n", getDefinedBlockCount());

    loadWorld();
    PyThreadState* st = PyEval_SaveThread();
    DisableCursor();
    printf("Start\n");

    while (!WindowShouldClose()){
        lock.unlock();
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (gameState == STATE_IN_GAME) player->render();

        DrawFPS(10, 10);

        EndDrawing();
    }
    PyEval_RestoreThread(st);
    finalizePython();
    exitGame();


    return 0;
}