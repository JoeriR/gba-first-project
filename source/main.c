#ifndef MAIN_C
#define MAIN_C

#include <tonc.h>

#include "player.h"
#include "sprites.h"

OBJ_ATTR obj_buffer[128];

u32 tileId = 0;
u32 palleteBank = 0;

// Game objects
Player player = {
    x: 64,
    y: 64,
    width: 16,
    height: 16,
    direction: 1
};

// Pointer to OAM for the Player Object
OBJ_ATTR *playerOamObj = &obj_buffer[0];

void updateOam()
{
    // Update Player's position
    obj_set_pos(playerOamObj, player.x, player.y);

    // Write buffer back to the real OAM
    oam_copy(oam_mem, obj_buffer, 1);
}

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

    obj_set_attr(playerOamObj, ATTR0_SQUARE, ATTR1_SIZE_16, ATTR2_PALBANK(palleteBank) | tileId);

    obj_set_pos(playerOamObj, player.x, player.y);
}

void gameLoop()
{
    // Wait V-Sync
    vid_vsync();    // TODO: Replace with a battery efficient solution (interupts )

    // Read gamepad
    key_poll();

    // Handle input
    if (key_held(KEY_RIGHT) && player.x < SCREEN_WIDTH - 16) {
        ++player.x;
        BIT_CLEAR(playerOamObj->attr1, ATTR1_HFLIP);
    }
    if (key_held(KEY_LEFT) && player.x > 0) {
        --player.x;
        BIT_SET(playerOamObj->attr1, ATTR1_HFLIP);    // Flip sprite to face left
    }
    if (key_held(KEY_UP) && player.y > 0)
        --player.y;
    if (key_held(KEY_DOWN) && player.y < SCREEN_HEIGHT - 16)
        ++player.y;

    // Set Player sprite

    // Update projectiles

    // Handle collision

    // Update enemies

    // Prevent out of bounds

    // Update OAM
    updateOam();

    // TODO: Wait for V-Blank interrupt (probably)
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