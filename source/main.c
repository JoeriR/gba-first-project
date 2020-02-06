#ifndef MAIN_C
#define MAIN_C

#include <tonc.h>

#include "sprites.h"

OBJ_ATTR obj_buffer[128];

int x = 64;
int y = 64;
u32 tileId = 0;
u32 palleteBank = 0;

// Object attributes are stored in the 1 entry of OAM objects
OBJ_ATTR *objectAttributes = &obj_buffer[0];

void init()
{
    // Init background
    //Empty for now

    // Init sprites (copy tiles to VRAM)
    tonccpy(&tile_mem[4][0], spritesTiles, spritesTilesLen);

    // Copy palette data to VRAM
    tonccpy(pal_obj_mem, spritesPal, spritesPalLen);

    // Init OAM
    oam_init(obj_buffer, 128);

    // Set Video mode and enable objects
    REG_DISPCNT = DCNT_OBJ | DCNT_OBJ_1D;

    obj_set_attr(objectAttributes, ATTR0_SQUARE, ATTR1_SIZE_16, ATTR2_PALBANK(palleteBank) | tileId);

    obj_set_pos(objectAttributes, x, y);
}

void gameLoop()
{
    // Wait V-Sync
    vid_vsync();    // TODO: Replace with a battery efficient solution (interupts )

    // Read gamepad
    key_poll();

    // Handle input
    if (key_held(KEY_RIGHT) && x < SCREEN_WIDTH - 16)
        ++x;
    if (key_held(KEY_LEFT) && x > 0)
        --x;
    if (key_held(KEY_UP) && y > 0)
        --y;
    if (key_held(KEY_DOWN) && y < SCREEN_HEIGHT - 16)
        ++y;

    // Set Player sprite

    // Update projectiles

    // Handle collision

    // Update enemies

    // Prevent out of bounds

    // Update OAM
    obj_set_pos(objectAttributes, x, y);

    oam_copy(oam_mem, obj_buffer, 1);
}


int main()
{
    init();

    while (1)
    {
        gameLoop();
    }

    return 0;
}

#endif