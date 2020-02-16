#ifndef PROJECTILE_CPP
#define PROJECTILE_CPP

#include "defines.h"
#include "projectile.h"

Projectile projectileData[PROJECTILE_LIMIT];
Projectile *projectilePointers[PROJECTILE_LIMIT] = { nullptr };    // Default all pointers to NULL

void updateProjectile(Projectile *projectile) {
    // Move Projectile
    projectile->x += projectile->direction;

    // Sync Hitbox
    projectile->hitbox.x = projectile->x;
    projectile->hitbox.y = projectile->y;

    // Despawn the Projectile if it is out of bounds
    if (projectile->x > 240 || projectile->x < 0 - projectile->hitbox.width)
        despawnProjectile(projectile);
}

// Loop through ProjectilePointers to find a free spot
// Then bind it to projectileData and copy the input data over
void fireProjectile(Projectile *projectileInput) {

    for (int i = 0; i < PROJECTILE_LIMIT; ++i) {
        if (projectilePointers[i] == nullptr) {

            Projectile *newProjectile = &projectileData[i];

            projectilePointers[i] = newProjectile;

            newProjectile->x = projectileInput->x;
            newProjectile->y = projectileInput->y;
            newProjectile->direction = projectileInput->direction;
            newProjectile->isActive = 1;
            newProjectile->hitbox = projectileInput->hitbox;

            break;
        }
    }
}

// Search for the projectile in the projectilePointers
// Then de-activate the projectile and remove it from projectilePointers
void despawnProjectile(Projectile *projectile) {
    for (int i = 0; i < PROJECTILE_LIMIT; ++i) {
        if (projectilePointers[i] == projectile) {
            projectile->isActive = 0;
            projectilePointers[i] = nullptr;

            break;
        }
    }
}

#endif