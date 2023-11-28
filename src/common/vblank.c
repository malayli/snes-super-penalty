#include <snes.h>
#include "vblank.h"
#include "gameState.h"
#include "bgText.h"
#include "utils.h"
#include "variables.h"

// ROM

const u16 oamSpriteAddress = 0;
const u16 oamSpriteSize = 0x220;

/*!\brief Handles the interruption of VBlank.
    \note Send information to VRAM via DMA.
*/
void superNintendoVblank() {
    // Refresh the sprite on the screen
    dmaCopyOAram((unsigned char *) &oamMemory, oamSpriteAddress, oamSpriteSize);

    if (refreshBg1 == 1) {
        dmaCopyVramChannel1((u8 *)bg1TileMap, 0x0000, 2048);
        refreshBg1 = 0;
    }

    if (refreshBg3 == 1) {
        dmaCopyVramChannel1((u8 *)bg3TileMap, bg3TextAddress, bgTextSize);
        refreshBg3 = 0;
    }

    if (refreshScore == 1) {
        dmaCopyVramChannel1((u8 *)scoreTileMapUp, scoreAddressUp, scoreTileMapSize);
        dmaCopyVramChannel1((u8 *)scoreTileMapDown, scoreAddressDown, scoreTileMapSize);
        refreshScore = 0;
    }

    if (refreshScore2 == 1) {
        dmaCopyVramChannel1((u8 *)score2TileMapUp, score2AddressUp, score2TileMapSize);
        dmaCopyVramChannel1((u8 *)score2TileMapDown, score2AddressDown, score2TileMapSize);
        refreshScore2 = 0;
    }

    if (refreshStriker == 1) {
        // 31 tiles to copy (31*32 = 992 bytes)
        dmaCopyVramChannel1(strikerSprites, 0x7000, 992);
        refreshStriker = 0;
    }

    if (refreshGoalkeeper == 1) {
        // 14 tiles to copy (16*32 = 256 bytes)
        dmaCopyVramChannel1(goalkeeperSprites, 0x6000, 256);
        dmaCopyVramChannel1(goalkeeperSprites + 256, 0x6100, 256);
        refreshGoalkeeper = 0;
    }
}
