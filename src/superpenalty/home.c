#include <snes.h>
#include "../common/gameState.h"
#include "../common/input.h"
#include "../common/bgText.h"
#include "../common/bgHud.h"
#include "../common/utils.h"
#include "../common/palette.h"
#include "home.h"
#include "level.h"

// ROM

extern char homePic, homePic_end;
extern char homePalette, homePalette_end;
extern char homeMap, homeMap_end;

const u8 emptyPicture[] = {
    // First part
    0b00000000, 0b00000000, // Bit plane 1 + Bit plane 0
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000,

    // Second part
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000
};

/*!\brief Initialize the Home screen.
*/
void initHome() {
    gameState = GameStateHome;

    bgSetMapPtr(BG0, 0x0000, SC_32x32);

    bgInitTileSet(BG0,
        &homePic,
        &homePalette,
        PAL1,
        (&homePic_end - &homePic),
        16*2*2,
        BG_16COLORS,
        0x3000);

    WaitForVBlank();

    dmaCopyVram(&homeMap, 0x0000, 2048);

    initBg3FontMapEx(&hudFontPic, &hudFontPic_end, &hudFontPalette, PAL0, 0x1000, 0x5000);
    printName("PRESS START", 11);
    refreshBg3 = 1;

    setMode(BG_MODE1, BG3_MODE1_PRORITY_HIGH);
    bgSetEnable(BG0);
    bgSetDisable(BG1);
    bgSetEnable(BG2);
    bgSetDisable(BG3);

    // Workaround for SNES
    bgSetScroll(BG0, 0, -1);
    bgSetScroll(BG1, 0, -1);
    bgSetScroll(BG2, 0, -1);

    setFadeEffect(FADE_IN);
	WaitForVBlank();
}

/*!\brief Handle the Home screen inputs.
*/
void handleHomeInput() {
    if(padDown0 & KEY_START) {
        clearHome();
        initLevelSession(CountryTypeFrance, CountryTypeBrazil, 5, LevelStateNormal);
    }
}

/*!\brief Clear the Home screen.
*/
void clearHome() {
    // Fade out so light to black
	setFadeEffect(FADE_OUT);
	WaitForVBlank();

    oamClear(0, 0);
    dmaClearVram();

    bgSetDisable(BG0);
    bgSetDisable(BG1);
    bgSetDisable(BG2);
    bgSetDisable(BG3);
}
