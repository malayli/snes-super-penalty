#include <snes.h>
#include "goalkeeper.h"
#include "../common/utils.h"

// GoalKeeperIdle

const u16 goalKeeperIdleSprites[] = {
    117, 101, 0x006, PAL5, 0, OBJ_SMALL,
    133, 93, 0x005, PAL5, 0, OBJ_SMALL,
    133, 101, 0x015, PAL5, 0, OBJ_SMALL,
    125, 103, 0x014, PAL5, 0, OBJ_SMALL,
    133, 85, 0x004, PAL5, 0, OBJ_SMALL,
    117, 85, 0x002, PAL5, 0, OBJ_LARGE,
    121, 69, 0x000, PAL5, 0, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

// GoalKeeperNoMove1

const u16 goalKeeperNoMoveSprites1[] = {
    138, 104, 0x017, PAL5, 0, OBJ_SMALL,
    130, 104, 0x016, PAL5, 0, OBJ_SMALL,
    122, 104, 0x015, PAL5, 0, OBJ_SMALL,
    114, 104, 0x014, PAL5, 0, OBJ_SMALL,
    135, 96, 0x013, PAL5, 0, OBJ_SMALL,
    114, 96, 0x012, PAL5, 0, OBJ_SMALL,
    130, 88, 0x011, PAL5, 0, OBJ_SMALL,
    122, 88, 0x010, PAL5, 0, OBJ_SMALL,
    114, 88, 0x007, PAL5, 0, OBJ_SMALL,
    138, 80, 0x006, PAL5, 0, OBJ_SMALL,
    130, 80, 0x005, PAL5, 0, OBJ_SMALL,
    122, 80, 0x004, PAL5, 0, OBJ_SMALL,
    130, 72, 0x003, PAL5, 0, OBJ_SMALL,
    122, 72, 0x002, PAL5, 0, OBJ_SMALL,
    114, 76, 0x001, PAL5, 0, OBJ_SMALL,
    122, 64, 0x000, PAL5, 0, OBJ_SMALL,
    0xFFFF
};

// GoalKeeperNoMove2

const u16 goalKeeperNoMoveSprites2[] = {
    134, 104, 0x017, PAL5, 0, OBJ_SMALL,
    126, 104, 0x016, PAL5, 0, OBJ_SMALL,
    118, 104, 0x015, PAL5, 0, OBJ_SMALL,
    141, 96, 0x014, PAL5, 0, OBJ_SMALL,
    133, 96, 0x013, PAL5, 0, OBJ_SMALL,
    114, 96, 0x012, PAL5, 0, OBJ_SMALL,
    133, 88, 0x011, PAL5, 0, OBJ_SMALL,
    125, 88, 0x010, PAL5, 0, OBJ_SMALL,
    117, 88, 0x007, PAL5, 0, OBJ_SMALL,
    136, 80, 0x006, PAL5, 0, OBJ_SMALL,
    128, 80, 0x005, PAL5, 0, OBJ_SMALL,
    120, 80, 0x004, PAL5, 0, OBJ_SMALL,
    133, 72, 0x003, PAL5, 0, OBJ_SMALL,
    125, 72, 0x002, PAL5, 0, OBJ_SMALL,
    117, 72, 0x001, PAL5, 0, OBJ_SMALL,
    125, 64, 0x000, PAL5, 0, OBJ_SMALL,
    0xFFFF
};

// GoalKeeperNoMove3

const u16 goalKeeperNoMoveSprites3[] = {
    126, 110, 0x016, PAL5, 0, OBJ_SMALL,
    134, 102, 0x015, PAL5, 0, OBJ_SMALL,
    126, 102, 0x014, PAL5, 0, OBJ_SMALL,
    118, 102, 0x013, PAL5, 0, OBJ_SMALL,
    136, 94, 0x012, PAL5, 0, OBJ_SMALL,
    128, 94, 0x011, PAL5, 0, OBJ_SMALL,
    115, 94, 0x010, PAL5, 0, OBJ_SMALL,
    133, 86, 0x007, PAL5, 0, OBJ_SMALL,
    125, 86, 0x006, PAL5, 0, OBJ_SMALL,
    117, 86, 0x005, PAL5, 0, OBJ_SMALL,
    128, 78, 0x004, PAL5, 0, OBJ_SMALL,
    120, 78, 0x003, PAL5, 0, OBJ_SMALL,
    128, 70, 0x002, PAL5, 0, OBJ_SMALL,
    120, 70, 0x001, PAL5, 0, OBJ_SMALL,
    128, 62, 0x000, PAL5, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

// GoalKeeperJumpLow1

const u16 goalKeeperJumpLowSprites1[] = {
    115, 102, 0x014, PAL5, 0, OBJ_SMALL,
    123, 102, 0x015, PAL5, 0, OBJ_SMALL,
    131, 102, 0x016, PAL5, 0, OBJ_SMALL,
    115, 70, 0x004, PAL5, 0, OBJ_SMALL,
    122, 86, 0x002, PAL5, 0, OBJ_LARGE,
    123, 70, 0x000, PAL5, 0, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

// GoalKeeperJumpLow2

const u16 goalKeeperJumpLowSprites2[] = {
    122, 81, 0x003, PAL5, 0, OBJ_SMALL,
    130, 81, 0x004, PAL5, 0, OBJ_SMALL,
    127, 89, 0x001, PAL5, 0, OBJ_LARGE,
    119, 89, 0x000, PAL5, 0, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

// GoalKeeperJumpLow3

const u16 goalKeeperJumpLowSprites3[] = {
    143, 89, 0x004, PAL5, 0, OBJ_SMALL,
    135, 98, 0x015, PAL5, 0, OBJ_SMALL,
    127, 98, 0x014, PAL5, 0, OBJ_SMALL,
    119, 98, 0x013, PAL5, 0, OBJ_SMALL,
    111, 94, 0x003, PAL5, 0, OBJ_SMALL,
    127, 82, 0x001, PAL5, 0, OBJ_LARGE,
    119, 82, 0x000, PAL5, 0, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

// GoalKeeperJumpLow4

const u16 goalKeeperJumpLowSprites4[] = {
    147, 93, 0x002, PAL5, 0, OBJ_SMALL,
    107, 94, 0x012, PAL5, 0, OBJ_SMALL,
    115, 94, 0x013, PAL5, 0, OBJ_SMALL,
    115, 86, 0x003, PAL5, 0, OBJ_SMALL,
    123, 86, 0x004, PAL5, 0, OBJ_SMALL,
    131, 85, 0x000, PAL5, 0, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

// GoalKeeperJumpLow5

const u16 goalKeeperJumpLowSprites5[] = {
    106, 88, 0x004, PAL5, 0, OBJ_SMALL,
    114, 88, 0x002, PAL5, 0, OBJ_LARGE,
    130, 94, 0x000, PAL5, 0, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

// GoalKeeperJumpLow6

const u16 goalKeeperJumpLowSprites6[] = {
    127, 93, 0x002, PAL5, 0, OBJ_LARGE,
    111, 86, 0x000, PAL5, 0, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

// GoalKeeperHappy1

const u16 goalKeeperSadSprites1[] = {
    127, 58, 0x006, PAL5, 0, OBJ_SMALL,
    119, 98, 0x004, PAL5, 0, OBJ_LARGE,
    119, 82, 0x002, PAL5, 0, OBJ_LARGE,
    119, 66, 0x000, PAL5, 0, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

// GoalKeeperHappy2

const u16 goalKeeperSadSprites2[] = {
    127, 57, 0x006, PAL5, 0, OBJ_SMALL,
    119, 97, 0x004, PAL5, 0, OBJ_LARGE,
    119, 81, 0x002, PAL5, 0, OBJ_LARGE,
    119, 65, 0x000, PAL5, 0, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

// GoalKeeperHappy3

const u16 goalKeeperSadSprites3[] = {
    127, 57, 0x006, PAL5, 0, OBJ_SMALL,
    119, 97, 0x004, PAL5, 0, OBJ_LARGE,
    119, 81, 0x002, PAL5, 0, OBJ_LARGE,
    119, 65, 0x000, PAL5, 0, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

// GoalKeeperHappyMirrored1

const u16 goalKeeperHappyMirroredSprites1[] = {
    164, 102, 0x005, PAL5, 1, OBJ_SMALL,
    156, 99, 0x006, PAL5, 1, OBJ_SMALL,
    156, 107, 0x015, PAL5, 1, OBJ_SMALL,
    148, 99, 0x014, PAL5, 1, OBJ_SMALL,
    145, 75, 0x004, PAL5, 1, OBJ_SMALL,
    149, 83, 0x002, PAL5, 1, OBJ_LARGE,
    153, 67, 0x000, PAL5, 1, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

// GoalKeeperHappyMirrored2

const u16 goalKeeperHappyMirroredSprites2[] = {
    147, 101, 0x015, PAL5, 1, OBJ_SMALL,
    155, 101, 0x005, PAL5, 1, OBJ_SMALL,
    163, 101, 0x014, PAL5, 1, OBJ_SMALL,
    147, 77, 0x004, PAL5, 1, OBJ_SMALL,
    148, 85, 0x002, PAL5, 1, OBJ_LARGE,
    155, 69, 0x000, PAL5, 1, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

// GoalKeeperHappyMirrored3

const u16 goalKeeperHappyMirroredSprites3[] = {
    158, 109, 0x015, PAL5, 1, OBJ_SMALL,
    148, 101, 0x005, PAL5, 1, OBJ_SMALL,
    156, 101, 0x014, PAL5, 1, OBJ_SMALL,
    158, 61, 0x004, PAL5, 1, OBJ_SMALL,
    148, 85, 0x002, PAL5, 1, OBJ_LARGE,
    150, 69, 0x000, PAL5, 1, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

// GoalKeeperHappy1

const u16 goalKeeperHappySprites1[] = {
    112, 102, 0x005, PAL5, 0, OBJ_SMALL,
    120, 99, 0x006, PAL5, 0, OBJ_SMALL,
    120, 107, 0x015, PAL5, 0, OBJ_SMALL,
    128, 99, 0x014, PAL5, 0, OBJ_SMALL,
    131, 75, 0x004, PAL5, 0, OBJ_SMALL,
    119, 83, 0x002, PAL5, 0, OBJ_LARGE,
    115, 67, 0x000, PAL5, 0, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

// GoalKeeperHappy2

const u16 goalKeeperHappySprites2[] = {
    129, 101, 0x015, PAL5, 0, OBJ_SMALL,
    121, 101, 0x005, PAL5, 0, OBJ_SMALL,
    113, 101, 0x014, PAL5, 0, OBJ_SMALL,
    129, 77, 0x004, PAL5, 0, OBJ_SMALL,
    120, 85, 0x002, PAL5, 0, OBJ_LARGE,
    113, 69, 0x000, PAL5, 0, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

// GoalKeeperHappy3

const u16 goalKeeperHappySprites3[] = {
    118, 109, 0x015, PAL5, 0, OBJ_SMALL,
    128, 101, 0x005, PAL5, 0, OBJ_SMALL,
    120, 101, 0x014, PAL5, 0, OBJ_SMALL,
    118, 61, 0x004, PAL5, 0, OBJ_SMALL,
    120, 85, 0x002, PAL5, 0, OBJ_LARGE,
    118, 69, 0x000, PAL5, 0, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

// GoalKeeperJumpHigh1

const u16 goalKeeperJumpHighSprites1[] = {
    136, 96, 0x005, PAL5, 1, OBJ_SMALL,
    128, 96, 0x006, PAL5, 1, OBJ_SMALL,
    120, 96, 0x007, PAL5, 1, OBJ_SMALL,
    136, 72, 0x004, PAL5, 1, OBJ_SMALL,
    120, 80, 0x002, PAL5, 1, OBJ_LARGE,
    120, 64, 0x000, PAL5, 1, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

// GoalKeeperJumpHigh2

const u16 goalKeeperJumpHighSprites2[] = {
    126, 101, 0x004, PAL5, 1, OBJ_SMALL,
    118, 101, 0x005, PAL5, 1, OBJ_SMALL,
    118, 85, 0x002, PAL5, 1, OBJ_LARGE,
    118, 69, 0x000, PAL5, 1, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

// GoalKeeperJumpHigh3

const u16 goalKeeperJumpHighSprites3[] = {
    109, 101, 0x007, PAL5, 1, OBJ_SMALL,
    106, 69, 0x006, PAL5, 1, OBJ_SMALL,
    117, 93, 0x004, PAL5, 1, OBJ_LARGE,
    117, 77, 0x002, PAL5, 1, OBJ_LARGE,
    114, 61, 0x000, PAL5, 1, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

// GoalKeeperJumpHigh4

const u16 goalKeeperJumpHighSprites4[] = {
    126, 93, 0x007, PAL5, 1, OBJ_SMALL,
    118, 85, 0x006, PAL5, 1, OBJ_SMALL,
    126, 85, 0x005, PAL5, 1, OBJ_SMALL,
    110, 69, 0x004, PAL5, 1, OBJ_SMALL,
    118, 69, 0x002, PAL5, 1, OBJ_LARGE,
    118, 53, 0x000, PAL5, 1, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

const s16 jumpHighXYvalues4[] = {
    0, -3, 
    0, -3, 
    0, -3, 
    0xFFF
};

// GoalKeeperJumpHigh5

const u16 goalKeeperJumpHighSprites5[] = {
    132, 86, 0x006, PAL5, 1, OBJ_SMALL,
    108, 81, 0x007, PAL5, 1, OBJ_SMALL,
    132, 78, 0x005, PAL5, 1, OBJ_SMALL,
    125, 46, 0x004, PAL5, 1, OBJ_SMALL,
    116, 70, 0x002, PAL5, 1, OBJ_LARGE,
    124, 54, 0x000, PAL5, 1, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
}; 

const s16 jumpHighXYvalues5[] = {
    0, -2, 
    0, -2, 
    0, -2, 
    0, -1, 
    0, -2, 
    0, -1, 
    0, 0, 
    0, -1, 
    0, 0, 
    0, -1, 
    0, 0, 
    0, 1, 
    0, 0, 
    0, 1, 
    0, 1, 
    0, 1, 
    0, 1, 
    0, 2, 
    0, 2, 
    0, 2, 
    0, 2, 
    0, 3, 
    0, 2, 
    0, 3, 
    0, 2, 
    0xFFF
};

// GoalKeeperPreJumpSideRight

const u16 goalKeeperPreJumpSideRight1[] = {
    133, 103, 0x016, PAL5, 1, OBJ_SMALL,
    132, 70, 0x007, PAL5, 1, OBJ_SMALL,
    140, 70, 0x006, PAL5, 1, OBJ_SMALL,
    117, 95, 0x004, PAL5, 1, OBJ_LARGE,
    117, 79, 0x002, PAL5, 1, OBJ_LARGE,
    116, 63, 0x000, PAL5, 1, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

const s16 jumpHighSideRightXYvalues1[] = {
    2, 0,
    2, 0,
    0xFFF
};

const u16 goalKeeperJumpHighSideSpritesRight2[] = {
    115, 98, 0x015, PAL5, 1, OBJ_SMALL,
    139, 98, 0x014, PAL5, 1, OBJ_SMALL,
    117, 82, 0x005, PAL5, 1, OBJ_SMALL,
    141, 86, 0x004, PAL5, 1, OBJ_SMALL,
    123, 90, 0x002, PAL5, 1, OBJ_LARGE,
    125, 74, 0x000, PAL5, 1, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

const s16 jumpHighSideRightXYvalues2[] = {
    2, 0,
    2, 0,
    0xFFF
};

const u16 goalKeeperJumpHighSideSpritesRight3[] = {
    131, 66, 0x001, PAL5, 1, OBJ_LARGE,
    123, 98, 0x007, PAL5, 1, OBJ_SMALL,
    131, 98, 0x006, PAL5, 1, OBJ_SMALL,
    131, 82, 0x003, PAL5, 1, OBJ_LARGE,
    139, 101, 0x017, PAL5, 1, OBJ_SMALL,
    147, 101, 0x016, PAL5, 1, OBJ_SMALL,
    123, 82, 0x004, PAL5, 1, OBJ_LARGE,
    139, 66, 0x000, PAL5, 1, OBJ_LARGE,
    0xFFFF
};

const s16 jumpHighSideRightXYvalues3[] = {
    2, 0,
    2, 1,
    2, 0,
    0xFFF
};

const u16 goalKeeperJumpHighSideSpritesRight4[] = {
    128, 91, 0x015, PAL5, 1, OBJ_SMALL,
    136, 91, 0x014, PAL5, 1, OBJ_SMALL,
    126, 83, 0x005, PAL5, 1, OBJ_SMALL,
    142, 67, 0x004, PAL5, 1, OBJ_SMALL,
    134, 75, 0x002, PAL5, 1, OBJ_LARGE,
    150, 67, 0x000, PAL5, 1, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

const s16 jumpHighSideRightXYvalues4[] = {
    2, -2,
    2, -2,
    2, -2,
    0xFFF
};

const u16 goalKeeperJumpHighSideSpritesRight5[] = {
    147, 61, 0x005, PAL5, 1, OBJ_SMALL,
    131, 73, 0x015, PAL5, 1, OBJ_SMALL,
    131, 81, 0x014, PAL5, 1, OBJ_SMALL,
    171, 56, 0x004, PAL5, 1, OBJ_SMALL,
    139, 69, 0x002, PAL5, 1, OBJ_LARGE,
    155, 60, 0x000, PAL5, 1, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

const s16 jumpHighSideRightXYvalues5[] = {
    2, -1,
    2, 0,
    2, -1,
    2, 0,
    2, -1,
    2, 0,
    2, 1,
    2, 0,
    2, 1,
    2, 1,
    2, 1,
    2, 2,
    2, 1,
    2, 2,
    0xFFF
};

const u16 goalKeeperJumpHighSideSpritesRight6[] = {
    160, 74, 0x004, PAL5, 1, OBJ_SMALL,
    160, 86, 0x014, PAL5, 1, OBJ_SMALL,
    189, 94, 0x015, PAL5, 1, OBJ_SMALL,
    200, 84, 0x005, PAL5, 1, OBJ_SMALL,
    168, 78, 0x002, PAL5, 1, OBJ_LARGE,
    184, 78, 0x000, PAL5, 1, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

const s16 jumpHighSideRightXYvalues6[] = {
    2, 1,
    0xFFF
};

const u16 goalKeeperJumpHighSideSpritesRight7[] = {
    207, 96, 0x005, PAL5, 1, OBJ_SMALL,
    167, 100, 0x014, PAL5, 1, OBJ_SMALL,
    167, 86, 0x004, PAL5, 1, OBJ_SMALL,
    175, 90, 0x002, PAL5, 1, OBJ_LARGE,
    191, 90, 0x000, PAL5, 1, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

// GoalKeeperJumpLowSideRight

const s16 jumpLowSideRightXYvalues1[] = {
    2, -2,
    2, -1,
    0xFFF
};

const u16 goalKeeperJumpLowSideSpritesRight2[] = {
    114, 90, 0x006, PAL5, 1, OBJ_SMALL,
    138, 95, 0x015, PAL5, 1, OBJ_SMALL,
    122, 77, 0x014, PAL5, 1, OBJ_SMALL,
    130, 69, 0x005, PAL5, 1, OBJ_SMALL,
    138, 69, 0x004, PAL5, 1, OBJ_SMALL,
    122, 85, 0x002, PAL5, 1, OBJ_LARGE,
    130, 77, 0x000, PAL5, 1, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

const s16 jumpLowSideRightXYvalues2[] = {
    3, 0,
    2, -1,
    0xFFF
};

// GoalKeeperJumpLowSideRight3

const u16 goalKeeperJumpLowSideSpritesRight3[] = {
    155, 90, 0x016, PAL5, 1, OBJ_SMALL,
    115, 89, 0x017, PAL5, 1, OBJ_SMALL,
    155, 82, 0x007, PAL5, 1, OBJ_SMALL,
    131, 75, 0x006, PAL5, 1, OBJ_SMALL,
    139, 87, 0x004, PAL5, 1, OBJ_LARGE,
    123, 83, 0x002, PAL5, 1, OBJ_LARGE,
    139, 71, 0x000, PAL5, 1, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

const s16 jumpLowSideRightXYvalues3[] = {
    2, 0,
    3, 2,
    2, 0,
    0xFFF
};

// GoalKeeperJumpLowSideRight4

const u16 goalKeeperJumpLowSideSpritesRight4[] = {
    127, 81, 0x014, PAL5, 1, OBJ_SMALL,
    127, 73, 0x005, PAL5, 1, OBJ_SMALL,
    167, 77, 0x004, PAL5, 1, OBJ_SMALL,
    135, 73, 0x002, PAL5, 1, OBJ_LARGE,
    151, 73, 0x000, PAL5, 1, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

const s16 jumpLowSideRightXYvalues4[] = {
    2, 1,
    3, 1,
    2, 1,
    2, 0,
    2, 0,
    0xFFF
};

// GoalKeeperJumpLowSideRight5

const u16 goalKeeperJumpLowSideSpritesRight5[] = {
    133, 85, 0x015, PAL5, 1, OBJ_SMALL,
    181, 88, 0x005, PAL5, 1, OBJ_SMALL,
    173, 88, 0x014, PAL5, 1, OBJ_SMALL,
    173, 80, 0x004, PAL5, 1, OBJ_SMALL,
    141, 77, 0x002, PAL5, 1, OBJ_LARGE,
    157, 80, 0x000, PAL5, 1, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

const s16 jumpLowSideRightXYvalues5[] = {
    2, 0,
    2, 0,
    2, 0,
    0xFFF
};

// GoalKeeperJumpLowSideRight6

const u16 goalKeeperJumpLowSideSpritesRight6[] = {
    190, 94, 0x005, PAL5, 1, OBJ_SMALL,
    150, 98, 0x014, PAL5, 1, OBJ_SMALL,
    150, 84, 0x004, PAL5, 1, OBJ_SMALL,
    158, 88, 0x002, PAL5, 1, OBJ_LARGE,
    174, 88, 0x000, PAL5, 1, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

const s16 jumpLowSideRightXYvalues6[] = {
    2, 0,
    2, 0,
    2, 0,
    2, 0,
    2, 0,
    1, 0,
    1, 0,
    2, 0,
    1, 0,
    0, 0,
    1, 0,
    0xFFF
};

// GoalKeeperJumpHighSideRightNotFar

const u16 goalKeeperPreJumpSideRightNotFar1[] = {
    133, 103, 0x016, PAL5, 1, OBJ_SMALL,
    132, 70, 0x007, PAL5, 1, OBJ_SMALL,
    140, 70, 0x006, PAL5, 1, OBJ_SMALL,
    117, 95, 0x004, PAL5, 1, OBJ_LARGE,
    117, 79, 0x002, PAL5, 1, OBJ_LARGE,
    116, 63, 0x000, PAL5, 1, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

const s16 jumpHighSideRightNotFarXYvalues1[] = {
    1, 0,
    1, 0,
    0xFFF
};

const u16 goalKeeperJumpHighSideSpritesRightNotFar2[] = {
    112, 98, 0x015, PAL5, 1, OBJ_SMALL,
    136, 98, 0x014, PAL5, 1, OBJ_SMALL,
    114, 82, 0x005, PAL5, 1, OBJ_SMALL,
    136, 86, 0x004, PAL5, 1, OBJ_SMALL,
    120, 90, 0x002, PAL5, 1, OBJ_LARGE,
    122, 74, 0x000, PAL5, 1, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

const s16 jumpHighSideRightNotFarXYvalues2[] = {
    1, 0,
    1, 0,
    0xFFF
};

const u16 goalKeeperJumpHighSideSpritesRightNotFar3[] = {
    125, 66, 0x001, PAL5, 1, OBJ_LARGE,
    117, 98, 0x007, PAL5, 1, OBJ_SMALL,
    125, 98, 0x006, PAL5, 1, OBJ_SMALL,
    125, 82, 0x003, PAL5, 1, OBJ_LARGE,
    133, 101, 0x017, PAL5, 1, OBJ_SMALL,
    141, 101, 0x016, PAL5, 1, OBJ_SMALL,
    117, 82, 0x004, PAL5, 1, OBJ_LARGE,
    133, 66, 0x000, PAL5, 1, OBJ_LARGE,
    0xFFFF
};

const s16 jumpHighSideRightNotFarXYvalues3[] = {
    1, 0,
    1, 0,
    1, 0,
    0xFFF
};

const u16 goalKeeperJumpHighSideSpritesRightNotFar4[] = {
    118, 90, 0x015, PAL5, 1, OBJ_SMALL,
    126, 90, 0x014, PAL5, 1, OBJ_SMALL,
    116, 82, 0x005, PAL5, 1, OBJ_SMALL,
    132, 66, 0x004, PAL5, 1, OBJ_SMALL,
    124, 74, 0x002, PAL5, 1, OBJ_LARGE,
    140, 66, 0x000, PAL5, 1, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

const s16 jumpHighSideRightNotFarXYvalues4[] = {
    1, -3,
    1, -2,
    1, -2,
    0xFFF
};

const u16 goalKeeperJumpHighSideSpritesRightNotFar5[] = {
    133, 58, 0x005, PAL5, 1, OBJ_SMALL,
    117, 70, 0x015, PAL5, 1, OBJ_SMALL,
    117, 78, 0x014, PAL5, 1, OBJ_SMALL,
    157, 53, 0x004, PAL5, 1, OBJ_SMALL,
    125, 66, 0x002, PAL5, 1, OBJ_LARGE,
    141, 57, 0x000, PAL5, 1, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

const s16 jumpHighSideRightNotFarXYvalues5[] = {
    1, -2,
    1, 0,
    1, -1,
    1, -1,
    1, -1,
    1, -1,
    1, 0,
    1, 0,
    1, 0,
    1, 0,
    1, 1,
    1, 1,
    1, 1,
    1, 1,
    0xFFF
};

const u16 goalKeeperJumpHighSideSpritesRightNotFar6[] = {
    131, 62, 0x004, PAL5, 1, OBJ_SMALL,
    131, 74, 0x014, PAL5, 1, OBJ_SMALL,
    160, 82, 0x015, PAL5, 1, OBJ_SMALL,
    171, 72, 0x005, PAL5, 1, OBJ_SMALL,
    139, 66, 0x002, PAL5, 1, OBJ_LARGE,
    155, 66, 0x000, PAL5, 1, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

const s16 jumpHighSideRightNotFarXYvalues6[] = {
    1, 2, 
    1, 2, 
    1, 3,
    1, 2, 
    1, 3, 
    0xFFF
};

const u16 goalKeeperJumpHighSideSpritesRightNotFar7[] = {
    180, 95, 0x005, PAL5, 1, OBJ_SMALL,
    140, 99, 0x014, PAL5, 1, OBJ_SMALL,
    140, 85, 0x004, PAL5, 1, OBJ_SMALL,
    148, 89, 0x002, PAL5, 1, OBJ_LARGE,
    164, 89, 0x000, PAL5, 1, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

// GoalKeeperJumpHighSideLeft1

const u16 goalKeeperJumpHighSideSpritesLeft1[] = {
    111, 103, 0x016, PAL5, 0, OBJ_SMALL,
    112, 70, 0x007, PAL5, 0, OBJ_SMALL,
    104, 70, 0x006, PAL5, 0, OBJ_SMALL,
    119, 95, 0x004, PAL5, 0, OBJ_LARGE,
    119, 79, 0x002, PAL5, 0, OBJ_LARGE,
    120, 63, 0x000, PAL5, 0, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

// GoalKeeperJumpHighSide2

const u16 goalKeeperJumpHighSideSprites2[] = {
    133, 98, 0x015, PAL5, 0, OBJ_SMALL,
    109, 98, 0x014, PAL5, 0, OBJ_SMALL,
    131, 82, 0x005, PAL5, 0, OBJ_SMALL,
    107, 86, 0x004, PAL5, 0, OBJ_LARGE,
    117, 90, 0x002, PAL5, 0, OBJ_LARGE,
    115, 74, 0x000, PAL5, 0, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

const s16 jumpHighSideXYvalues2[] = {
    -2, 0,
    -2, 0,
    0xFFF
};

// GoalKeeperJumpHighSide3

const u16 goalKeeperJumpHighSideSprites3[] = {
    109, 66, 0x001, PAL5, 0, OBJ_LARGE,
    125, 98, 0x007, PAL5, 0, OBJ_SMALL,
    117, 98, 0x006, PAL5, 0, OBJ_SMALL,
    109, 82, 0x003, PAL5, 0, OBJ_LARGE,
    109, 101, 0x017, PAL5, 0, OBJ_SMALL,
    101, 101, 0x016, PAL5, 0, OBJ_SMALL,
    117, 82, 0x004, PAL5, 0, OBJ_LARGE,
    101, 66, 0x000, PAL5, 0, OBJ_LARGE,
    0xFFFF
};

const s16 jumpHighSideXYvalues3[] = {
    -2, 0,
    -2, 1,
    -2, 0,
    0xFFF
};

// GoalKeeperJumpHighSide4

const u16 goalKeeperJumpHighSideSprites4[] = {
    120, 91, 0x015, PAL5, 0, OBJ_SMALL,
    112, 91, 0x014, PAL5, 0, OBJ_SMALL,
    122, 83, 0x005, PAL5, 0, OBJ_SMALL,
    106, 67, 0x004, PAL5, 0, OBJ_SMALL,
    106, 75, 0x002, PAL5, 0, OBJ_LARGE,
    90, 67, 0x000, PAL5, 0, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

const s16 jumpHighSideXYvalues4[] = {
    -2, -2,
    -2, -2,
    -2, -2,
    0xFFF
};

// GoalKeeperJumpHighSide5

const u16 goalKeeperJumpHighSideSprites5[] = {
    101, 61, 0x005, PAL5, 0, OBJ_SMALL,
    117, 73, 0x015, PAL5, 0, OBJ_SMALL,
    117, 81, 0x014, PAL5, 0, OBJ_SMALL,
    77, 56, 0x004, PAL5, 0, OBJ_SMALL,
    101, 69, 0x002, PAL5, 0, OBJ_LARGE,
    85, 60, 0x000, PAL5, 0, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

const s16 jumpHighSideXYvalues5[] = {
    -2, -1,
    -2, 0,
    -2, -1,
    -2, 0,
    -2, -1,
    -2, 0,
    -2, 1,
    -2, 0,
    -2, 1,
    -2, 1,
    -2, 1,
    -2, 2,
    -2, 1,
    -2, 2,
    0xFFF
};

// GoalKeeperJumpHighSide6

const u16 goalKeeperJumpHighSideSprites6[] = {
    88, 74, 0x004, PAL5, 0, OBJ_SMALL,
    88, 86, 0x014, PAL5, 0, OBJ_SMALL,
    59, 94, 0x015, PAL5, 0, OBJ_SMALL,
    48, 84, 0x005, PAL5, 0, OBJ_SMALL,
    72, 78, 0x002, PAL5, 0, OBJ_LARGE,
    56, 78, 0x000, PAL5, 0, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

const s16 jumpHighSideXYvalues6[] = {
    -2, 1,
    0xFFF
};

// GoalKeeperJumpHighSide7

const u16 goalKeeperJumpHighSideSprites7[] = {
    41, 96, 0x005, PAL5, 0, OBJ_SMALL,
    81, 100, 0x014, PAL5, 0, OBJ_SMALL,
    81, 86, 0x004, PAL5, 0, OBJ_SMALL,
    65, 90, 0x002, PAL5, 0, OBJ_LARGE,
    49, 90, 0x000, PAL5, 0, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

// GoalKeeperJumpLowLeftSideNotFar

const u16 goalKeeperJumpLowLeftSideNotFarSprites1[] = {
    113, 102, 0x016, PAL5, 0, OBJ_SMALL,
    114, 69, 0x007, PAL5, 0, OBJ_SMALL,
    106, 69, 0x006, PAL5, 0, OBJ_SMALL,
    121, 94, 0x004, PAL5, 0, OBJ_LARGE,
    121, 78, 0x002, PAL5, 0, OBJ_LARGE,
    122, 62, 0x000, PAL5, 0, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

const s16 jumpLowLeftSideNotFarXYvalues1[] = {
    -1, -1, 
    0xFFF
};

const u16 goalKeeperJumpLowLeftSideNotFarSprites2[] = {
    136, 91, 0x006, PAL5, 0, OBJ_SMALL,
    112, 96, 0x015, PAL5, 0, OBJ_SMALL,
    128, 78, 0x014, PAL5, 0, OBJ_SMALL,
    120, 70, 0x005, PAL5, 0, OBJ_SMALL,
    112, 70, 0x004, PAL5, 0, OBJ_SMALL,
    120, 86, 0x002, PAL5, 0, OBJ_LARGE,
    112, 78, 0x000, PAL5, 0, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

const s16 jumpLowLeftSideNotFarXYvalues2[] = {
    -1, 0,
    -2, 0,
    0xFFF
};

const u16 goalKeeperJumpLowLeftSideNotFarSprites3[] = {
    98, 92, 0x016, PAL5, 0, OBJ_SMALL,
    138, 91, 0x017, PAL5, 0, OBJ_SMALL,
    98, 84, 0x007, PAL5, 0, OBJ_SMALL,
    122, 77, 0x006, PAL5, 0, OBJ_SMALL,
    106, 89, 0x004, PAL5, 0, OBJ_LARGE,
    122, 85, 0x002, PAL5, 0, OBJ_LARGE,
    106, 73, 0x000, PAL5, 0, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

const s16 jumpLowLeftSideNotFarXYvalues3[] = {
    -1, 0,
    -1, 1,
    -2, 0,
    0xFFF
};

const u16 goalKeeperJumpLowLeftSideNotFarSprites4[] = {
    130, 82, 0x014, PAL5, 0, OBJ_SMALL,
    130, 74, 0x005, PAL5, 0, OBJ_SMALL,
    90, 78, 0x004, PAL5, 0, OBJ_SMALL,
    114, 74, 0x002, PAL5, 0, OBJ_LARGE,
    98, 74, 0x000, PAL5, 0, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

const s16 jumpLowLeftSideNotFarXYvalues4[] = {
    -1, 1,
    -1, 0,
    -2, 0,
    0xFFF
};

const u16 goalKeeperJumpLowLeftSideNotFarSprites5[] = {
    133, 84, 0x015, PAL5, 0, OBJ_SMALL,
    85, 87, 0x005, PAL5, 0, OBJ_SMALL,
    93, 87, 0x014, PAL5, 0, OBJ_SMALL,
    93, 79, 0x004, PAL5, 0, OBJ_SMALL,
    117, 76, 0x002, PAL5, 0, OBJ_LARGE,
    101, 79, 0x000, PAL5, 0, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

const s16 jumpLowLeftSideNotFarXYvalues5[] = {
    -1, 0,
    -1, 0,
    -2, 0,
    0xFFF
};

const u16 goalKeeperJumpLowLeftSideNotFarSprites6[] = {
    80, 93, 0x005, PAL5, 0, OBJ_SMALL,
    120, 97, 0x014, PAL5, 0, OBJ_SMALL,
    120, 83, 0x004, PAL5, 0, OBJ_SMALL,
    104, 87, 0x002, PAL5, 0, OBJ_LARGE,
    88, 87, 0x000, PAL5, 0, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

const s16 jumpLowLeftSideNotFarXYvalues6[] = {
    -1, 0,
    -1, 0,
    -2, 0,
    0, 0,
    -1, 0,
    0xFFF
};

// GoalKeeperJumpLowLeftSide

const u16 goalKeeperJumpLowLeftSideSprites1[] = {
    115, 103, 0x016, PAL5, 0, OBJ_SMALL,
    116, 70, 0x007, PAL5, 0, OBJ_SMALL,
    108, 70, 0x006, PAL5, 0, OBJ_SMALL,
    123, 95, 0x004, PAL5, 0, OBJ_LARGE,
    123, 79, 0x002, PAL5, 0, OBJ_LARGE,
    124, 63, 0x000, PAL5, 0, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

const s16 jumpLowLeftSideXYvalues1[] = {
    -3, -2, 
    -2, -1,
    0xFFF
};

const u16 goalKeeperJumpLowLeftSideSprites2[] = {
    133, 90, 0x006, PAL5, 0, OBJ_SMALL,
    109, 95, 0x015, PAL5, 0, OBJ_SMALL,
    125, 77, 0x014, PAL5, 0, OBJ_SMALL,
    117, 69, 0x005, PAL5, 0, OBJ_SMALL,
    109, 69, 0x004, PAL5, 0, OBJ_SMALL,
    117, 85, 0x002, PAL5, 0, OBJ_LARGE,
    109, 77, 0x000, PAL5, 0, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

const s16 jumpLowLeftSideXYvalues2[] = {
    -2, 0,
    -3, -1,
    0xFFF
};

const u16 goalKeeperJumpLowLeftSideSprites3[] = {
    92, 90, 0x016, PAL5, 0, OBJ_SMALL,
    132, 89, 0x017, PAL5, 0, OBJ_SMALL,
    92, 82, 0x007, PAL5, 0, OBJ_SMALL,
    116, 75, 0x006, PAL5, 0, OBJ_SMALL,
    100, 87, 0x004, PAL5, 0, OBJ_LARGE,
    116, 83, 0x002, PAL5, 0, OBJ_LARGE,
    100, 71, 0x000, PAL5, 0, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

const s16 jumpLowLeftSideXYvalues3[] = {
    -2, 0,
    -2, 2,
    -3, 0,
    0xFFF
};

const u16 goalKeeperJumpLowLeftSideSprites4[] = {
    120, 81, 0x014, PAL5, 0, OBJ_SMALL,
    120, 73, 0x005, PAL5, 0, OBJ_SMALL,
    80, 77, 0x004, PAL5, 0, OBJ_SMALL,
    104, 73, 0x002, PAL5, 0, OBJ_LARGE,
    88, 73, 0x000, PAL5, 0, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

const s16 jumpLowLeftSideXYvalues4[] = {
    -2, 1,
    -2, 1,
    -3, 1,
    -2, 0,
    -2, 0,
    0xFFF
};

const u16 goalKeeperJumpLowLeftSideSprites5[] = {
    115, 85, 0x015, PAL5, 0, OBJ_SMALL,
    67, 88, 0x005, PAL5, 0, OBJ_SMALL,
    75, 88, 0x014, PAL5, 0, OBJ_SMALL,
    75, 80, 0x004, PAL5, 0, OBJ_SMALL,
    99, 77, 0x002, PAL5, 0, OBJ_LARGE,
    83, 80, 0x000, PAL5, 0, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

const s16 jumpLowLeftSideXYvalues5[] = {
    -3, 0,
    -2, 0,
    -2, 0,
    0xFFF
};

const u16 goalKeeperJumpLowLeftSideSprites6[] = {
    58, 94, 0x005, PAL5, 0, OBJ_SMALL,
    98, 98, 0x014, PAL5, 0, OBJ_SMALL,
    98, 84, 0x004, PAL5, 0, OBJ_SMALL,
    82, 88, 0x002, PAL5, 0, OBJ_LARGE,
    66, 88, 0x000, PAL5, 0, OBJ_LARGE,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0, 0, 0x06F, PAL0, 0, OBJ_SMALL,
    0xFFFF
};

const s16 jumpLowLeftSideXYvalues6[] = {
    -3, 0,
    -2, 0,
    -2, 0,
    -2, 0,
    -1, 0,
    -2, 0,
    -1, 0,
    0, 0,
    -1, 0,
    0xFFF
};
