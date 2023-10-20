#include <snes.h>
#include "bgHud.h"
#include "bgText.h"
#include "utils.h"
#include "variables.h"
#include "oam.h"

// RAM

char *bg3TilePointer;
u16 bg3Index;
u16 titleX;
u16 countryNameOffsetX;
u16 asciiValue;

/*!\brief Initialize the HUD.
    \param tilesetAddress the tileset address
*/
void initHud(u16 tilesetAddress) {
    initBg3FontMapEx(&hudFontPic, &hudFontPic_end, &hudFontPalette, PAL0, 0x1000, tilesetAddress);
    refreshBg3 = 1;
}

/*!\brief Print a player name.
    \param name the player name
    \param x the player name x position
*/
void printName(char *name, u16 x) {
    bg3TilePointer = name;
    bg3Index = x;
    while (*bg3TilePointer != 0) {
        asciiValue = (u16)(*bg3TilePointer);
        if (asciiValue == 46) { // .
            bg3PrintInt(bg3Index, 23, 0x000, PAL0);
            bg3PrintInt(bg3Index, 24, 0x082, PAL0);

        } else if (asciiValue == 32) { // space ' '
            bg3PrintInt(bg3Index, 23, 0, PAL0);
            bg3PrintInt(bg3Index, 24, 0, PAL0);

        } else if (asciiValue < 70) {
            bg3PrintInt(bg3Index, 23, 144 + ((*bg3TilePointer - 65)<<1), PAL0);
            bg3PrintInt(bg3Index, 24, 144 + ((*bg3TilePointer - 65)<<1) + 1, PAL0);
                
        } else if (asciiValue == 70) { // F
            bg3PrintInt(bg3Index, 23, 144 + ((*bg3TilePointer - 65)<<1) - 2, PAL0);
            bg3PrintInt(bg3Index, 24, 144 + ((*bg3TilePointer - 65)<<1), PAL0);

        } else if (asciiValue <= 81) {
            bg3PrintInt(bg3Index, 23, 144 + ((*bg3TilePointer - 65)<<1) - 1, PAL0);
            bg3PrintInt(bg3Index, 24, 144 + ((*bg3TilePointer - 65)<<1), PAL0);

        } else if (asciiValue == 82) { // R
            bg3PrintInt(bg3Index, 23, 173, PAL0);
            bg3PrintInt(bg3Index, 24, 144 + ((*bg3TilePointer - 65)<<1) - 1, PAL0);

        } else if (asciiValue == 83) { // S
            bg3PrintInt(bg3Index, 23, 144 + ((*bg3TilePointer - 65)<<1) - 2, PAL0);
            bg3PrintInt(bg3Index, 24, 144 + ((*bg3TilePointer - 65)<<1) - 1, PAL0);

        } else if (asciiValue == 84) { // T
            bg3PrintInt(bg3Index, 23, 144 + ((*bg3TilePointer - 65)<<1) - 2, PAL0);
            bg3PrintInt(bg3Index, 24, 144 + 16, PAL0);

        } else if (asciiValue == 85) { // U
            bg3PrintInt(bg3Index, 23, 144 + ((*bg3TilePointer - 65)<<1) - 3, PAL0);
            bg3PrintInt(bg3Index, 24, 144 + 28, PAL0);

        } else if (asciiValue <= 90) {
            bg3PrintInt(bg3Index, 23, 144 + ((*bg3TilePointer - 65)<<1) - 4, PAL0);
            bg3PrintInt(bg3Index, 24, 144 + ((*bg3TilePointer - 65)<<1) - 3, PAL0);

        } else {
            bg3PrintInt(bg3Index, 23, ((*bg3TilePointer - 97) << 1) + 1, PAL0);
            bg3PrintInt(bg3Index, 24, ((*bg3TilePointer - 97) << 1) + 2, PAL0);
        }
        bg3TilePointer++;
        bg3Index++;
    }
}

/*!\brief Print the striker role on BG3.
    \param role the player role
    \param x the player role x position
*/
void printRole(u8 role, u16 x) {
    switch(role) {
        case 0: // GK
            bg3PrintInt(x, 23, 0x18C, PAL1);
            bg3PrintInt(x, 24, 0x18D, PAL1);
            break;

        case 1: // DF
            bg3PrintInt(x, 23, 0x18E, PAL1);
            bg3PrintInt(x, 24, 0x18F, PAL1);
            break;

        case 2: // MF
            bg3PrintInt(x, 23, 0x17E, PAL1);
            bg3PrintInt(x, 24, 0x18F, PAL1);
            break;

        case 3: // FW
            bg3PrintInt(x, 23, 0x18F, PAL1);
            bg3PrintInt(x, 24, 0x17F, PAL1);
            break;
    }
}

/*!\brief Set a character in the OAM.
    \param letterUp the up part of the character.
    \param letterDown the down part of the character.
    \param paletteNumber the palette number.
*/
void oamSetCharacter(u16 letterUp, u16 letterDown, u8 paletteNumber) {
    oamSet(oamIdOffset, titleX + countryNameOffsetX, 8, prio, 0, 0, letterUp, paletteNumber); oamSetEx(oamIdOffset, OBJ_SMALL, OBJ_SHOW); oamIdOffset += 4;
    oamSet(oamIdOffset, titleX + countryNameOffsetX, 16, prio, 0, 0, letterDown, paletteNumber); oamSetEx(oamIdOffset, OBJ_SMALL, OBJ_SHOW); oamIdOffset += 4;
    countryNameOffsetX += 8;
}

/*!\brief Set LAND string in the OAM.
*/
void oamSetLand() {
    // LA
    oamSet(oamIdOffset, titleX + countryNameOffsetX, 8, prio, 0, 0, 0xAB, PAL6); oamSetEx(oamIdOffset, OBJ_LARGE, OBJ_SHOW); oamIdOffset += 4;
    countryNameOffsetX += 16;
    // ND
    oamSet(oamIdOffset, titleX + countryNameOffsetX, 8, prio, 0, 0, 0xAD, PAL6); oamSetEx(oamIdOffset, OBJ_LARGE, OBJ_SHOW); oamIdOffset += 4;
    countryNameOffsetX += 16;
}

/*!\brief Set a string in the OAM.
    \param string the string to set in the OAM.
*/
void oamSetString(char *string) {
    bg3TilePointer = string;
    while (*bg3TilePointer != 0) {
        asciiValue = (u16)(*bg3TilePointer);
        if (asciiValue < 82) { // 82 = R
            oamSetCharacter(
                128 + asciiValue - 65, 
                144 + asciiValue - 65, 
                PAL6);

        } else {
            oamSetCharacter(
                161 + asciiValue - 82, 
                177 + asciiValue - 82, 
                PAL6);
        }

        bg3TilePointer++;
    }
}
