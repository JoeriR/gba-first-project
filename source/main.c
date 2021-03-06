#ifndef MAIN_C
#define MAIN_C

#include <tonc.h>

#include "block_spritedata.h"
#include "defines.h"
#include "hitbox.h"
#include "player.h"
#include "projectile.h"
#include "sprites.h"

#define OAM_BUFFER_SIZE 128

OBJ_ATTR obj_buffer[OAM_BUFFER_SIZE];

u32 tileId = 0;
u32 palleteBank = 0;

// Game objects
Player player = {
    x: 100,
    y: 64,
    width: 16,
    height: 16,
    direction: 1,
    hurtbox: {
        x: 64,
        y: 64,
        width: 16,
        height: 16
    }
};

// Pointer to OAM for the Player Object
OBJ_ATTR *playerOamObj = &obj_buffer[0];
OBJ_ATTR *testProjectileObj = &obj_buffer[1];

// MAP data (should be moved to it's own file sometime)


int frameCounter = 0;

// TEMP variables
int isJumping = true;
int jumpFrameCounter = 100;

void updateOam()
{
    // Update Player's position
    obj_set_pos(playerOamObj, player.x, player.y);

    // Update Projectile positions
    for (int i = 0; i < PROJECTILE_LIMIT; ++i) {

        if (projectilePointers[i] != nullptr) {
            OBJ_ATTR *attributes = &obj_buffer[i + 1];
            Projectile *projectile = projectilePointers[i];

            obj_set_pos(attributes, projectile->x, projectile->y);
        }
    }

    // Write buffer back to the real OAM
    oam_copy(oam_mem, obj_buffer, OAM_BUFFER_SIZE);
}

void init()
{
    // Init background
    //Empty for now

    // Init sprites (copy tiles to VRAM)
    tonccpy(&tile_mem[4][0], spritesTiles, spritesTilesLen);

    // Copy palette data to VRAM
    tonccpy(pal_obj_mem, spritesPal, spritesPalLen);

    // Copy background data to VRAM
    tonccpy(&tile_mem[0][0], block_spritedataTiles, block_spritedataTilesLen);

    //      Including palletes
    tonccpy(pal_bg_mem, block_spritedataPal, block_spritedataPalLen);

    // Init OAM
    oam_init(obj_buffer, OAM_BUFFER_SIZE);

    // Set Video mode and enable objects AND setup for a 4bpp map
    REG_BG0CNT = BG_CBB(0) | BG_SBB(30) | BG_4BPP | BG_REG_32x32;
    REG_DISPCNT = DCNT_OBJ | DCNT_OBJ_1D | DCNT_MODE0 | DCNT_BG0;

    obj_set_attr(playerOamObj, ATTR0_SQUARE, ATTR1_SIZE_16, ATTR2_PALBANK(palleteBank) | tileId);
    
    // Enable projectile sprites
    for (int i = 1; i < 1 + PROJECTILE_LIMIT; ++i) {
        obj_set_attr(&obj_buffer[i], ATTR0_SQUARE, ATTR1_SIZE_8, ATTR2_PALBANK(palleteBank) | (i+3));
        obj_set_pos(&obj_buffer[i], 400, 400);
    }

    // Fill the entire background with AIR
    for (int i = 0; i < 32 * 20; ++i) {
        se_mem[30][i] = 0x02;
    }

    const int brickYUpper = 32 * 18;
    const int brickYLower = 32 * 19;

    // Draw bricks across the floor
    for (int i = 0; i < 32; i += 2) {

        se_mem[30][i + brickYUpper] = 0x04;
        se_mem[30][i + 1 + brickYUpper] = 0x05;
        se_mem[30][i + brickYLower] = 0x0C;
        se_mem[30][i + 1 + brickYLower] = 0x0D;
    }

    obj_set_pos(playerOamObj, player.x, player.y);
}

void gameLoop()
{
    // Wait V-Sync
    vid_vsync();    // TODO: Replace with a battery efficient solution (interupts )

    // Read gamepad
    key_poll();

    // Handle input
    if (key_is_down(KEY_RIGHT) && player.x < SCREEN_WIDTH - 16) {
        ++player.x;
        player.direction = 1;
        BIT_CLEAR(playerOamObj->attr1, ATTR1_HFLIP);
    }
    if (key_is_down(KEY_LEFT) && player.x > 0) {
        --player.x;
        player.direction = -1;
        BIT_SET(playerOamObj->attr1, ATTR1_HFLIP);    // Flip sprite to face left
    }

    if (key_is_down(KEY_A) && !isJumping)
        isJumping = true;

    // Fire projectile
    if (key_hit(KEY_B)) {
        
        int projectileX = player.x + 16;

        if (player.direction == -1)
            projectileX = player.x - 8;

        Projectile projectile = {
            x: projectileX,
            y: player.y + 4,
            direction: player.direction,
            isActive: 1,
            hitbox: {
                x: player.x + 16,
                y: player.y + player.width + 8,
                width: 8,
                height: 8
            }
        };

        fireProjectile(&projectile);
    }

    // Handle jumping
    if (isJumping) {

        if (jumpFrameCounter < 20)
            --player.y;
        else if (jumpFrameCounter < 25)
            player.y = player.y;
        else {
            ++player.y;

            // Reset jump variables when the Player lands on the ground
            if (player.y >= 128) {
                player.y = 128;
                isJumping = false;
                jumpFrameCounter = 0;
            }
        }

        ++jumpFrameCounter;
    }

    // Set Player sprite

    // Update active projectiles
    for (int i = 0; i < PROJECTILE_LIMIT; ++i) {
        if (projectilePointers[i] != nullptr) {
            updateProjectile(projectilePointers[i]);
        }
    }

    // Handle collision

    // Update enemies

    // Prevent out of bounds

    // Update OAM
    updateOam();

    // TODO: Wait for V-Blank interrupt (probably)

    ++frameCounter;
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