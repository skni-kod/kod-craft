#ifndef PLAYER_H
#define PLAYER_H

class Player;

#include "dimension.h"

class Player {
private:
    Dimension* dimension;

    double x, y, z;
public:
    Player();

    void setDimension(Dimension* dimension);

    void draw();
};

#endif
