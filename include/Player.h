#ifndef PLAYER_H
#define PLAYER_H

#include "Vectors.h"

typedef struct Player {
    Vec2 pos;
    Vec2 dir;
    Vec2 plane;
} Player;

#endif // PLAYER_H