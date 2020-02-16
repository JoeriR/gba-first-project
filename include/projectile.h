#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "hitbox.h"

#define PROJECTILE_LIMIT 10

typedef struct Projectile {
    int x;
    int y;
    int direction;  // use -1 for left and +1 for right
    int isActive;
    Hitbox hitbox;
} Projectile;

extern Projectile projectileData[PROJECTILE_LIMIT];
extern Projectile *projectilePointers[PROJECTILE_LIMIT];

void updateProjectile(Projectile *projectile);
void fireProjectile(Projectile *projectile);
void despawnProjectile(Projectile *projectile);

#endif