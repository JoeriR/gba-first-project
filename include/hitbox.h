#ifndef HITBOX_H
#define HITBOX_H

// This struct stores const data for hitboxes
// It's position is relative to the players x- and y-coordinates
typedef struct ConstHitbox {
    const int xOffset;
    const int yOffset;
    const int width;
    const int height;
} ConstHitbox;

typedef struct Hitbox {
    int x;
    int y;
    int width;
    int height;
} Hitbox;

int collision(Hitbox *hitbox1, Hitbox *hitbox2);
int isPointInBox(int x, int y, Hitbox *hitbox);

#endif