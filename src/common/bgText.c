#include <snes.h>

#include "bgText.h"
#include "vblank.h"
#include "utils.h"
#include "palette.h"

// ROM

const u16 bgTextSize = 2048;

// RAM

u16 bg3TextAddress;
char *bgTilePointer;
int bgTileIndex;

/*!\brief Initialize the BG3 Font for mode 1.
*/
void initBg3FontMapEx(char *fontTileSource,
    char *fontTileSource_end,
    char *fontTilePalette,
    u8 paletteNumber,
    u16 mapAddress,
    u16 tileAddress) {
    bg3TextAddress = mapAddress;
    
    bgSetMapPtr(BG2, mapAddress, SC_32x32);

    bgInitTileSet(BG2, 
        fontTileSource,
        fontTilePalette,
        paletteNumber,
        (fontTileSource_end - fontTileSource),
        paletteSize4*3,
        BG_4COLORS,
        tileAddress);
    
    clearBgText(bg3TileMap);
}

/*!\brief Set all the tiles to 0.
*/
void clearBgText(u16 *tileMap) {
    for (bgTileIndex=0; bgTileIndex < 1024;) {
        tileMap[bgTileIndex] = 0;
        bgTileIndex += 1;
    }
}

/*!\brief Print text on BG3 and set priority to 1.
    \param x the X position
    \param y the Y position
    \param number the tile number to print (u8)
    \param paletteNumber the palette number
*/
void bg3PrintInt(u16 x, u16 y, u16 number, u8 paletteNumber) {
    bg3TileMap[x + (y<<5)] = number | (paletteNumber<<10) | (1<<13); // on BG3 so priority high
}

/*!\brief Print text on BG3 and set priority to 1.
    \param x the X position
    \param y the Y position
    \param number the tile number to print (u8)
    \param paletteNumber the palette number
    \param hflip the horizontal flip
*/
void bg3PrintIntEx(u16 x, u16 y, u16 number, u8 paletteNumber, u8 hflip) {
    bg3TileMap[x + (y<<5)] = number | (hflip << 14) | (paletteNumber<<10) | (1<<13); // on BG3 so priority high
}
