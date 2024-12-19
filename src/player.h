#ifndef PLAYER_H
#define PLAYER_H

class Player;

extern Player* player;

#include "dimension.h"

class Player {
private:
    Dimension* dimension;

    double x, y, z;
public:
    Player();

    void setDimension(Dimension* dimension);
    void setDimension(std::string dimension);

    void draw();

    void processTick();
};

#endif
