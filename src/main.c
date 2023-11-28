#include <snes.h>

#include "common/gameState.h"
#include "common/vblank.h"
#include "common/utils.h"
#include "common/input.h"
#include "common/variables.h"
#include "common/bgHud.h"
#include "superpenalty/home.h"
#include "superpenalty/database.h"
#include "superpenalty/level.h"

int main(void) {
    // Initialize sound engine (take some time)
    spcBoot();

    // Initialize SNES
    consoleInit();

    nmiSet(superNintendoVblank);

    inputs[GameStateHome] = handleHomeInput;
    inputs[GameStateLevel] = handleLevelInput;

    updates[GameStateHome] = emptyFunction;
    updates[GameStateLevel] = updateLevel;

    refreshBg1 = 0;
    refreshBg3 = 0;
    refreshScore = 0;
    refreshScore2 = 0;
    refreshStriker = 0;
    refreshGoalkeeper = 0;

    initHome();

    while(1) {
        scanPads();
        pad0 = padsCurrent(0);
        padDown0 = padsDown(0);
        
        inputs[gameState]();
        updates[gameState]();

        spcProcess();

        WaitForVBlank();
    }
    return 0;
}
