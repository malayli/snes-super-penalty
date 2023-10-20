#include <snes.h>
#include "oam.h"

u16 stopLoop;
u8 spriteObjIndex;
u16 oamId;
u8 prio;
u16 *selectedSprites;
s16 offsetX;
s16 offsetY;
u8 spritesCount;
s16 checkX;

/*!\brief Set a sprite in the OAM.
*/
void oamSetSprite() {
    oamSet(oamId, 
        selectedSprites[0], 
        selectedSprites[1], 
        prio, 
        selectedSprites[4], 
        0, 
        selectedSprites[2], 
        selectedSprites[3]);
    oamSetEx(oamId, selectedSprites[5], OBJ_SHOW);
        
    oamId += 4;
}

/*!\brief Set a sprite in the OAM by inverting X position.
*/
void oamSetSpriteInvertX() {
    checkX = 256 - selectedSprites[0] - 16;
    if (checkX < 0) {
        return;
    }

    oamSet(oamId, 
        checkX, 
        selectedSprites[1], 
        prio, 
        selectedSprites[4], 
        0, 
        selectedSprites[2], 
        selectedSprites[3]);
    oamSetEx(oamId, selectedSprites[5], OBJ_SHOW);
        
    oamId += 4;
}

/*!\brief Set a sprite {X,Y} position in the OAM.
*/
void oamSetSpriteXYEx() {
    oamSetXY(oamId, 
        selectedSprites[0] + offsetX, 
        selectedSprites[1] + offsetY);
    oamId += 4;
}

/*!\brief Set a sprite {X,Y} position in the OAM by inverting the X position.
*/
void oamSetSpriteXYInvertXEx() {
    oamSetXY(oamId, 
        256 - (selectedSprites[0] + offsetX) - 16, 
        selectedSprites[1] + offsetY);
    oamId += 4;
}

/*!\brief Set a metasprite {X,Y} position in the OAM.
*/
void oamSetMetaSpriteXYEx() {
    spriteObjIndex = 0;
    stopLoop = 0;
    while(stopLoop != 0xFFFF) {
        oamSetXY(oamId, 
            selectedSprites[spriteObjIndex] + offsetX, 
            selectedSprites[spriteObjIndex + 1] + offsetY);
        stopLoop = selectedSprites[spriteObjIndex + 6];
        oamId += 4;
        spriteObjIndex += 6;
    }
}

/*!\brief Set a metasprite {X,Y} position in the OAM by inverting the X position.
*/
void oamSetMetaSpriteXYInvertXEx() {
    spriteObjIndex = 0;
    stopLoop = 0;
    while(stopLoop != 0xFFFF) {
        oamSetXY(oamId, 
            256 - (selectedSprites[spriteObjIndex] + offsetX) - 8, 
            selectedSprites[spriteObjIndex + 1] + offsetY);
        stopLoop = selectedSprites[spriteObjIndex + 6];
        oamId += 4;
        spriteObjIndex += 6;
    }
}

/*!\brief Set a metasprite in the OAM.
*/
void oamSetMetaSprite() {
    spritesCount = 0;
    spriteObjIndex = 0;
    stopLoop = 0;
    while(stopLoop != 0xFFFF) {
        oamSet(oamId, 
            selectedSprites[spriteObjIndex], 
            selectedSprites[spriteObjIndex + 1], 
            prio, 
            selectedSprites[spriteObjIndex + 4], 
            0, 
            selectedSprites[spriteObjIndex + 2], 
            selectedSprites[spriteObjIndex + 3]);
        oamSetEx(oamId, selectedSprites[spriteObjIndex + 5], OBJ_SHOW);
        stopLoop = selectedSprites[spriteObjIndex + 6];
        oamId += 4;
        spritesCount += 1;
        spriteObjIndex += 6;
    }
}

/*!\brief Set a metasprite in the OAM by setting X and Y offsets.
*/
void oamSetMetaSpriteEx() {
    spriteObjIndex = 0;
    stopLoop = 0;
    while(stopLoop != 0xFFFF) {
        oamSet(oamId, 
            selectedSprites[spriteObjIndex] + offsetX, 
            selectedSprites[spriteObjIndex + 1] + offsetY, 
            prio, 
            selectedSprites[spriteObjIndex + 4], 
            0, 
            selectedSprites[spriteObjIndex + 2], 
            selectedSprites[spriteObjIndex + 3]);
        oamSetEx(oamId, selectedSprites[spriteObjIndex + 5], OBJ_SHOW);
        stopLoop = selectedSprites[spriteObjIndex + 6];
        oamId += 4;
        spriteObjIndex += 6;
    }
}
