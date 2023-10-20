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

    inputs[0] = handleHomeInput;
    inputs[1] = handleLevelInput;
    inputs[2] = emptyFunction;
    inputs[3] = emptyFunction;
    inputs[4] = emptyFunction;
    inputs[5] = emptyFunction;
    inputs[6] = emptyFunction;
    inputs[7] = emptyFunction;
    inputs[8] = emptyFunction;
    inputs[9] = emptyFunction;
    inputs[10] = emptyFunction;
    inputs[11] = emptyFunction;
    inputs[12] = emptyFunction;
    inputs[13] = emptyFunction;

    updates[0] = emptyFunction;
    updates[1] = updateLevel;
    updates[2] = emptyFunction;
    updates[3] = emptyFunction;
    updates[4] = emptyFunction;
    updates[5] = emptyFunction;
    updates[6] = emptyFunction;
    updates[7] = emptyFunction;
    updates[8] = emptyFunction;
    updates[9] = emptyFunction;
    updates[10] = emptyFunction;
    updates[11] = emptyFunction;
    updates[12] = emptyFunction;
    updates[13] = emptyFunction;

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
