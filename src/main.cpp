#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "lib/raygui.h"

#include "py.h"
#include "block.h"
#include "world.h"
#include "states.h"
#include "player.h"
#include "spinlock.h"
#include "keyboard.h"

constexpr char FPS_FLAG_SMALL[] = "-f";
constexpr char FPS_FLAG_LONG[] = "--fps";

GameState gameState;
extern spinlock lock;

inline void exitGame() {
    unloadWorld();
    unloadBlocks();
    unloadDimensions();
    CloseWindow();
}

int main(int argc, char* argv[]) {
    uint16_t fps = 60;
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], FPS_FLAG_SMALL) || !strcmp(argv[i], FPS_FLAG_LONG)) {
            if (i + 1 >= argc)
                puts("Did not set fps in command line");
            else {
                fps = (uint16_t)atoi(argv[i + 1]);
                break;
            }
        }
    }

    InitWindow(800, 450, "Kod Craft");
    SetTargetFPS(fps);

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
        updateKeyboard();
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