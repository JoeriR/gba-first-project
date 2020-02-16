#ifndef PLAYER_H
#define PLAYER_H

#include "hitbox.h"

typedef struct Player {
    int x;
    int y;
    int width;
    int height;
    int direction;
    Hitbox hurtbox;
} Player;

#endif