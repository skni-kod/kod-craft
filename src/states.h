#ifndef STATES_H
#define STATES_H

enum GameState {
    STATE_MAIN_MENU,
    STATE_LOADING_GAME,
    STATE_IN_GAME
};

extern GameState gameState;

#endif
