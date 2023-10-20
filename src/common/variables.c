#include <snes.h>
#include "utils.h"

// ROM

const u16 circleUpTileMap[] = {
    0x182 | (0 << 14) | TPAL2 | (1<<13),
    0x182 | (1 << 14) | TPAL2 | (1<<13)
};

const u16 circleDownTileMap[] = {
    0x183 | (0 << 14) | TPAL2 | (1<<13),
    0x183 | (1 << 14) | TPAL2 | (1<<13)
};

const u16 crossUpTileMap[] = {
    0x180 | (0 << 14) | TPAL3 | (1<<13),
    0x180 | (1 << 14) | TPAL3 | (1<<13)
};

const u16 crossDownTileMap[] = {
    0x181 | (0 << 14) | TPAL3 | (1<<13),
    0x181 | (1 << 14) | TPAL3 | (1<<13)
};

//RAM

u8 refreshStriker;
u8 refreshGoalkeeper;
u8 *strikerSprites;
u8 *goalkeeperSprites;

u16 scoreTileMapUp[10], scoreTileMapDown[10], score2TileMapUp[10], score2TileMapDown[10];
u16 scoreAddressUp, scoreAddressDown, score2AddressUp, score2AddressDown;
u16 scoreTileMapSize, score2TileMapSize;
