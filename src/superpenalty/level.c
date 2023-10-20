#include <snes.h>
#include "../common/bgHud.h"
#include "database.h"
#include "level.h"
#include "home.h"
#include "data.h"
#include "tables.h"
#include "striker.h"
#include "goalkeeper.h"
#include "ball.h"
#include "tilemaps.h"
#include "tilemaps2.h"
#include "../common/oam.h"
#include "colors.h"
#include "../common/gameState.h"
#include "../common/utils.h"
#include "../common/window.h"
#include "../common/bgText.h"
#include "../common/spriteEngine.h"
#include "../common/palette.h"
#include "../common/input.h"
#include "../common/variables.h"
#include "../soundbank.h"

// Types

typedef enum StrikerState {
    StrikerStateIdle = 0, 
    StrikerStatePreShoot,
    StrikerStateShootLow,
    StrikerStateShootHigh,
    StrikerStatePostShoot,
    StrikerStateWait,
    StrikerStateHappy,
    StrikerStateSad,
    StrikerStateEnd
} StrikerState;

typedef enum BallState {
    BallStateIdle = 0, 

    BallStatePreMoveLow,
    BallStateMoveLowNotBlocked,
    BallStateMoveLowBlocked,

    BallStatePreMoveLowRight,
    BallStateMoveLowRightNotBlocked,
    BallStateMoveLowRightBlocked,

    BallStatePreMoveLowLeft,
    BallStateMoveLowLeftNotBlocked,
    BallStateMoveLowLeftBlocked,

    BallStatePreMoveHigh,
    BallStateMoveHighNotBlocked,
    BallStateMoveHighBlocked,

    BallStatePreMoveHighRight,
    BallStateMoveHighRightNotBlocked,
    BallStateMoveHighRightBlocked,

    BallStatePreMoveHighLeft,
    BallStateMoveHighLeftNotBlocked,
    BallStateMoveHighLeftBlocked,

    BallStateEnd
} BallState;

typedef enum GoalkeeperState {
    GoalkeeperStateIdle = 0, 

    GoalkeeperStateNoMove,

    GoalkeeperStateJumpLow,
    GoalkeeperStateJumpLowSideLeft,
    GoalkeeperStateJumpLowSideRight,

    GoalkeeperStateJumpHigh,
    GoalkeeperStateJumpHighSideLeft,
    GoalkeeperStateJumpHighSideRight,

    GoalkeeperStateJumpLowSideLeftNotFar,
    GoalkeeperStateJumpHighSideRightNotFar,

    GoalkeeperStateWait,
    GoalkeeperStateReact,
    GoalkeeperStateHappy,
    GoalkeeperStateHappyMirrored,
    GoalkeeperStateSad,
    GoalkeeperStateBackToFloor,
    GoalkeeperStateEnd
} GoalkeeperState;

typedef enum GoalkeeperJumpState {
    GoalkeeperJumpStateNone = 0, 

    // High
    GoalkeeperJumpHigh4State,
    GoalkeeperJumpHigh5State,

    // Low Side Right
    GoalkeeperJumpLowSideRight1State,
    GoalkeeperJumpLowSideRight2State,
    GoalkeeperJumpLowSideRight3State,
    GoalkeeperJumpLowSideRight4State,
    GoalkeeperJumpLowSideRight5State,
    GoalkeeperJumpLowSideRight6State,

    // Low Side Left
    GoalkeeperJumpLowSide1State,
    GoalkeeperJumpLowSide2State,
    GoalkeeperJumpLowSide3State,
    GoalkeeperJumpLowSide4State,
    GoalkeeperJumpLowSide5State,
    GoalkeeperJumpLowSide6State,

    // High Side Right
    GoalkeeperJumpHighSideRight1State,
    GoalkeeperJumpHighSideRight2State,
    GoalkeeperJumpHighSideRight3State,
    GoalkeeperJumpHighSideRight4State,
    GoalkeeperJumpHighSideRight5State,
    GoalkeeperJumpHighSideRight6State,

    // High Side Left
    GoalkeeperJumpHighSide2State,
    GoalkeeperJumpHighSide3State,
    GoalkeeperJumpHighSide4State,
    GoalkeeperJumpHighSide5State,
    GoalkeeperJumpHighSide6State
} GoalkeeperJumpState;

typedef enum LoadState {
    LoadStateNone = 0,
    LoadStateStart,
    LoadStateDo
} LoadState;

// RAM

LevelState levelState;
StrikerState strikerState;
StrikerState strikerStatePost;
BallState ballState;
BallState ballStateSelectedByCpu;
GoalkeeperState previousGoalkeeperState;
GoalkeeperState goalkeeperState;
LoadState loadState;
u8 loadFrameCounter;
u8 strikerFramesCounter;
u8 goalkeeperFramesCounter;
u8 manFramesCounter;
u8 bigFlagFramesCounter;
u8 littleFlagFramesCounter;
u8 isCpuStriker;
u8 country1Palette, country2Palette;
u16 gkX, gkY;
s16 gkOffsetX, gkOffsetY;
u8 offsetIndex;
GoalkeeperJumpState goalkeeperJumpState;
s16 ballshadowOffsetX;
s16 ballshadowOffsetY;
u16 stopBallOffset;
s16 stopBallshadowOffset;
u16 ballOffsetIndex;
u8 ballshadowOffsetIndex;
u16 ballOamId;
u16 goalkeeperOamId;
u8 zOrder;
u8 goalkeeperSpritesCount;
u8 showTinyShadow;
u8 loadSprite;
u8 animNet;
u8 displayBlueCircle;
u8 netFramesCount;
u8 enableGoalKeeperInput;
u8 circleFrameCounter;
u8 circleBlinkCounter;
u8 shootingCountry1Round, shootingCountry2Round;
u16 ballGfxOffset;
u8 ballGfxOffsetCounter;
CountryType countryTmptype;
CountryType country1type;
CountryType country2type;
CountryType countryOfStrikertype;
CountryType countryOfGoalkeepertype;
char *flagPicPtr;
char *flagPalettePtr;
u16 randomizedState;
u16 ballX, ballY;
u16 ballCountry1StartOamId, ballCountry2StartOamId, selectedBallCountryStartOamId;
u16 goalUpAddress;
u16 goalDownAddress;
u16 goalAddressOffset;

/*!\brief The score state (0: not shooted, 1: scored, 2: failed to score)
*/
u8 scoreState;
u8 score1, score2;
u8 ballsCounter;
u8 scoreStates1[] = { 0, 0, 0, 0, 0 };
u8 scoreStates2[] = { 0, 0, 0, 0, 0 };
u8 scoreStateTmp;
u8 scoreOffset;
u8 spriteObjIndex2;
u8 remainingShoots1, remainingShoots2;

u8 numberIndex1, numberIndex2;

char *strikersName1[5];
char *strikersName2[5];
char *goalkeeperName1;
char *goalkeeperName2;

// Functions

void initLevel();

/*!\brief Load the level music.
*/
void initLevelMusic() {
    spcSetBank(&SOUNDBANK__);
    spcLoad(MOD_MUSIC);
}

/*!\brief Copy a palette to foreground CGRam.
    \param source the palette source to copy
    \param paletteNumber the palette number
*/
void initForegroundPalette(u8 *source, u16 paletteNumber) {
    dmaCopyCGram(source, 128 + (paletteNumber<<4), 32);
}

/*!\brief Set the country flag palette.
*/
void setCountryFlagPalette(CountryType type, u8 paletteNumber) {
    switch(type) {
        case CountryTypeBrazil:
            flagPalettePtr = &flagbrazilPalette;
            break;
        case CountryTypeColombia:
            flagPalettePtr = &flagcolombiaPalette;
            break;
        case CountryTypeEngland:
            flagPalettePtr = &flagenglandPalette;
            break;
        case CountryTypeFrance:
            flagPalettePtr = &flagfrancePalette;
            break;
    }
    initForegroundPalette(flagPalettePtr, paletteNumber);
}

/*!\brief Set the striker palette.
*/
void setStrikerPalette() {
    switch(countryOfStrikertype) {
        case CountryTypeBrazil:
            initForegroundPalette(&strikerBrazilPalette, PAL2);
            break;
        case CountryTypeColombia:
            initForegroundPalette(&strikerColombiaPalette, PAL2);
            break;
        case CountryTypeEngland:
            initForegroundPalette(&strikerEnglandPalette, PAL2);
            break;
        case CountryTypeFrance:
            initForegroundPalette(&strikerFrancePalette, PAL2);
            break;
    }
}

/*!\brief Set the country 1.
*/
void setCountry1() {
    switch(country1type) {
        case CountryTypeBrazil:
            titleX = 52;
            flagPicPtr = &flagbrazilPic;
            break;
        case CountryTypeColombia:
            titleX = 44;
            flagPicPtr = &flagcolombiaPic;
            break;
        case CountryTypeEngland:
            titleX = 48;
            flagPicPtr = &flagenglandPic;
            break;
        case CountryTypeFrance:
            titleX = 52;
            flagPicPtr = &flagfrancePic;
            break;
    }
    oamSetString(countriesName[country1type]);
    dmaCopyVram(flagPicPtr, 0x78A0, 96);
    dmaCopyVram(flagPicPtr + 96, 0x79A0, 96);
}

/*!\brief Set the country 2.
*/
void setCountry2() {
    switch(country2type) {
        case CountryTypeBrazil:
            titleX = 180;
            flagPicPtr = &flagbrazilPic;
            break;
        case CountryTypeColombia:
            titleX = 172;
            flagPicPtr = &flagcolombiaPic;
            break;
        case CountryTypeEngland:
            titleX = 176;
            flagPicPtr = &flagenglandPic;
            break;
        case CountryTypeFrance:
            titleX = 176;
            flagPicPtr = &flagfrancePic;
            break;
    }
    oamSetString(countriesName[country2type]);
    dmaCopyVram(flagPicPtr, 0x78D0, 96);
    dmaCopyVram(flagPicPtr + 96, 0x79D0, 96);
}

/*!\brief Initialize the level session.
*/
void initLevelSession(CountryType country1, CountryType country2, u8 ballsCount, LevelState state) {
    isCpuStriker = 0;
    goalAddressOffset = 0;
    countryOfStrikertype = country1;
    countryOfGoalkeepertype = country2;
    country1type = country1;
    country2type = country2;
    shootingCountry1Round = 0;
    shootingCountry2Round = 0;
    score1 = 0;
    score2 = 0;
    ballsCounter = ballsCount;
    remainingShoots1 = ballsCount;
    remainingShoots2 = ballsCount;
    levelState = state;
    numberIndex1 = country1type * 12;
    numberIndex2 = country2type * 12;
    initLevel();
}

/*!\brief Show country 1's balls.
*/
void showCountry1Balls(u8 ballsCount) {
    spriteObjIndex = shootingCountry1Round;
    ballCountry1StartOamId = oamIdOffset;
    while(spriteObjIndex < ballsCount) {
        oamSet(oamIdOffset, 24 + (spriteObjIndex<<4), 200, prio, 0, 0, 0x144, PAL7); oamSetEx(oamIdOffset, OBJ_LARGE, OBJ_SHOW); oamIdOffset += 4;
        spriteObjIndex++;
    }

    spriteObjIndex = 0;
    spriteObjIndex2 = 0;
    scoreAddressUp = 0x1000 + 803;
    scoreAddressDown = 0x1000 + 835;
    while(spriteObjIndex < shootingCountry1Round) {
        scoreStateTmp = scoreStates1[spriteObjIndex];
        scoreOffset = spriteObjIndex << 1;

        if (scoreStateTmp == 1) {
            scoreTileMapUp[spriteObjIndex2] = 0x182 | (0 << 14) | TPAL2 | (1<<13);
            scoreTileMapUp[spriteObjIndex2 + 1] = 0x182 | (1 << 14) | TPAL2 | (1<<13);
            
            scoreTileMapDown[spriteObjIndex2] = 0x183 | (0 << 14) | TPAL2 | (1<<13);
            scoreTileMapDown[spriteObjIndex2 + 1] = 0x183 | (1 << 14) | TPAL2 | (1<<13);

        } else if (scoreStateTmp == 2) {
            scoreTileMapUp[spriteObjIndex2] = 0x180 | (0 << 14) | TPAL3 | (1<<13);
            scoreTileMapUp[spriteObjIndex2 + 1] = 0x180 | (1 << 14) | TPAL3 | (1<<13);
            
            scoreTileMapDown[spriteObjIndex2] = 0x181 | (0 << 14) | TPAL3 | (1<<13);
            scoreTileMapDown[spriteObjIndex2 + 1] = 0x181 | (1 << 14) | TPAL3 | (1<<13);
        }

        spriteObjIndex++;
        spriteObjIndex2 += 2;
    }
    scoreTileMapSize = spriteObjIndex<<2;
    refreshScore = (0 < shootingCountry1Round);
}

/*!\brief Show country 2's balls.
*/
void showCountry2Balls(u8 ballsCount) {
    spriteObjIndex = shootingCountry2Round;
    ballCountry2StartOamId = oamIdOffset;
    while(spriteObjIndex < ballsCount) {
        oamSet(oamIdOffset, 160 + (spriteObjIndex<<4), 200, prio, 0, 0, 0x144, PAL7); oamSetEx(oamIdOffset, OBJ_LARGE, OBJ_SHOW); oamIdOffset += 4;
        spriteObjIndex++;
    }

    spriteObjIndex = 0;
    spriteObjIndex2 = 0;
    score2AddressUp = 0x1000 + 820;
    score2AddressDown = 0x1000 + 852;
    while(spriteObjIndex < shootingCountry2Round) {
        scoreStateTmp = scoreStates2[spriteObjIndex];
        scoreOffset = spriteObjIndex << 1;

        if (scoreStateTmp == 1) {
            score2TileMapUp[spriteObjIndex2] = 0x182 | (0 << 14) | TPAL2 | (1<<13);
            score2TileMapUp[spriteObjIndex2 + 1] = 0x182 | (1 << 14) | TPAL2 | (1<<13);
            
            score2TileMapDown[spriteObjIndex2] = 0x183 | (0 << 14) | TPAL2 | (1<<13);
            score2TileMapDown[spriteObjIndex2 + 1] = 0x183 | (1 << 14) | TPAL2 | (1<<13);

        } else if (scoreStateTmp == 2) {
            score2TileMapUp[spriteObjIndex2] = 0x180 | (0 << 14) | TPAL3 | (1<<13);
            score2TileMapUp[spriteObjIndex2 + 1] = 0x180 | (1 << 14) | TPAL3 | (1<<13);
            
            score2TileMapDown[spriteObjIndex2] = 0x181 | (0 << 14) | TPAL3 | (1<<13);
            score2TileMapDown[spriteObjIndex2 + 1] = 0x181 | (1 << 14) | TPAL3 | (1<<13);
        }

        spriteObjIndex++;
        spriteObjIndex2 += 2;
    }
    score2TileMapSize = spriteObjIndex<<2;
    refreshScore2 = (0 < shootingCountry2Round);
}

/*!\brief Initialize the Level screen.
*/
void initLevel() {
    gameState = GameStateLevel;
    strikerState = StrikerStateIdle;
    ballState = BallStateIdle;
    previousGoalkeeperState = GoalkeeperStateIdle;
    goalkeeperState = GoalkeeperStateIdle;
    goalkeeperJumpState = GoalkeeperJumpStateNone;
    loadState = LoadStateNone;

    enableGoalKeeperInput = 0;
    strikerFramesCounter = 0;
    goalkeeperFramesCounter = 0;
    manFramesCounter = 0;
    bigFlagFramesCounter = 0;
    littleFlagFramesCounter = 0;
    prio = 3;

    zOrder = 0;
    scoreState = 0;
    animNet = 0;
    netFramesCount = 0;
    displayBlueCircle = 0;
    ballGfxOffsetCounter = 0;

    circleFrameCounter = 0;
    circleBlinkCounter = 0;

    dmaClearVram();

    bgSetMapPtr(BG0, 0x0000, SC_32x32);
    bgSetMapPtr(BG1, 0x0000 + 2048, SC_32x32);

    bgInitTileSet(BG0, 
        &stage1Bg1Pic, 
        &stage1Bg1Palette, 
        PAL0, 
        (&stage1Bg1Pic_end - &stage1Bg1Palette), 
        32*8, 
        BG_16COLORS, 
        0x2000);

    bgSetGfxPtr(BG1, 0x2000);

    initHud(0x5000);

    dmaCopyVram((u8 *)&oam1Pic, 0x6000, &oam1Pic_end - &oam1Pic);
    dmaCopyVram((u8 *)&oam2Pic, 0x7000, &oam2Pic_end - &oam2Pic);

    // Striker Number
    if (isCpuStriker == 0) {
        dmaCopyVram(numberSource[playersNumber[numberIndex1 + ((shootingCountry1Round+1)<<1)]], 0x72A0, 192);

    } else if (isCpuStriker == 1) {
        dmaCopyVram(numberSource[playersNumber[numberIndex2 + ((shootingCountry2Round+1)<<1)]], 0x72A0, 192);
    }

    WaitForVBlank();

    dmaCopyCGram((u8 *)&oam1Palette, 128, 32<<3);

    WaitForVBlank();

    dmaCopyVram((u8 *)stage1Bg1TileMapType1, 0x0000, 2048);
    dmaCopyVram((u8 *)stage1Bg2TileMap, 0x0000 + 2048, 2048);

    // Players Name
    
    spriteObjIndex = 0;
    while (spriteObjIndex < 5) {
        strikersName1[spriteObjIndex] = playersName[spriteObjIndex + 1 + (6 * country1type)];
        spriteObjIndex++;
    }

    spriteObjIndex = 0;
    while (spriteObjIndex < 5) {
        strikersName2[spriteObjIndex] = playersName[spriteObjIndex + 1 + (6 * country2type)];
        spriteObjIndex++;
    }

    goalkeeperName1 = playersName[6 * country1type];
    goalkeeperName2 = playersName[6 * country2type];

    if (isCpuStriker == 0) {
        printRole(playersNumber[numberIndex1 + 1 + ((shootingCountry1Round+1)<<1)], 5);
        printName(strikersName1[shootingCountry1Round], 5 + 1);

        printRole(0, 22);
        printName(goalkeeperName2, 22 + 1);

    } else {
        printRole(playersNumber[numberIndex2 + 1 + ((shootingCountry2Round+1)<<1)], 22);
        printName(strikersName2[shootingCountry2Round], 22 + 1);

        printRole(0, 5);
        printName(goalkeeperName1, 5 + 1);
    }

    refreshBg3 = 1;

    setMode(BG_MODE1, BG3_MODE1_PRORITY_HIGH);
    bgSetEnable(BG0);
    bgSetEnable(BG1);
    bgSetEnable(BG2);
    bgSetDisable(BG3);

    // Workaround for SNES
    bgSetScroll(BG0, 0, -1);
    bgSetScroll(BG1, 0, -1);
    bgSetScroll(BG2, 0, -1);

    WaitForVBlank();

    initSpriteEngine(OBJ_SIZE8_L16);

    oamId = oamIdOffset;

    selectedSprites = hairIdleSprites;
    oamSetMetaSprite();
    oamIdOffset = oamId;

    selectedSprites = shirtNumberIdleSprites;
    oamSetMetaSprite();
    oamIdOffset = oamId;

    selectedSprites = strikerIdleSprites;
    oamSetMetaSprite();
    oamIdOffset = oamId;

    ballOamId = oamIdOffset;
    oamId = oamIdOffset;
    selectedSprites = ballIdleSprites;
    oamSetMetaSprite();
    oamIdOffset = oamId;

    goalkeeperOamId = oamIdOffset;
    oamId = oamIdOffset;
    selectedSprites = goalKeeperIdleSprites;
    oamSetMetaSprite();
    oamIdOffset = oamId;
    goalkeeperSpritesCount = spritesCount;

    oamId = oamIdOffset;
    selectedSprites = ballshadowBigSprites;
    oamSetMetaSprite();
    oamIdOffset = oamId;

    showCountry1Balls(ballsCounter);
    showCountry2Balls(ballsCounter);

    if (isCpuStriker == 0) {
        selectedBallCountryStartOamId = ballCountry1StartOamId;
        goalUpAddress = 0x1000 + 803 + goalAddressOffset;
        goalDownAddress = 0x1000 + 835 + goalAddressOffset;
        country1Palette = PAL4;
        country2Palette = PAL5;

    } else {
        selectedBallCountryStartOamId = ballCountry2StartOamId;
        goalUpAddress = 0x1000 + 820 + goalAddressOffset;
        goalDownAddress = 0x1000 + 852 + goalAddressOffset;
        country1Palette = PAL5;
        country2Palette = PAL4;
    }

    ballGfxOffset = 0x144;

    // Player1 icon
    oamSet(oamIdOffset, 28, 184, prio, 0, 0, 0x1B6, PAL6); oamSetEx(oamIdOffset, OBJ_SMALL, OBJ_SHOW); oamIdOffset += 4;

    // SNES Controller icon
    oamSet(oamIdOffset, 23, 184, prio, 0, 0, 0x1B8, PAL6); oamSetEx(oamIdOffset, OBJ_LARGE, OBJ_SHOW); oamIdOffset += 4;

    // CPU icon
    oamSet(oamIdOffset, 159, 184, prio, 0, 0, 0x160, PAL6); oamSetEx(oamIdOffset, OBJ_LARGE, OBJ_SHOW); oamIdOffset += 4;

    oamSet(oamIdOffset, 16, 8, prio, 0, 0, 0x18A, country1Palette); oamSetEx(oamIdOffset, OBJ_LARGE, OBJ_SHOW); oamIdOffset += 4;
    oamSet(oamIdOffset, 24, 8, prio, 0, 0, 0x18B, country1Palette); oamSetEx(oamIdOffset, OBJ_LARGE, OBJ_SHOW); oamIdOffset += 4;
    
    WaitForVBlank();

    countryNameOffsetX = 0;
    setStrikerPalette();
    setCountryFlagPalette(countryOfStrikertype, PAL4);
    setCountry1();

    oamSet(oamIdOffset, 144, 8, prio, 0, 0, 0x18D, country2Palette); oamSetEx(oamIdOffset, OBJ_LARGE, OBJ_SHOW); oamIdOffset += 4;
    oamSet(oamIdOffset, 152, 8, prio, 0, 0, 0x18E, country2Palette); oamSetEx(oamIdOffset, OBJ_LARGE, OBJ_SHOW); oamIdOffset += 4;
    
    countryNameOffsetX = 0;
    setCountry2();
    setCountryFlagPalette(countryOfGoalkeepertype, PAL5);

    createOverlays(
        (u8 *) &tableleft, 
        (u8 *) &tableright,
        (u8 *) &tableleft2, 
        (u8 *) &tableright2,
        BG1, 
        0b10000000, 
        0b00011111, 
        0b01000000);

    dmaCopyVram((u8 *)&goalkeeperPic, 0x6000, 256);
    dmaCopyVram((u8 *)&goalkeeperPic + 256, 0x6100, 256);

    dmaCopyVram((u8 *)&strikerPic, 0x7000, 992);

    WaitForVBlank();
    initLevelMusic();
    
    WaitForVBlank();
    spcPlay(0);
    WaitForVBlank();

    setFadeEffect(FADE_IN);
	WaitForVBlank();
}

/*!\brief Update supporters.
*/
void updateSupporters() {
    if (manFramesCounter == 32) {
        manFramesCounter = 0;
        return;
    }

    if (manFramesCounter == 0) {
        dmaCopyVram((u8 *)&stagePeopleMan1Pic, 0x3600, 192);

        dmaCopyVram((u8 *)&stagePeopleOldMan1Pic, 0x36C0, 160);

    } else if (manFramesCounter == 7) {
        dmaCopyVram((u8 *)&stagePeopleMan2Pic, 0x3600, 192);

        dmaCopyVram((u8 *)&stagePeopleOldMan2Pic, 0x36C0, 160);

    } else if (manFramesCounter == 15) {
        dmaCopyVram((u8 *)&stagePeopleMan1Pic, 0x3600, 192);

        dmaCopyVram((u8 *)&stagePeopleOldMan1Pic, 0x36C0, 160);

    } else if (manFramesCounter == 23) {
        dmaCopyVram((u8 *)&stagePeopleMan3Pic, 0x3600, 192);

        dmaCopyVram((u8 *)&stagePeopleOldMan3Pic, 0x36C0, 160);
    }

    manFramesCounter++;
}

/*!\brief Update the big flag.
*/
void updateBigFlag() {
    if (bigFlagFramesCounter == 64) {
        bigFlagFramesCounter = 0;
        return;
    }

    if (bigFlagFramesCounter == 0) {
        dmaCopyVram((u8 *)&stagePeopleBigFlag2Pic, 0x3760, 160);
        dmaCopyVram((u8 *)&stagePeopleBigFlag2Pic, 0x37B0, 160);

    } else if (bigFlagFramesCounter == 15) {
        dmaCopyVram((u8 *)&stagePeopleBigFlag1Pic, 0x3760, 160);
        dmaCopyVram((u8 *)&stagePeopleBigFlag1Pic, 0x37B0, 160);

    } else if (bigFlagFramesCounter == 31) {
        dmaCopyVram((u8 *)&stagePeopleBigFlag2Pic, 0x3760, 160);
        dmaCopyVram((u8 *)&stagePeopleBigFlag2Pic, 0x37B0, 160);

    } else if (bigFlagFramesCounter == 47) {
        dmaCopyVram((u8 *)&stagePeopleBigFlag3Pic, 0x3760, 160);
        dmaCopyVram((u8 *)&stagePeopleBigFlag3Pic, 0x37B0, 160);
    }

    bigFlagFramesCounter++;
}

/*!\brief Update the little flag.
*/
void updateLittleFlag() {
    if (littleFlagFramesCounter == 48) {
        littleFlagFramesCounter = 0;
        return;
    }

    if (littleFlagFramesCounter == 0) {
        dmaCopyVram((u8 *)&stagePeopleLittleFlagPic, 0x3800, 32);
        dmaCopyVram((u8 *)&stagePeopleLittleFlagPic, 0x3810, 32);
        dmaCopyVram((u8 *)&stagePeopleLittleFlagPic, 0x3820, 32);

    } else if (littleFlagFramesCounter == 15) {
        dmaCopyVram((u8 *)&stagePeopleLittleFlagPic + 32, 0x3800, 32);
        dmaCopyVram((u8 *)&stagePeopleLittleFlagPic + 32, 0x3810, 32);
        dmaCopyVram((u8 *)&stagePeopleLittleFlagPic + 32, 0x3820, 32);

    } else if (littleFlagFramesCounter == 31) {
        dmaCopyVram((u8 *)&stagePeopleLittleFlagPic + 64, 0x3800, 32);
        dmaCopyVram((u8 *)&stagePeopleLittleFlagPic + 64, 0x3810, 32);
        dmaCopyVram((u8 *)&stagePeopleLittleFlagPic + 64, 0x3820, 32);
    }

    littleFlagFramesCounter++;
}

/*!\brief Make the goalkeeper decision for CPU.
*/
void makeGoalkeeperDecision() {
    randomizedState = rand() & 7;

    if (randomizedState < 2) {
        goalkeeperState = 2;

    } else if (randomizedState > 7) {
        goalkeeperState = 7;

    } else {
        goalkeeperState = randomizedState;
    }
}

/*!\brief Make the striker decision for CPU.
*/
void makeStrikerDecision() {
    if (strikerState != StrikerStateIdle) {
        return;
    }

    randomizedState = rand();

    if (randomizedState%2 == 0) {
        strikerState = StrikerStatePreShoot;
        strikerStatePost = StrikerStateShootLow;

    } else {
        strikerState = StrikerStatePreShoot;
        strikerStatePost = StrikerStateShootHigh;
    }

    if (randomizedState%7 <= 2) {
        if (strikerStatePost == StrikerStateShootLow) {
            ballStateSelectedByCpu = BallStatePreMoveLowLeft;

        } else if (strikerStatePost == StrikerStateShootHigh) {
            ballStateSelectedByCpu = BallStatePreMoveHighLeft;
        }

    } else if (randomizedState%7 <= 4) {
        if (strikerStatePost == StrikerStateShootLow) {
            ballStateSelectedByCpu = BallStatePreMoveLowRight;

        } else if (strikerStatePost == StrikerStateShootHigh) {
            ballStateSelectedByCpu = BallStatePreMoveHighRight;
        }

    } else {
        if (strikerStatePost == StrikerStateShootLow) {
            ballStateSelectedByCpu = BallStatePreMoveLow;

        } else if (strikerStatePost == StrikerStateShootHigh) {
            ballStateSelectedByCpu = BallStatePreMoveHigh;
        }
    }
}

/*!\brief Handle the level input.
*/
void handleLevelInput() {
    if (enableGoalKeeperInput == 1) {
        if (goalkeeperState != StrikerStateIdle) {
            return;
        }

        if (padDown0 & KEY_A) {
            if (pad0 & KEY_RIGHT) {
                goalkeeperState = GoalkeeperStateJumpHighSideRight;

            } else if (pad0 & KEY_LEFT) {
                goalkeeperState = GoalkeeperStateJumpHighSideLeft;

            } else {
                goalkeeperState = GoalkeeperStateJumpHigh;
            }

        } else if (padDown0 & KEY_B) {
            if (pad0 & KEY_RIGHT) {
                goalkeeperState = GoalkeeperStateJumpLowSideRight;

            } else if (pad0 & KEY_LEFT) {
                goalkeeperState = GoalkeeperStateJumpLowSideLeft;

            } else {
                goalkeeperState = GoalkeeperStateJumpLow;
            }
        }
    }

    if (isCpuStriker == 1) {
        makeStrikerDecision();
        return;
    }

    if (strikerState != StrikerStateIdle) {
        return;
    }

    if (padDown0 & KEY_A) {
        strikerState = StrikerStatePreShoot;
        strikerStatePost = StrikerStateShootHigh;

    } else if (padDown0 & KEY_B) {
        strikerState = StrikerStatePreShoot;
        strikerStatePost = StrikerStateShootLow;
    }
}

/*!\brief Initialize the ball variables.
*/
void initBall() {
    ballshadowOffsetX = 0;
    ballshadowOffsetY = 0;
    ballOffsetIndex = 0;
    ballshadowOffsetIndex = 0;
    stopBallOffset = 0;
    stopBallshadowOffset = 0;
    showTinyShadow = 0;
}

/*!\brief Set the striker in the OAM.
*/
void oamSetStriker() {
    switch (strikerState) {
    case StrikerStatePreShoot:
        if (strikerFramesCounter == 12) {
            oamId = 0;
            selectedSprites = strikerPreShootSprites1;
            oamSetMetaSprite();
            strikerSprites = &strikerPic + 1024;
            refreshStriker = 1;

        } else if (strikerFramesCounter == 20) {
            oamId = 0;
            selectedSprites = strikerPreShootSprites2;
            oamSetMetaSprite();
            strikerSprites = &strikerPic + (1024*2);
            refreshStriker = 1;

        } else if (strikerFramesCounter == 27) {
            oamId = 0;
            selectedSprites = strikerPreShootSprites3;
            oamSetMetaSprite();
            strikerSprites = &strikerPic + (1024*3);
            refreshStriker = 1;

        } else if (strikerFramesCounter == 33) {
            oamId = 0;
            selectedSprites = strikerPreShootSprites4;
            oamSetMetaSprite();
            strikerSprites = &strikerPic + (1024*4);
            refreshStriker = 1;

        } else if (strikerFramesCounter == 37) {
            strikerState = strikerStatePost;
            strikerFramesCounter = 0;
        }

        strikerFramesCounter++;

        break;

    case StrikerStateShootLow:
        if (strikerFramesCounter == 0) {
            oamId = 0;
            selectedSprites = strikerShootLowSprites1;
            oamSetMetaSprite();
            strikerSprites = &strikerPic + (1024*20);
            refreshStriker = 1;

        } else if (strikerFramesCounter == 9) {
            oamId = 0;
            selectedSprites = strikerShootLowSprites2;
            oamSetMetaSprite();
            strikerSprites = &strikerPic + (1024*21);
            refreshStriker = 1;

        } else if (strikerFramesCounter == 13) {
            oamId = 0;
            selectedSprites = strikerShootLowSprites3;
            oamSetMetaSprite();
            strikerSprites = &strikerPic + (1024*22);
            refreshStriker = 1;

            if (isCpuStriker == 1) {
                ballState = ballStateSelectedByCpu;

                if (ballState == BallStatePreMoveLowLeft 
                || ballState == BallStatePreMoveHighLeft) {
                    oamId = 54<<2;
                    selectedSprites = ballshadowBigSpritesInvert;
                    oamSetMetaSprite();
                }

            } else {
                if (pad0 & KEY_RIGHT) {
                    ballState = BallStatePreMoveLowRight;

                } else if (pad0 & KEY_LEFT) {
                    oamId = 54<<2;
                    selectedSprites = ballshadowBigSpritesInvert;
                    oamSetMetaSprite();
                    ballState = BallStatePreMoveLowLeft;

                } else {
                    ballState = BallStatePreMoveLow;
                }
            }

            initBall();
            
            if (isCpuStriker == 1) {
                enableGoalKeeperInput = 1;

            } else {
                makeGoalkeeperDecision();
            }

        } else if (strikerFramesCounter == 17) {
            oamId = 0;
            selectedSprites = strikerShootLowSprites4;
            oamSetMetaSprite();
            strikerSprites = &strikerPic + (1024*23);
            refreshStriker = 1;

        } else if (strikerFramesCounter == 21) {
            oamId = 0;
            selectedSprites = strikerShootLowSprites5;
            oamSetMetaSprite();
            strikerSprites = &strikerPic + (1024*24);
            refreshStriker = 1;

        } else if (strikerFramesCounter == 25) {
            oamId = 0;
            selectedSprites = strikerShootLowSprites6;
            oamSetMetaSprite();
            strikerSprites = &strikerPic + (1024*25);
            refreshStriker = 1;

        } else if (strikerFramesCounter == 34) {
            oamId = 0;
            selectedSprites = strikerShootLowSprites7;
            oamSetMetaSprite();
            strikerSprites = &strikerPic + (1024*26);
            refreshStriker = 1;

        }

        strikerFramesCounter++;
        
        if (strikerFramesCounter == 40) {
            strikerState = StrikerStatePostShoot;
            strikerFramesCounter = 0;
        }
        break;

    case StrikerStateShootHigh:
        if (strikerFramesCounter == 0) {
            oamId = 0;
            selectedSprites = strikerShootHighSprites1;
            oamSetMetaSprite();
            strikerSprites = &strikerPic + (1024*5);
            refreshStriker = 1;

        } else if (strikerFramesCounter == 5) {
            oamId = 0;
            selectedSprites = strikerShootHighSprites2;
            oamSetMetaSprite();
            strikerSprites = &strikerPic + (1024*6);
            refreshStriker = 1;

        } else if (strikerFramesCounter == 14) {
            oamId = 0;
            selectedSprites = strikerShootHighSprites3;
            oamSetMetaSprite();
            strikerSprites = &strikerPic + (1024*7);
            refreshStriker = 1;

        } else if (strikerFramesCounter == 18) {
            oamId = 0;
            selectedSprites = strikerShootHighSprites4;
            oamSetMetaSprite();
            strikerSprites = &strikerPic + (1024*8);
            refreshStriker = 1;

            if (isCpuStriker == 1) {
                ballState = ballStateSelectedByCpu;

                if (ballState == BallStatePreMoveLowLeft 
                || ballState == BallStatePreMoveHighLeft) {
                    oamId = 54<<2;
                    selectedSprites = ballshadowBigSpritesInvert;
                    oamSetMetaSprite();
                }

            } else {
                if (pad0 & KEY_RIGHT) {
                    ballState = BallStatePreMoveHighRight;

                } else if (pad0 & KEY_LEFT) {
                    oamId = 54<<2;
                    selectedSprites = ballshadowBigSpritesInvert;
                    oamSetMetaSprite();
                    ballState = BallStatePreMoveHighLeft;

                } else {
                    ballState = BallStatePreMoveHigh;
                }
            }

            initBall();

            if (isCpuStriker == 1) {
                enableGoalKeeperInput = 1;

            } else {
                makeGoalkeeperDecision();
            }

        } else if (strikerFramesCounter == 22) {
            oamId = 0;
            selectedSprites = strikerShootHighSprites5;
            oamSetMetaSprite();
            strikerSprites = &strikerPic + (1024*9);
            refreshStriker = 1;

        } else if (strikerFramesCounter == 26) {
            oamId = 0;
            selectedSprites = strikerShootHighSprites6;
            oamSetMetaSprite();
            strikerSprites = &strikerPic + (1024*10);
            refreshStriker = 1;
        }

        strikerFramesCounter++;
        
        if (strikerFramesCounter == 30) {
            strikerState = StrikerStatePostShoot;
            strikerFramesCounter = 0;
        }
        break;

    case StrikerStatePostShoot:
        if (strikerFramesCounter == 0) {
            oamId = 0;
            selectedSprites = strikerPostShootSprites1;
            oamSetMetaSprite();
            strikerSprites = &strikerPic + (1024*11);
            refreshStriker = 1;

        } else if (strikerFramesCounter == 7) {
            oamId = 0;
            selectedSprites = strikerPostShootSprites2;
            oamSetMetaSprite();
            strikerSprites = &strikerPic + (1024*12);
            refreshStriker = 1;

        } else if (strikerFramesCounter == 15) {
            oamId = 0;
            selectedSprites = strikerPostShootSprites3;
            oamSetMetaSprite();
            strikerSprites = &strikerPic + (1024*13);
            refreshStriker = 1;
        } 

        strikerFramesCounter++;

        if (strikerFramesCounter == 57) {
            strikerState = StrikerStateWait;
            strikerFramesCounter = 0;
        }

        break;

    case StrikerStateWait:
        if (scoreState == 1) {
            strikerState = StrikerStateHappy;
            strikerFramesCounter = 0;
            loadFrameCounter = 0;
            loadState = LoadStateStart;

        } else if (scoreState == 2) {
            strikerState = StrikerStateSad;
            strikerFramesCounter = 0;
            loadFrameCounter = 0;
            loadState = LoadStateStart;
        }
        break;

    case StrikerStateHappy:
        if (strikerFramesCounter == 0) {
            oamId = 0;
            selectedSprites = strikerHappySprites1;
            oamSetMetaSprite();
            strikerSprites = &strikerPic + (1024*17);
            refreshStriker = 1;

        } else if (strikerFramesCounter == 9) {
            oamId = 0;
            selectedSprites = strikerHappySprites2;
            oamSetMetaSprite();
            strikerSprites = &strikerPic + (1024*18);
            refreshStriker = 1;

        } else if (strikerFramesCounter == 14) {
            oamId = 0;
            selectedSprites = strikerHappySprites3;
            oamSetMetaSprite();
            strikerSprites = &strikerPic + (1024*19);
            refreshStriker = 1;

            strikerState = StrikerStateEnd;
        }

        strikerFramesCounter++;
        break;

    case StrikerStateSad:
        if (strikerFramesCounter == 104) {
            oamId = 0;
            selectedSprites = strikerSadSprites1;
            oamSetMetaSprite();
            strikerSprites = &strikerPic + (1024*14);
            refreshStriker = 1;

        } else if (strikerFramesCounter == 112) {
            oamId = 0;
            selectedSprites = strikerSadSprites2;
            oamSetMetaSprite();
            strikerSprites = &strikerPic + (1024*15);
            refreshStriker = 1;

        } else if (strikerFramesCounter == 120) {
            oamId = 0;
            selectedSprites = strikerSadSprites3;
            oamSetMetaSprite();
            strikerSprites = &strikerPic + (1024*16);
            refreshStriker = 1;

            strikerState = StrikerStateEnd;
        }

        strikerFramesCounter++;
        break;
    }
}

/*!\brief Set the goalkeeper in the OAM.
*/
void oamSetGoalkeeper() {
    switch(goalkeeperState) {
        case GoalkeeperStateIdle:
            if (scoreState != 0) {
                goalkeeperState = GoalkeeperStateNoMove;
            }
            break;

        case GoalkeeperStateNoMove:
            if (goalkeeperFramesCounter == 0) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperNoMoveSprites1;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512*29);
                refreshGoalkeeper = 1;
                previousGoalkeeperState = goalkeeperState;

            } else if (goalkeeperFramesCounter == 2) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperNoMoveSprites2;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512*30);
                refreshGoalkeeper = 1;

            } else if (goalkeeperFramesCounter == 5) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperNoMoveSprites3;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512*31);
                refreshGoalkeeper = 1;

                goalkeeperState = GoalkeeperStateWait;
                goalkeeperFramesCounter = 0;
                break;
            }

            gkX = selectedSprites[0] + gkOffsetX;
            gkY = selectedSprites[1] + gkOffsetY;

            goalkeeperFramesCounter++;

            break;

        case GoalkeeperStateJumpLow:
            if (goalkeeperFramesCounter == 0) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperJumpLowSprites1;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + 512;
                refreshGoalkeeper = 1;
                previousGoalkeeperState = goalkeeperState;

            } else if (goalkeeperFramesCounter == 8) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperJumpLowSprites2;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512*2);
                refreshGoalkeeper = 1;

            } else if (goalkeeperFramesCounter == 16) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperJumpLowSprites3;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512*3);
                refreshGoalkeeper = 1;

            } else if (goalkeeperFramesCounter == 24) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperJumpLowSprites4;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512*4);
                refreshGoalkeeper = 1;

            } else if (goalkeeperFramesCounter == 32) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperJumpLowSprites5;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512*5);
                refreshGoalkeeper = 1;

            } else if (goalkeeperFramesCounter == 40) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperJumpLowSprites6;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512*6);
                refreshGoalkeeper = 1;

                goalkeeperState = GoalkeeperStateWait;
                goalkeeperFramesCounter = 0;
                break;
            }

            gkX = selectedSprites[0] + gkOffsetX;
            gkY = selectedSprites[1] + gkOffsetY;

            goalkeeperFramesCounter++;

            break;

        case GoalkeeperStateJumpLowSideRight:
            if (goalkeeperFramesCounter == 0) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperPreJumpSideRightNotFar1;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512 * 15);
                refreshGoalkeeper = 1;
                previousGoalkeeperState = goalkeeperState;

            } else if (goalkeeperFramesCounter == 1) {
                goalkeeperJumpState = GoalkeeperJumpLowSideRight1State;
                gkOffsetX = 0;
                gkOffsetY = 0;
                offsetIndex = 0;

            } else if (goalkeeperFramesCounter == 3) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperJumpLowSideSpritesRight2;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512 * 16);
                refreshGoalkeeper = 1;

            } else if (goalkeeperFramesCounter == 4) {
                goalkeeperJumpState = GoalkeeperJumpLowSideRight2State;
                gkOffsetX = 0;
                gkOffsetY = 0;
                offsetIndex = 0;

            } else if (goalkeeperFramesCounter == 6) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperJumpLowSideSpritesRight3;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512 * 17);
                refreshGoalkeeper = 1;

            } else if (goalkeeperFramesCounter == 7) {
                goalkeeperJumpState = GoalkeeperJumpLowSideRight3State;
                gkOffsetX = 0;
                gkOffsetY = 0;
                offsetIndex = 0;

            } else if (goalkeeperFramesCounter == 10) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperJumpLowSideSpritesRight4;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512 * 18);
                refreshGoalkeeper = 1;

            } else if (goalkeeperFramesCounter == 11) {
                goalkeeperJumpState = GoalkeeperJumpLowSideRight4State;
                gkOffsetX = 0;
                gkOffsetY = 0;
                offsetIndex = 0;

            } else if (goalkeeperFramesCounter == 16) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperJumpLowSideSpritesRight5;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512 * 19);
                refreshGoalkeeper = 1;

            } else if (goalkeeperFramesCounter == 17) {
                goalkeeperJumpState = GoalkeeperJumpLowSideRight5State;
                gkOffsetX = 0;
                gkOffsetY = 0;
                offsetIndex = 0;

            } else if (goalkeeperFramesCounter == 20) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperJumpLowSideSpritesRight6;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512 * 20);
                refreshGoalkeeper = 1;

            } else if (goalkeeperFramesCounter == 21) {
                goalkeeperJumpState = GoalkeeperJumpLowSideRight6State;
                gkOffsetX = 0;
                gkOffsetY = 0;
                offsetIndex = 0;
            }

            goalkeeperFramesCounter++;

            if (goalkeeperJumpState == GoalkeeperJumpLowSideRight1State) {
                oamId = goalkeeperOamId;
                gkOffsetX += jumpLowSideRightXYvalues1[offsetIndex];
                gkOffsetY += jumpLowSideRightXYvalues1[offsetIndex + 1];
                selectedSprites = goalKeeperPreJumpSideRightNotFar1;
                offsetX = gkOffsetX;
                offsetY = gkOffsetY;
                oamSetMetaSpriteXYEx();
                offsetIndex += 2;
                if (jumpLowSideRightXYvalues1[offsetIndex] == 0xFFF) {
                    goalkeeperJumpState = GoalkeeperJumpStateNone;
                }

            } else if (goalkeeperJumpState == GoalkeeperJumpLowSideRight2State) {
                oamId = goalkeeperOamId;
                gkOffsetX += jumpLowSideRightXYvalues2[offsetIndex];
                gkOffsetY += jumpLowSideRightXYvalues2[offsetIndex + 1];
                selectedSprites = goalKeeperJumpLowSideSpritesRight2;
                offsetX = gkOffsetX;
                offsetY = gkOffsetY;
                oamSetMetaSpriteXYEx();
                offsetIndex += 2;
                if (jumpLowSideRightXYvalues2[offsetIndex] == 0xFFF) {
                    goalkeeperJumpState = GoalkeeperJumpStateNone;
                }

            } else if (goalkeeperJumpState == GoalkeeperJumpLowSideRight3State) {
                oamId = goalkeeperOamId;
                gkOffsetX += jumpLowSideRightXYvalues3[offsetIndex];
                gkOffsetY += jumpLowSideRightXYvalues3[offsetIndex + 1];
                selectedSprites = goalKeeperJumpLowSideSpritesRight3;
                offsetX = gkOffsetX;
                offsetY = gkOffsetY;
                oamSetMetaSpriteXYEx();
                offsetIndex += 2;
                if (jumpLowSideRightXYvalues3[offsetIndex] == 0xFFF) {
                    goalkeeperJumpState = GoalkeeperJumpStateNone;
                }

            } else if (goalkeeperJumpState == GoalkeeperJumpLowSideRight4State) {
                oamId = goalkeeperOamId;
                gkOffsetX += jumpLowSideRightXYvalues4[offsetIndex];
                gkOffsetY += jumpLowSideRightXYvalues4[offsetIndex + 1];
                selectedSprites = goalKeeperJumpLowSideSpritesRight4;
                offsetX = gkOffsetX;
                offsetY = gkOffsetY;
                oamSetMetaSpriteXYEx();
                offsetIndex += 2;
                if (jumpLowSideRightXYvalues4[offsetIndex] == 0xFFF) {
                    goalkeeperJumpState = GoalkeeperJumpStateNone;
                }

            } else if (goalkeeperJumpState == GoalkeeperJumpLowSideRight5State) {
                oamId = goalkeeperOamId;
                gkOffsetX += jumpLowSideRightXYvalues5[offsetIndex];
                gkOffsetY += jumpLowSideRightXYvalues5[offsetIndex + 1];
                selectedSprites = goalKeeperJumpLowSideSpritesRight5;
                offsetX = gkOffsetX;
                offsetY = gkOffsetY;
                oamSetMetaSpriteXYEx();
                offsetIndex += 2;
                if (jumpLowSideRightXYvalues5[offsetIndex] == 0xFFF) {
                    goalkeeperJumpState = GoalkeeperJumpStateNone;
                }

            } else if (goalkeeperJumpState == GoalkeeperJumpLowSideRight6State) {
                oamId = goalkeeperOamId;
                gkOffsetX += jumpLowSideRightXYvalues6[offsetIndex];
                gkOffsetY += jumpLowSideRightXYvalues6[offsetIndex + 1];
                selectedSprites = goalKeeperJumpLowSideSpritesRight6;
                offsetX = gkOffsetX;
                offsetY = gkOffsetY;
                oamSetMetaSpriteEx();
                offsetIndex += 2;
                if (jumpLowSideRightXYvalues6[offsetIndex] == 0xFFF) {
                    goalkeeperJumpState = GoalkeeperJumpStateNone;

                    goalkeeperState = GoalkeeperStateWait;
                    goalkeeperFramesCounter = 0;
                    break;
                }
            }

            gkX = selectedSprites[0] + gkOffsetX;
            gkY = selectedSprites[1] + gkOffsetY;

            break;

        case GoalkeeperStateJumpHighSideRight:
            if (goalkeeperFramesCounter == 0) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperPreJumpSideRight1;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512 * 15);
                refreshGoalkeeper = 1;
                previousGoalkeeperState = goalkeeperState;

            } else if (goalkeeperFramesCounter == 1) {
                goalkeeperJumpState = GoalkeeperJumpHighSideRight1State;
                gkOffsetX = 0;
                gkOffsetY = 0;
                offsetIndex = 0;
                
            } else if (goalkeeperFramesCounter == 3) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperJumpHighSideSpritesRight2;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512 * 21);
                refreshGoalkeeper = 1;

            } else if (goalkeeperFramesCounter == 4) {
                goalkeeperJumpState = GoalkeeperJumpHighSideRight2State;
                gkOffsetX = 0;
                gkOffsetY = 0;
                offsetIndex = 0;

            } else if (goalkeeperFramesCounter == 6) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperJumpHighSideSpritesRight3;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512 * 22);
                refreshGoalkeeper = 1;

            } else if (goalkeeperFramesCounter == 7) {
                goalkeeperJumpState = GoalkeeperJumpHighSideRight3State;
                gkOffsetX = 0;
                gkOffsetY = 0;
                offsetIndex = 0;

            } else if (goalkeeperFramesCounter == 10) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperJumpHighSideSpritesRight4;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512 * 23);
                refreshGoalkeeper = 1;

            } else if (goalkeeperFramesCounter == 11) {
                goalkeeperJumpState = GoalkeeperJumpHighSideRight4State;
                gkOffsetX = 0;
                gkOffsetY = 0;
                offsetIndex = 0;

            } else if (goalkeeperFramesCounter == 14) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperJumpHighSideSpritesRight5;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512 * 24);
                refreshGoalkeeper = 1;

            } else if (goalkeeperFramesCounter == 15) {
                goalkeeperJumpState = GoalkeeperJumpHighSideRight5State;
                gkOffsetX = 0;
                gkOffsetY = 0;
                offsetIndex = 0;

            } else if (goalkeeperFramesCounter == 29) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperJumpHighSideSpritesRight6;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512 * 25);
                refreshGoalkeeper = 1;

            } else if (goalkeeperFramesCounter == 30) {
                goalkeeperJumpState = GoalkeeperJumpHighSideRight6State;
                gkOffsetX = 0;
                gkOffsetY = 0;
                offsetIndex = 0;

            } else if (goalkeeperFramesCounter == 32) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperJumpHighSideSpritesRight7;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512 * 20);
                refreshGoalkeeper = 1;
            }

            goalkeeperFramesCounter++;

            if (goalkeeperJumpState == GoalkeeperJumpHighSideRight1State) {
                oamId = goalkeeperOamId;
                gkOffsetX += jumpHighSideRightXYvalues1[offsetIndex];
                gkOffsetY += jumpHighSideRightXYvalues1[offsetIndex + 1];
                selectedSprites = goalKeeperPreJumpSideRightNotFar1;
                offsetX = gkOffsetX;
                offsetY = gkOffsetY;
                oamSetMetaSpriteXYEx();
                offsetIndex += 2;
                if (jumpHighSideRightXYvalues1[offsetIndex] == 0xFFF) {
                    goalkeeperJumpState = GoalkeeperJumpStateNone;
                }

            } else if (goalkeeperJumpState == GoalkeeperJumpHighSideRight2State) {
                oamId = goalkeeperOamId;
                gkOffsetX += jumpHighSideRightXYvalues2[offsetIndex];
                gkOffsetY += jumpHighSideRightXYvalues2[offsetIndex + 1];
                selectedSprites = goalKeeperJumpHighSideSpritesRight2;
                offsetX = gkOffsetX;
                offsetY = gkOffsetY;
                oamSetMetaSpriteXYEx();
                offsetIndex += 2;
                if (jumpHighSideRightXYvalues2[offsetIndex] == 0xFFF) {
                    goalkeeperJumpState = GoalkeeperJumpStateNone;
                }

            } else if (goalkeeperJumpState == GoalkeeperJumpHighSideRight3State) {
                oamId = goalkeeperOamId;
                gkOffsetX += jumpHighSideRightXYvalues3[offsetIndex];
                gkOffsetY += jumpHighSideRightXYvalues3[offsetIndex + 1];
                selectedSprites = goalKeeperJumpHighSideSpritesRight3;
                offsetX = gkOffsetX;
                offsetY = gkOffsetY;
                oamSetMetaSpriteXYEx();
                offsetIndex += 2;
                if (jumpHighSideRightXYvalues3[offsetIndex] == 0xFFF) {
                    goalkeeperJumpState = GoalkeeperJumpStateNone;
                }

            } else if (goalkeeperJumpState == GoalkeeperJumpHighSideRight4State) {
                oamId = goalkeeperOamId;
                gkOffsetX += jumpHighSideRightXYvalues4[offsetIndex];
                gkOffsetY += jumpHighSideRightXYvalues4[offsetIndex + 1];
                selectedSprites = goalKeeperJumpHighSideSpritesRight4;
                offsetX = gkOffsetX;
                offsetY = gkOffsetY;
                oamSetMetaSpriteXYEx();
                offsetIndex += 2;
                if (jumpHighSideRightXYvalues4[offsetIndex] == 0xFFF) {
                    goalkeeperJumpState = GoalkeeperJumpStateNone;
                }

            } else if (goalkeeperJumpState == GoalkeeperJumpHighSideRight5State) {
                oamId = goalkeeperOamId;
                gkOffsetX += jumpHighSideRightXYvalues5[offsetIndex];
                gkOffsetY += jumpHighSideRightXYvalues5[offsetIndex + 1];
                selectedSprites = goalKeeperJumpHighSideSpritesRight5;
                offsetX = gkOffsetX;
                offsetY = gkOffsetY;
                oamSetMetaSpriteEx();
                offsetIndex += 2;
                if (jumpHighSideRightXYvalues5[offsetIndex] == 0xFFF) {
                    goalkeeperJumpState = GoalkeeperJumpStateNone;
                }

            } else if (goalkeeperJumpState == GoalkeeperJumpHighSideRight6State) {
                oamId = goalkeeperOamId;
                gkOffsetX += jumpHighSideRightXYvalues6[offsetIndex];
                gkOffsetY += jumpHighSideRightXYvalues6[offsetIndex + 1];
                selectedSprites = goalKeeperJumpHighSideSpritesRight6;
                offsetX = gkOffsetX;
                offsetY = gkOffsetY;
                oamSetMetaSpriteEx();
                offsetIndex += 2;
                if (jumpHighSideRightXYvalues6[offsetIndex] == 0xFFF) {
                    goalkeeperJumpState = GoalkeeperJumpStateNone;
                }
            }

            gkX = selectedSprites[0] + gkOffsetX;
            gkY = selectedSprites[1] + gkOffsetY;

            if (goalkeeperFramesCounter == 33) {
                goalkeeperState = GoalkeeperStateWait;
                goalkeeperFramesCounter = 0;
            }
            break;

        case GoalkeeperStateJumpHighSideRightNotFar:
            if (goalkeeperFramesCounter == 0) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperPreJumpSideRightNotFar1;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512 * 15);
                refreshGoalkeeper = 1;
                previousGoalkeeperState = goalkeeperState;

            } else if (goalkeeperFramesCounter == 1) {
                goalkeeperJumpState = GoalkeeperJumpHighSideRight1State;
                gkOffsetX = 0;
                gkOffsetY = 0;
                offsetIndex = 0;

            } else if (goalkeeperFramesCounter == 3) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperJumpHighSideSpritesRightNotFar2;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512 * 21);
                refreshGoalkeeper = 1;

            } else if (goalkeeperFramesCounter == 4) {
                goalkeeperJumpState = GoalkeeperJumpHighSideRight2State;
                gkOffsetX = 0;
                gkOffsetY = 0;
                offsetIndex = 0;

            } else if (goalkeeperFramesCounter == 6) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperJumpHighSideSpritesRightNotFar3;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512 * 22);
                refreshGoalkeeper = 1;

            } else if (goalkeeperFramesCounter == 7) {
                goalkeeperJumpState = GoalkeeperJumpHighSideRight3State;
                gkOffsetX = 0;
                gkOffsetY = 0;
                offsetIndex = 0;

            } else if (goalkeeperFramesCounter == 10) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperJumpHighSideSpritesRightNotFar4;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512 * 23);
                refreshGoalkeeper = 1;

            } else if (goalkeeperFramesCounter == 11) {
                goalkeeperJumpState = GoalkeeperJumpHighSideRight4State;
                gkOffsetX = 0;
                gkOffsetY = 0;
                offsetIndex = 0;

            } else if (goalkeeperFramesCounter == 14) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperJumpHighSideSpritesRightNotFar5;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512 * 24);
                refreshGoalkeeper = 1;

            } else if (goalkeeperFramesCounter == 15) {
                goalkeeperJumpState = GoalkeeperJumpHighSideRight5State;
                gkOffsetX = 0;
                gkOffsetY = 0;
                offsetIndex = 0;

            } else if (goalkeeperFramesCounter == 29) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperJumpHighSideSpritesRightNotFar6;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512 * 25);
                refreshGoalkeeper = 1;

            } else if (goalkeeperFramesCounter == 30) {
                goalkeeperJumpState = GoalkeeperJumpHighSideRight6State;
                gkOffsetX = 0;
                gkOffsetY = 0;
                offsetIndex = 0;

            } else if (goalkeeperFramesCounter == 36) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperJumpHighSideSpritesRightNotFar7;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512 * 20);
                refreshGoalkeeper = 1;
            }

            goalkeeperFramesCounter++;

            if (goalkeeperJumpState == GoalkeeperJumpHighSideRight1State) {
                oamId = goalkeeperOamId;
                gkOffsetX += jumpHighSideRightNotFarXYvalues1[offsetIndex];
                gkOffsetY += jumpHighSideRightNotFarXYvalues1[offsetIndex + 1];
                selectedSprites = goalKeeperPreJumpSideRightNotFar1;
                offsetX = gkOffsetX;
                offsetY = gkOffsetY;
                oamSetMetaSpriteXYEx();
                offsetIndex += 2;
                if (jumpHighSideRightNotFarXYvalues1[offsetIndex] == 0xFFF) {
                    goalkeeperJumpState = GoalkeeperJumpStateNone;
                }

            } else if (goalkeeperJumpState == GoalkeeperJumpHighSideRight2State) {
                oamId = goalkeeperOamId;
                gkOffsetX += jumpHighSideRightNotFarXYvalues2[offsetIndex];
                gkOffsetY += jumpHighSideRightNotFarXYvalues2[offsetIndex + 1];
                selectedSprites = goalKeeperJumpHighSideSpritesRightNotFar2;
                offsetX = gkOffsetX;
                offsetY = gkOffsetY;
                oamSetMetaSpriteXYEx();
                offsetIndex += 2;
                if (jumpHighSideRightNotFarXYvalues2[offsetIndex] == 0xFFF) {
                    goalkeeperJumpState = GoalkeeperJumpStateNone;
                }

            } else if (goalkeeperJumpState == GoalkeeperJumpHighSideRight3State) {
                oamId = goalkeeperOamId;
                gkOffsetX += jumpHighSideRightNotFarXYvalues3[offsetIndex];
                gkOffsetY += jumpHighSideRightNotFarXYvalues3[offsetIndex + 1];
                selectedSprites = goalKeeperJumpHighSideSpritesRightNotFar3;
                offsetX = gkOffsetX;
                offsetY = gkOffsetY;
                oamSetMetaSpriteXYEx();
                offsetIndex += 2;
                if (jumpHighSideRightNotFarXYvalues3[offsetIndex] == 0xFFF) {
                    goalkeeperJumpState = GoalkeeperJumpStateNone;
                }

            } else if (goalkeeperJumpState == GoalkeeperJumpHighSideRight4State) {
                oamId = goalkeeperOamId;
                gkOffsetX += jumpHighSideRightNotFarXYvalues4[offsetIndex];
                gkOffsetY += jumpHighSideRightNotFarXYvalues4[offsetIndex + 1];
                selectedSprites = goalKeeperJumpHighSideSpritesRightNotFar4;
                offsetX = gkOffsetX;
                offsetY = gkOffsetY;
                oamSetMetaSpriteXYEx();
                offsetIndex += 2;
                if (jumpHighSideRightNotFarXYvalues4[offsetIndex] == 0xFFF) {
                    goalkeeperJumpState = GoalkeeperJumpStateNone;
                }

            } else if (goalkeeperJumpState == GoalkeeperJumpHighSideRight5State) {
                oamId = goalkeeperOamId;
                gkOffsetX += jumpHighSideRightNotFarXYvalues5[offsetIndex];
                gkOffsetY += jumpHighSideRightNotFarXYvalues5[offsetIndex + 1];
                selectedSprites = goalKeeperJumpHighSideSpritesRightNotFar5;
                offsetX = gkOffsetX;
                offsetY = gkOffsetY;
                oamSetMetaSpriteEx();
                offsetIndex += 2;
                if (jumpHighSideRightNotFarXYvalues5[offsetIndex] == 0xFFF) {
                    goalkeeperJumpState = GoalkeeperJumpStateNone;
                }

            } else if (goalkeeperJumpState == GoalkeeperJumpHighSideRight6State) {
                oamId = goalkeeperOamId;
                gkOffsetX += jumpHighSideRightNotFarXYvalues6[offsetIndex];
                gkOffsetY += jumpHighSideRightNotFarXYvalues6[offsetIndex + 1];
                selectedSprites = goalKeeperJumpHighSideSpritesRightNotFar6;
                offsetX = gkOffsetX;
                offsetY = gkOffsetY;
                oamSetMetaSpriteEx();
                offsetIndex += 2;
                if (jumpHighSideRightNotFarXYvalues6[offsetIndex] == 0xFFF) {
                    goalkeeperJumpState = GoalkeeperJumpStateNone;
                }
            }

            gkX = selectedSprites[0] + gkOffsetX;
            gkY = selectedSprites[1] + gkOffsetY;

            if (goalkeeperFramesCounter == 37) {
                goalkeeperState = GoalkeeperStateWait;
                goalkeeperFramesCounter = 0;
            }
            break;

        case GoalkeeperStateJumpHighSideLeft:
            if (goalkeeperFramesCounter == 0) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperJumpHighSideSpritesLeft1;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512 * 15);
                refreshGoalkeeper = 1;
                previousGoalkeeperState = goalkeeperState;

            } else if (goalkeeperFramesCounter == 1) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperJumpHighSideSprites2;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512 * 21);
                refreshGoalkeeper = 1;

            } else if (goalkeeperFramesCounter == 2) {
                goalkeeperJumpState = GoalkeeperJumpHighSide2State;
                gkOffsetX = 0;
                gkOffsetY = 0;
                offsetIndex = 0;

            } else if (goalkeeperFramesCounter == 4) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperJumpHighSideSprites3;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512 * 22);
                refreshGoalkeeper = 1;

            } else if (goalkeeperFramesCounter == 5) {
                goalkeeperJumpState = GoalkeeperJumpHighSide3State;
                gkOffsetX = 0;
                gkOffsetY = 0;
                offsetIndex = 0;

            } else if (goalkeeperFramesCounter == 8) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperJumpHighSideSprites4;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512 * 23);
                refreshGoalkeeper = 1;

            } else if (goalkeeperFramesCounter == 9) {
                goalkeeperJumpState = GoalkeeperJumpHighSide4State;
                gkOffsetX = 0;
                gkOffsetY = 0;
                offsetIndex = 0;

            } else if (goalkeeperFramesCounter == 12) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperJumpHighSideSprites5;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512 * 24);
                refreshGoalkeeper = 1;

            } else if (goalkeeperFramesCounter == 13) {
                goalkeeperJumpState = GoalkeeperJumpHighSide5State;
                gkOffsetX = 0;
                gkOffsetY = 0;
                offsetIndex = 0;

            } else if (goalkeeperFramesCounter == 27) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperJumpHighSideSprites6;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512 * 25);
                refreshGoalkeeper = 1;

            } else if (goalkeeperFramesCounter == 28) {
                goalkeeperJumpState = GoalkeeperJumpHighSide6State;
                gkOffsetX = 0;
                gkOffsetY = 0;
                offsetIndex = 0;

            } else if (goalkeeperFramesCounter == 29) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperJumpHighSideSprites7;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512 * 20);
                refreshGoalkeeper = 1;
            }

            goalkeeperFramesCounter++;

            if (goalkeeperJumpState == GoalkeeperJumpHighSide2State) {
                oamId = goalkeeperOamId;
                gkOffsetX += jumpHighSideXYvalues2[offsetIndex];
                gkOffsetY += jumpHighSideXYvalues2[offsetIndex + 1];
                selectedSprites = goalKeeperJumpHighSideSprites2;
                offsetX = gkOffsetX;
                offsetY = gkOffsetY;
                oamSetMetaSpriteXYEx();
                offsetIndex += 2;
                if (jumpHighSideXYvalues2[offsetIndex] == 0xFFF) {
                    goalkeeperJumpState = GoalkeeperJumpStateNone;
                }

            } else if (goalkeeperJumpState == GoalkeeperJumpHighSide3State) {
                oamId = goalkeeperOamId;
                gkOffsetX += jumpHighSideXYvalues3[offsetIndex];
                gkOffsetY += jumpHighSideXYvalues3[offsetIndex + 1];
                selectedSprites = goalKeeperJumpHighSideSprites3;
                offsetX = gkOffsetX;
                offsetY = gkOffsetY;
                oamSetMetaSpriteXYEx();
                offsetIndex += 2;
                if (jumpHighSideXYvalues3[offsetIndex] == 0xFFF) {
                    goalkeeperJumpState = GoalkeeperJumpStateNone;
                }

            } else if (goalkeeperJumpState == GoalkeeperJumpHighSide4State) {
                oamId = goalkeeperOamId;
                gkOffsetX += jumpHighSideXYvalues4[offsetIndex];
                gkOffsetY += jumpHighSideXYvalues4[offsetIndex + 1];
                selectedSprites = goalKeeperJumpHighSideSprites4;
                offsetX = gkOffsetX;
                offsetY = gkOffsetY;
                oamSetMetaSpriteXYEx();
                offsetIndex += 2;
                if (jumpHighSideXYvalues4[offsetIndex] == 0xFFF) {
                    goalkeeperJumpState = GoalkeeperJumpStateNone;
                }

            } else if (goalkeeperJumpState == GoalkeeperJumpHighSide5State) {
                oamId = goalkeeperOamId;
                gkOffsetX += jumpHighSideXYvalues5[offsetIndex];
                gkOffsetY += jumpHighSideXYvalues5[offsetIndex + 1];
                selectedSprites = goalKeeperJumpHighSideSprites5;
                offsetX = gkOffsetX;
                offsetY = gkOffsetY;
                oamSetMetaSpriteEx();
                offsetIndex += 2;
                if (jumpHighSideXYvalues5[offsetIndex] == 0xFFF) {
                    goalkeeperJumpState = GoalkeeperJumpStateNone;
                }

            } else if (goalkeeperJumpState == GoalkeeperJumpHighSide6State) {
                oamId = goalkeeperOamId;
                gkOffsetX += jumpHighSideXYvalues6[offsetIndex];
                gkOffsetY += jumpHighSideXYvalues6[offsetIndex + 1];
                selectedSprites = goalKeeperJumpHighSideSprites6;
                offsetX = gkOffsetX;
                offsetY = gkOffsetY;
                oamSetMetaSpriteEx();
                offsetIndex += 2;
                if (jumpHighSideXYvalues6[offsetIndex] == 0xFFF) {
                    goalkeeperJumpState = GoalkeeperJumpStateNone;
                }
            }

            gkX = selectedSprites[0] + gkOffsetX;
            gkY = selectedSprites[1] + gkOffsetY;

            if (goalkeeperFramesCounter == 30) {
                goalkeeperState = GoalkeeperStateWait;
                goalkeeperFramesCounter = 0;
            }
            break;

        case GoalkeeperStateJumpLowSideLeftNotFar:
            if (goalkeeperFramesCounter == 0) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperJumpLowLeftSideNotFarSprites1;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512 * 15);
                refreshGoalkeeper = 1;
                previousGoalkeeperState = goalkeeperState;

            } else if (goalkeeperFramesCounter == 1) {
                goalkeeperJumpState = GoalkeeperJumpLowSide1State;
                gkOffsetX = 0;
                gkOffsetY = 0;
                offsetIndex = 0;

            } else if (goalkeeperFramesCounter == 2) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperJumpLowLeftSideNotFarSprites2;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512 * 16);
                refreshGoalkeeper = 1;

            } else if (goalkeeperFramesCounter == 3) {
                goalkeeperJumpState = GoalkeeperJumpLowSide2State;
                gkOffsetX = 0;
                gkOffsetY = 0;
                offsetIndex = 0;

            } else if (goalkeeperFramesCounter == 6) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperJumpLowLeftSideNotFarSprites3;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512 * 17);
                refreshGoalkeeper = 1;

            } else if (goalkeeperFramesCounter == 7) {
                goalkeeperJumpState = GoalkeeperJumpLowSide3State;
                gkOffsetX = 0;
                gkOffsetY = 0;
                offsetIndex = 0;

            } else if (goalkeeperFramesCounter == 10) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperJumpLowLeftSideNotFarSprites4;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512 * 18);
                refreshGoalkeeper = 1;

            } else if (goalkeeperFramesCounter == 11) {
                goalkeeperJumpState = GoalkeeperJumpLowSide4State;
                gkOffsetX = 0;
                gkOffsetY = 0;
                offsetIndex = 0;

            } else if (goalkeeperFramesCounter == 14) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperJumpLowLeftSideNotFarSprites5;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512 * 19);
                refreshGoalkeeper = 1;

            } else if (goalkeeperFramesCounter == 15) {
                goalkeeperJumpState = GoalkeeperJumpLowSide5State;
                gkOffsetX = 0;
                gkOffsetY = 0;
                offsetIndex = 0;

            } else if (goalkeeperFramesCounter == 18) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperJumpLowLeftSideNotFarSprites6;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512 * 20);
                refreshGoalkeeper = 1;

            } else if (goalkeeperFramesCounter == 19) {
                goalkeeperJumpState = GoalkeeperJumpLowSide6State;
                gkOffsetX = 0;
                gkOffsetY = 0;
                offsetIndex = 0;
            }

            goalkeeperFramesCounter++;

            if (goalkeeperJumpState == GoalkeeperJumpLowSide1State) {
                oamId = goalkeeperOamId;
                gkOffsetX += jumpLowLeftSideNotFarXYvalues1[offsetIndex];
                gkOffsetY += jumpLowLeftSideNotFarXYvalues1[offsetIndex + 1];
                selectedSprites = goalKeeperJumpLowLeftSideNotFarSprites1;
                offsetX = gkOffsetX;
                offsetY = gkOffsetY;
                oamSetMetaSpriteXYEx();
                offsetIndex += 2;
                if (jumpLowLeftSideNotFarXYvalues1[offsetIndex] == 0xFFF) {
                    goalkeeperJumpState = GoalkeeperJumpStateNone;
                }

            } else if (goalkeeperJumpState == GoalkeeperJumpLowSide2State) {
                oamId = goalkeeperOamId;
                gkOffsetX += jumpLowLeftSideNotFarXYvalues2[offsetIndex];
                gkOffsetY += jumpLowLeftSideNotFarXYvalues2[offsetIndex + 1];
                selectedSprites = goalKeeperJumpLowLeftSideNotFarSprites2;
                offsetX = gkOffsetX;
                offsetY = gkOffsetY;
                oamSetMetaSpriteXYEx();
                offsetIndex += 2;
                if (jumpLowLeftSideNotFarXYvalues2[offsetIndex] == 0xFFF) {
                    goalkeeperJumpState = GoalkeeperJumpStateNone;
                }

            } else if (goalkeeperJumpState == GoalkeeperJumpLowSide3State) {
                oamId = goalkeeperOamId;
                gkOffsetX += jumpLowLeftSideNotFarXYvalues3[offsetIndex];
                gkOffsetY += jumpLowLeftSideNotFarXYvalues3[offsetIndex + 1];
                selectedSprites = goalKeeperJumpLowLeftSideNotFarSprites3;
                offsetX = gkOffsetX;
                offsetY = gkOffsetY;
                oamSetMetaSpriteXYEx();
                offsetIndex += 2;
                if (jumpLowLeftSideNotFarXYvalues3[offsetIndex] == 0xFFF) {
                    goalkeeperJumpState = GoalkeeperJumpStateNone;
                }

            } else if (goalkeeperJumpState == GoalkeeperJumpLowSide4State) {
                oamId = goalkeeperOamId;
                gkOffsetX += jumpLowLeftSideNotFarXYvalues4[offsetIndex];
                gkOffsetY += jumpLowLeftSideNotFarXYvalues4[offsetIndex + 1];
                selectedSprites = goalKeeperJumpLowLeftSideNotFarSprites4;
                offsetX = gkOffsetX;
                offsetY = gkOffsetY;
                oamSetMetaSpriteXYEx();
                offsetIndex += 2;
                if (jumpLowLeftSideNotFarXYvalues4[offsetIndex] == 0xFFF) {
                    goalkeeperJumpState = GoalkeeperJumpStateNone;
                }

            } else if (goalkeeperJumpState == GoalkeeperJumpLowSide5State) {
                oamId = goalkeeperOamId;
                gkOffsetX += jumpLowLeftSideNotFarXYvalues5[offsetIndex];
                gkOffsetY += jumpLowLeftSideNotFarXYvalues5[offsetIndex + 1];
                selectedSprites = goalKeeperJumpLowLeftSideNotFarSprites5;
                offsetX = gkOffsetX;
                offsetY = gkOffsetY;
                oamSetMetaSpriteXYEx();
                offsetIndex += 2;
                if (jumpLowLeftSideNotFarXYvalues5[offsetIndex] == 0xFFF) {
                    goalkeeperJumpState = GoalkeeperJumpStateNone;
                }

            } else if (goalkeeperJumpState == GoalkeeperJumpLowSide6State) {
                oamId = goalkeeperOamId;
                gkOffsetX += jumpLowLeftSideNotFarXYvalues6[offsetIndex];
                gkOffsetY += jumpLowLeftSideNotFarXYvalues6[offsetIndex + 1];
                selectedSprites = goalKeeperJumpLowLeftSideNotFarSprites6;
                offsetX = gkOffsetX;
                offsetY = gkOffsetY;
                oamSetMetaSpriteEx();
                offsetIndex += 2;
                if (jumpLowLeftSideNotFarXYvalues6[offsetIndex] == 0xFFF) {
                    goalkeeperJumpState = GoalkeeperJumpStateNone;

                    goalkeeperState = GoalkeeperStateWait;
                    goalkeeperFramesCounter = 0;
                    break;
                }
            }

            gkX = selectedSprites[0] + gkOffsetX;
            gkY = selectedSprites[1] + gkOffsetY;

            break;

        case GoalkeeperStateJumpLowSideLeft:
            if (goalkeeperFramesCounter == 0) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperJumpLowLeftSideSprites1;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512 * 15);
                refreshGoalkeeper = 1;
                previousGoalkeeperState = goalkeeperState;

            } else if (goalkeeperFramesCounter == 1) {
                goalkeeperJumpState = GoalkeeperJumpLowSide1State;
                gkOffsetX = 0;
                gkOffsetY = 0;
                offsetIndex = 0;

            } else if (goalkeeperFramesCounter == 3) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperJumpLowLeftSideSprites2;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512 * 16);
                refreshGoalkeeper = 1;

            } else if (goalkeeperFramesCounter == 4) {
                goalkeeperJumpState = GoalkeeperJumpLowSide2State;
                gkOffsetX = 0;
                gkOffsetY = 0;
                offsetIndex = 0;

            } else if (goalkeeperFramesCounter == 7) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperJumpLowLeftSideSprites3;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512 * 17);
                refreshGoalkeeper = 1;

            } else if (goalkeeperFramesCounter == 8) {
                goalkeeperJumpState = GoalkeeperJumpLowSide3State;
                gkOffsetX = 0;
                gkOffsetY = 0;
                offsetIndex = 0;

            } else if (goalkeeperFramesCounter == 11) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperJumpLowLeftSideSprites4;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512 * 18);
                refreshGoalkeeper = 1;

            } else if (goalkeeperFramesCounter == 12) {
                goalkeeperJumpState = GoalkeeperJumpLowSide4State;
                gkOffsetX = 0;
                gkOffsetY = 0;
                offsetIndex = 0;

            } else if (goalkeeperFramesCounter == 17) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperJumpLowLeftSideSprites5;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512 * 19);
                refreshGoalkeeper = 1;

            } else if (goalkeeperFramesCounter == 18) {
                goalkeeperJumpState = GoalkeeperJumpLowSide5State;
                gkOffsetX = 0;
                gkOffsetY = 0;
                offsetIndex = 0;

            } else if (goalkeeperFramesCounter == 21) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperJumpLowLeftSideSprites6;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512 * 20);
                refreshGoalkeeper = 1;

            } else if (goalkeeperFramesCounter == 22) {
                goalkeeperJumpState = GoalkeeperJumpLowSide6State;
                gkOffsetX = 0;
                gkOffsetY = 0;
                offsetIndex = 0;
            }

            goalkeeperFramesCounter++;

            if (goalkeeperJumpState == GoalkeeperJumpLowSide1State) {
                oamId = goalkeeperOamId;
                gkOffsetX += jumpLowLeftSideXYvalues1[offsetIndex];
                gkOffsetY += jumpLowLeftSideXYvalues1[offsetIndex + 1];
                selectedSprites = goalKeeperJumpLowLeftSideSprites1;
                offsetX = gkOffsetX;
                offsetY = gkOffsetY;
                oamSetMetaSpriteXYEx();
                offsetIndex += 2;
                if (jumpLowLeftSideXYvalues1[offsetIndex] == 0xFFF) {
                    goalkeeperJumpState = GoalkeeperJumpStateNone;
                }

            } else if (goalkeeperJumpState == GoalkeeperJumpLowSide2State) {
                oamId = goalkeeperOamId;
                gkOffsetX += jumpLowLeftSideXYvalues2[offsetIndex];
                gkOffsetY += jumpLowLeftSideXYvalues2[offsetIndex + 1];
                selectedSprites = goalKeeperJumpLowLeftSideSprites2;
                offsetX = gkOffsetX;
                offsetY = gkOffsetY;
                oamSetMetaSpriteXYEx();
                offsetIndex += 2;
                if (jumpLowLeftSideXYvalues2[offsetIndex] == 0xFFF) {
                    goalkeeperJumpState = GoalkeeperJumpStateNone;
                }

            } else if (goalkeeperJumpState == GoalkeeperJumpLowSide3State) {
                oamId = goalkeeperOamId;
                gkOffsetX += jumpLowLeftSideXYvalues3[offsetIndex];
                gkOffsetY += jumpLowLeftSideXYvalues3[offsetIndex + 1];
                selectedSprites = goalKeeperJumpLowLeftSideSprites3;
                offsetX = gkOffsetX;
                offsetY = gkOffsetY;
                oamSetMetaSpriteXYEx();
                offsetIndex += 2;
                if (jumpLowLeftSideXYvalues3[offsetIndex] == 0xFFF) {
                    goalkeeperJumpState = GoalkeeperJumpStateNone;
                }

            } else if (goalkeeperJumpState == GoalkeeperJumpLowSide4State) {
                oamId = goalkeeperOamId;
                gkOffsetX += jumpLowLeftSideXYvalues4[offsetIndex];
                gkOffsetY += jumpLowLeftSideXYvalues4[offsetIndex + 1];
                selectedSprites = goalKeeperJumpLowLeftSideSprites4;
                offsetX = gkOffsetX;
                offsetY = gkOffsetY;
                oamSetMetaSpriteXYEx();
                offsetIndex += 2;
                if (jumpLowLeftSideXYvalues4[offsetIndex] == 0xFFF) {
                    goalkeeperJumpState = GoalkeeperJumpStateNone;
                }

            } else if (goalkeeperJumpState == GoalkeeperJumpLowSide5State) {
                oamId = goalkeeperOamId;
                gkOffsetX += jumpLowLeftSideXYvalues5[offsetIndex];
                gkOffsetY += jumpLowLeftSideXYvalues5[offsetIndex + 1];
                selectedSprites = goalKeeperJumpLowLeftSideSprites5;
                offsetX = gkOffsetX;
                offsetY = gkOffsetY;
                oamSetMetaSpriteXYEx();
                offsetIndex += 2;
                if (jumpLowLeftSideXYvalues5[offsetIndex] == 0xFFF) {
                    goalkeeperJumpState = GoalkeeperJumpStateNone;
                }

            } else if (goalkeeperJumpState == GoalkeeperJumpLowSide6State) {
                oamId = goalkeeperOamId;
                gkOffsetX += jumpLowLeftSideXYvalues6[offsetIndex];
                gkOffsetY += jumpLowLeftSideXYvalues6[offsetIndex + 1];
                selectedSprites = goalKeeperJumpLowLeftSideSprites6;
                offsetX = gkOffsetX;
                offsetY = gkOffsetY;
                oamSetMetaSpriteEx(); // Workaround for Z order
                offsetIndex += 2;
                if (jumpLowLeftSideXYvalues6[offsetIndex] == 0xFFF) {
                    goalkeeperJumpState = GoalkeeperJumpStateNone;

                    goalkeeperState = GoalkeeperStateWait;
                    goalkeeperFramesCounter = 0;
                }
            }

            gkX = selectedSprites[0] + gkOffsetX;
            gkY = selectedSprites[1] + gkOffsetY;

            break;

        case GoalkeeperStateJumpHigh:
            if (goalkeeperFramesCounter == 0) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperJumpHighSprites1;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512 * 10);
                refreshGoalkeeper = 1;
                previousGoalkeeperState = goalkeeperState;

            } else if (goalkeeperFramesCounter == 8) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperJumpHighSprites2;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512 * 11);
                refreshGoalkeeper = 1;

            } else if (goalkeeperFramesCounter == 16) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperJumpHighSprites3;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512 * 12);
                refreshGoalkeeper = 1;

            } else if (goalkeeperFramesCounter == 24) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperJumpHighSprites4;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512 * 13);
                refreshGoalkeeper = 1;

            } else if (goalkeeperFramesCounter == 25) {
                goalkeeperJumpState = GoalkeeperJumpHigh4State;
                gkOffsetX = 0;
                gkOffsetY = 0;
                offsetIndex = 0;

            } else if (goalkeeperFramesCounter == 32) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperJumpHighSprites5;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512 * 14);
                refreshGoalkeeper = 1;

            } else if (goalkeeperFramesCounter == 33) {
                goalkeeperJumpState = GoalkeeperJumpHigh5State;
                gkOffsetX = 0;
                gkOffsetY = 0;
                offsetIndex = 0;
            }

            goalkeeperFramesCounter++;

            if (goalkeeperJumpState == GoalkeeperJumpHigh4State) {
                oamId = goalkeeperOamId;
                gkOffsetX += jumpHighXYvalues4[offsetIndex];
                gkOffsetY += jumpHighXYvalues4[offsetIndex + 1];
                selectedSprites = goalKeeperJumpHighSprites4;
                offsetX = gkOffsetX;
                offsetY = gkOffsetY;
                oamSetMetaSpriteXYEx();
                offsetIndex += 2;
                if (jumpHighXYvalues4[offsetIndex] == 0xFFF) {
                    goalkeeperJumpState = GoalkeeperJumpStateNone;
                }

            } else if (goalkeeperJumpState == GoalkeeperJumpHigh5State) {
                oamId = goalkeeperOamId;
                gkOffsetX += jumpHighXYvalues5[offsetIndex];
                gkOffsetY += jumpHighXYvalues5[offsetIndex + 1];
                selectedSprites = goalKeeperJumpHighSprites5;
                offsetX = gkOffsetX;
                offsetY = gkOffsetY;
                oamSetMetaSpriteEx();
                offsetIndex += 2;
                if (jumpHighXYvalues5[offsetIndex] == 0xFFF) {
                    goalkeeperJumpState = GoalkeeperJumpStateNone;
                    goalkeeperState = GoalkeeperStateBackToFloor;
                    goalkeeperFramesCounter = 0;
                }
            }

            gkX = selectedSprites[0] + gkOffsetX;
            gkY = selectedSprites[1] + gkOffsetY;

            break;

        case GoalkeeperStateBackToFloor:
            if (goalkeeperFramesCounter == 0) {
                oamId = goalkeeperOamId;
                selectedSprites = goalKeeperJumpHighSprites2;
                oamSetMetaSprite();
                goalkeeperSprites = &goalkeeperPic + (512 * 11);
                refreshGoalkeeper = 1;

                previousGoalkeeperState = goalkeeperState;
                goalkeeperState = GoalkeeperStateWait;
                goalkeeperFramesCounter = 0;
            }
            break;

        case GoalkeeperStateWait:
            if (goalkeeperFramesCounter == 32) {
                goalkeeperState = GoalkeeperStateReact;
                
            } else {
                goalkeeperFramesCounter++;
            }
            break;

        case GoalkeeperStateReact:
            if (scoreState == 1) {
                if (previousGoalkeeperState == GoalkeeperStateJumpLowSideRight 
                    || previousGoalkeeperState == GoalkeeperStateJumpHighSideRight) {
                    gkOffsetX = 70;
                    gkOffsetY = 0;

                } else if (previousGoalkeeperState == GoalkeeperStateJumpLowSideLeft 
                    || previousGoalkeeperState == GoalkeeperStateJumpHighSideLeft) {
                    gkOffsetX = -70;
                    gkOffsetY = 0;

                } else if (previousGoalkeeperState == GoalkeeperStateJumpLowSideLeftNotFar) {
                    gkOffsetX = -40;
                    gkOffsetY = 0;

                } else if (previousGoalkeeperState == GoalkeeperStateJumpHighSideRightNotFar) {
                    gkOffsetX = 40;
                    gkOffsetY = 0;

                } else {
                    gkOffsetX = 0;
                    gkOffsetY = 0;
                }
        
                goalkeeperState = GoalkeeperStateSad;
                goalkeeperFramesCounter = 0;

            } else if (scoreState == 2) {
                if (previousGoalkeeperState == GoalkeeperStateJumpLow 
                    || previousGoalkeeperState == GoalkeeperStateBackToFloor) {
                    goalkeeperState = GoalkeeperStateHappy;
                    goalkeeperFramesCounter = 0;
                    gkOffsetX = 0;
                    gkOffsetY = 0;

                } else if (previousGoalkeeperState == GoalkeeperStateJumpLowSideRight 
                    || previousGoalkeeperState == GoalkeeperStateJumpHighSideRight) {
                    goalkeeperState = GoalkeeperStateHappy;
                    goalkeeperFramesCounter = 0;
                    gkOffsetX = 70;
                    gkOffsetY = 0;

                } else if (previousGoalkeeperState == GoalkeeperStateJumpHighSideRightNotFar) {
                    goalkeeperState = GoalkeeperStateHappy;
                    goalkeeperFramesCounter = 0;
                    gkOffsetX = 40;
                    gkOffsetY = 0;

                } else if (previousGoalkeeperState == GoalkeeperStateJumpLowSideLeft 
                    || previousGoalkeeperState == GoalkeeperStateJumpHighSideLeft) {
                    goalkeeperState = GoalkeeperStateHappy;
                    goalkeeperFramesCounter = 0;
                    gkOffsetX = -70;
                    gkOffsetY = 0;

                } else if (previousGoalkeeperState == GoalkeeperStateJumpLowSideLeftNotFar) {
                    goalkeeperState = GoalkeeperStateHappy;
                    goalkeeperFramesCounter = 0;
                    gkOffsetX = -23;
                    gkOffsetY = 0;
                }
            }
            break;

        case GoalkeeperStateSad:
            if (goalkeeperFramesCounter == 0) {
                selectedSprites = goalKeeperSadSprites1;

                oamId = goalkeeperOamId;
                offsetX = gkOffsetX;
                offsetY = gkOffsetY;
                oamSetMetaSpriteEx();

                goalkeeperSprites = &goalkeeperPic + (512*26);
                refreshGoalkeeper = 1;

            } else if (goalkeeperFramesCounter == 4) {
                selectedSprites = goalKeeperSadSprites2;

                oamId = goalkeeperOamId;
                offsetX = gkOffsetX;
                offsetY = gkOffsetY;
                oamSetMetaSpriteEx();

                goalkeeperSprites = &goalkeeperPic + (512*27);
                refreshGoalkeeper = 1;

            } else if (goalkeeperFramesCounter == 8) {
                selectedSprites = goalKeeperSadSprites3;

                oamId = goalkeeperOamId;
                offsetX = gkOffsetX;
                offsetY = gkOffsetY;
                oamSetMetaSpriteEx();

                goalkeeperSprites = &goalkeeperPic + (512*28);
                refreshGoalkeeper = 1;

                goalkeeperState = GoalkeeperStateEnd;
            }

            goalkeeperFramesCounter++;

            break;

        case GoalkeeperStateHappyMirrored:
            if (goalkeeperFramesCounter == 0) {
                selectedSprites = goalKeeperHappyMirroredSprites1;

                oamId = goalkeeperOamId;
                offsetX = gkOffsetX;
                offsetY = gkOffsetY;
                oamSetMetaSpriteEx();

                goalkeeperSprites = &goalkeeperPic + (512*7);
                refreshGoalkeeper = 1;

            } else if (goalkeeperFramesCounter == 8) {
                selectedSprites = goalKeeperHappyMirroredSprites2;

                oamId = goalkeeperOamId;
                offsetX = gkOffsetX;
                offsetY = gkOffsetY;
                oamSetMetaSpriteEx();

                goalkeeperSprites = &goalkeeperPic + (512*8);
                refreshGoalkeeper = 1;

            } else if (goalkeeperFramesCounter == 16) {
                selectedSprites = goalKeeperHappyMirroredSprites3;

                oamId = goalkeeperOamId;
                offsetX = gkOffsetX;
                offsetY = gkOffsetY;
                oamSetMetaSpriteEx();

                goalkeeperSprites = &goalkeeperPic + (512*9);
                refreshGoalkeeper = 1;
            }

            goalkeeperFramesCounter++;
            break;

        case GoalkeeperStateHappy:
            if (goalkeeperFramesCounter == 0) {
                selectedSprites = goalKeeperHappySprites1;

                oamId = goalkeeperOamId;
                offsetX = gkOffsetX;
                offsetY = gkOffsetY;
                oamSetMetaSpriteEx();

                goalkeeperSprites = &goalkeeperPic + (512*7);
                refreshGoalkeeper = 1;

            } else if (goalkeeperFramesCounter == 8) {
                selectedSprites = goalKeeperHappySprites2;

                oamId = goalkeeperOamId;
                offsetX = gkOffsetX;
                offsetY = gkOffsetY;
                oamSetMetaSpriteEx();

                goalkeeperSprites = &goalkeeperPic + (512*8);
                refreshGoalkeeper = 1;

            } else if (goalkeeperFramesCounter == 16) {
                selectedSprites = goalKeeperHappySprites3;

                oamId = goalkeeperOamId;
                offsetX = gkOffsetX;
                offsetY = gkOffsetY;
                oamSetMetaSpriteEx();

                goalkeeperSprites = &goalkeeperPic + (512*9);
                refreshGoalkeeper = 1;

                goalkeeperState = GoalkeeperStateEnd;
            }

            goalkeeperFramesCounter++;
            break;
    }
}

/*!\brief Check a collision between two sprites.
    \param sprite1_x the sprite 1's X position
    \param sprite1_y the sprite 1's Y position
    \param sprite1_size the sprite 1's size
    \param sprite2_x the sprite 2's X position
    \param sprite2_y the sprite 2's Y position
    \param sprite2_size the sprite 2's size
*/
bool isCollided(
    u16 sprite1_x, 
    u16 sprite1_y, 
    u16 sprite1_size, 
    u16 sprite2_x, 
    u16 sprite2_y, 
    u16 sprite2_size) {
    return (sprite2_x < (sprite1_x + sprite1_size)) &&
        ((sprite2_x + sprite2_size) > sprite1_x ) &&
        (sprite2_y < (sprite1_y + sprite1_size)) &&
        ((sprite2_y + sprite2_size) > sprite1_y);
}

/*!\brief Set the ball in the OAM.
*/
void oamSetBall() {
    switch(ballState) {
        case BallStatePreMoveHighLeft:
            if (stopBallOffset != 0xFFFF) {
                selectedSprites = ballSpritesHighRight + ballOffsetIndex;
                oamId = ballOamId;
                oamSetSpriteInvertX();
                stopBallOffset = ballSpritesHighRight[ballOffsetIndex + 6];
                ballOffsetIndex += 6;

                ballX = 256 - selectedSprites[0] - 16;
                ballY = selectedSprites[1];
            }

            // Big shadow

            if (showTinyShadow == 0 && stopBallshadowOffset != 0xFFF) {
                selectedSprites = ballshadowBigSprites;
                oamId = 54<<2;
                ballshadowOffsetX += ballshadowBigSpritesXYHighRight[ballshadowOffsetIndex];
                ballshadowOffsetY += ballshadowBigSpritesXYHighRight[ballshadowOffsetIndex + 1];
                stopBallshadowOffset = ballshadowBigSpritesXYHighRight[ballshadowOffsetIndex + 2];
                offsetX = ballshadowOffsetX;
                offsetY = ballshadowOffsetY;
                oamSetMetaSpriteXYInvertXEx();
                ballshadowOffsetIndex += 2;

            } else if (showTinyShadow == 0 && stopBallshadowOffset == 0xFFF) {
                showTinyShadow = 1;
                loadSprite = 1;
                stopBallshadowOffset = 0;
                ballshadowOffsetX = 0;
                ballshadowOffsetY = 0;
                ballshadowOffsetIndex = 0;
            }

            // Tiny shadow

            if (showTinyShadow == 1 && stopBallshadowOffset != 0xFFF) {
                if (loadSprite == 1) {
                    selectedSprites = ballshadowTinySprites;
                    oamId = 54<<2;
                    oamSetMetaSprite();
                    loadSprite = 0;
                }

                selectedSprites = ballshadowTinySprites;
                oamId = 54<<2;
                ballshadowOffsetX += ballshadowTinySpritesXYHighRight[ballshadowOffsetIndex];
                ballshadowOffsetY += ballshadowTinySpritesXYHighRight[ballshadowOffsetIndex + 1];
                stopBallshadowOffset = ballshadowTinySpritesXYHighRight[ballshadowOffsetIndex + 2];
                offsetX = ballshadowOffsetX;
                offsetY = ballshadowOffsetY;
                oamSetSpriteXYInvertXEx();
                ballshadowOffsetIndex += 2;
            }

            if (stopBallOffset == 0xFFFF) {
                if (previousGoalkeeperState == GoalkeeperStateJumpHighSideLeft 
                    && isCollided(ballX, ballY, 8, gkX, gkY, 48) == 1) {
                    ballState = BallStateMoveHighLeftBlocked;
                    scoreState = 2;
                    
                } else {
                    ballState = BallStateMoveHighLeftNotBlocked;
                    scoreState = 1;
                    animNet = 1;
                }

                stopBallOffset = 0;
                stopBallshadowOffset = 0;
                ballOffsetIndex = 0;
                ballshadowOffsetIndex = 0;
            }
            break;

        case BallStateMoveHighLeftBlocked:
            if (stopBallOffset != 0xFFFF) {
                selectedSprites = ballSpritesHighRightBlocked + ballOffsetIndex;
                oamId = ballOamId;
                oamSetSpriteInvertX();
                stopBallOffset = ballSpritesHighRightBlocked[ballOffsetIndex + 6];
                ballOffsetIndex += 6;

            } else {
                oamSetEx(ballOamId, OBJ_SMALL, OBJ_HIDE);
                ballState = BallStateEnd;
            }

            if (stopBallshadowOffset != 0xFFF) {
                selectedSprites = ballshadowTinySprites;
                oamId = 54<<2;
                ballshadowOffsetX += ballshadowTinySpritesXYHighRightBlocked[ballshadowOffsetIndex];
                ballshadowOffsetY += ballshadowTinySpritesXYHighRightBlocked[ballshadowOffsetIndex + 1];
                stopBallshadowOffset = ballshadowTinySpritesXYHighRightBlocked[ballshadowOffsetIndex + 2];
                offsetX = ballshadowOffsetX;
                offsetY = ballshadowOffsetY;
                oamSetSpriteXYInvertXEx();
                ballshadowOffsetIndex += 2;

            } else {
                oamSetEx(54<<2, OBJ_SMALL, OBJ_HIDE);
            }
            break;

        case BallStateMoveHighLeftNotBlocked:
            if (stopBallOffset != 0xFFFF) {
                selectedSprites = ballSpritesHighRightNotBlocked + ballOffsetIndex;
                oamId = ballOamId;
                oamSetSpriteInvertX();
                stopBallOffset = ballSpritesHighRightNotBlocked[ballOffsetIndex + 6];
                ballOffsetIndex += 6;

            } else {
                ballState = BallStateEnd;
            }

            if (stopBallshadowOffset != 0xFFF) {
                selectedSprites = ballshadowTinySprites;
                oamId = 54<<2;
                ballshadowOffsetX += ballshadowTinySpritesXYHighRightNotBlocked[ballshadowOffsetIndex];
                ballshadowOffsetY += ballshadowTinySpritesXYHighRightNotBlocked[ballshadowOffsetIndex + 1];
                stopBallshadowOffset = ballshadowTinySpritesXYHighRightNotBlocked[ballshadowOffsetIndex + 2];
                offsetX = ballshadowOffsetX;
                offsetY = ballshadowOffsetY;
                oamSetSpriteXYInvertXEx();
                ballshadowOffsetIndex += 2;
            }
            break;

        case BallStatePreMoveHighRight:
            if (stopBallOffset != 0xFFFF) {
                selectedSprites = ballSpritesHighRight + ballOffsetIndex;
                oamId = ballOamId;
                oamSetSprite();
                stopBallOffset = ballSpritesHighRight[ballOffsetIndex + 6];
                ballOffsetIndex += 6;

                ballX = selectedSprites[0];
                ballY = selectedSprites[1];
            }

            // Big shadow

            if (showTinyShadow == 0 && stopBallshadowOffset != 0xFFF) {
                selectedSprites = ballshadowBigSprites;
                oamId = 54<<2;
                ballshadowOffsetX += ballshadowBigSpritesXYHighRight[ballshadowOffsetIndex];
                ballshadowOffsetY += ballshadowBigSpritesXYHighRight[ballshadowOffsetIndex + 1];
                stopBallshadowOffset = ballshadowBigSpritesXYHighRight[ballshadowOffsetIndex + 2];
                offsetX = ballshadowOffsetX;
                offsetY = ballshadowOffsetY;
                oamSetMetaSpriteXYEx();
                ballshadowOffsetIndex += 2;

            } else if (showTinyShadow == 0 && stopBallshadowOffset == 0xFFF) {
                showTinyShadow = 1;
                loadSprite = 1;
                stopBallshadowOffset = 0;
                ballshadowOffsetX = 0;
                ballshadowOffsetY = 0;
                ballshadowOffsetIndex = 0;
            }

            // Tiny shadow

            if (showTinyShadow == 1 && stopBallshadowOffset != 0xFFF) {
                if (loadSprite == 1) {
                    selectedSprites = ballshadowTinySprites;
                    oamId = 54<<2;
                    oamSetMetaSprite();
                    loadSprite = 0;
                }

                selectedSprites = ballshadowTinySprites;
                oamId = 54<<2;
                ballshadowOffsetX += ballshadowTinySpritesXYHighRight[ballshadowOffsetIndex];
                ballshadowOffsetY += ballshadowTinySpritesXYHighRight[ballshadowOffsetIndex + 1];
                stopBallshadowOffset = ballshadowTinySpritesXYHighRight[ballshadowOffsetIndex + 2];
                offsetX = ballshadowOffsetX;
                offsetY = ballshadowOffsetY;
                oamSetSpriteXYEx();
                ballshadowOffsetIndex += 2;
            }

            if (stopBallOffset == 0xFFFF) {
                if (previousGoalkeeperState == GoalkeeperStateJumpHighSideRight 
                    && isCollided(ballX, ballY, 8, gkX, gkY, 48) == 1) {
                    ballState = BallStateMoveHighRightBlocked;
                    scoreState = 2;

                } else {
                    ballState = BallStateMoveHighRightNotBlocked;
                    scoreState = 1;
                    animNet = 1;
                }

                stopBallOffset = 0;
                stopBallshadowOffset = 0;
                ballOffsetIndex = 0;
                ballshadowOffsetIndex = 0;
            }
            break;

        case BallStateMoveHighRightBlocked:
            if (stopBallOffset != 0xFFFF) {
                selectedSprites = ballSpritesHighRightBlocked + ballOffsetIndex;
                oamId = ballOamId;
                oamSetSprite();
                stopBallOffset = ballSpritesHighRightBlocked[ballOffsetIndex + 6];
                ballOffsetIndex += 6;

            } else {
                oamSetEx(ballOamId, OBJ_SMALL, OBJ_HIDE);
                ballState = BallStateEnd;
            }

            if (stopBallshadowOffset != 0xFFF) {
                selectedSprites = ballshadowTinySprites;
                oamId = 54<<2;
                ballshadowOffsetX += ballshadowTinySpritesXYHighRightBlocked[ballshadowOffsetIndex];
                ballshadowOffsetY += ballshadowTinySpritesXYHighRightBlocked[ballshadowOffsetIndex + 1];
                stopBallshadowOffset = ballshadowTinySpritesXYHighRightBlocked[ballshadowOffsetIndex + 2];
                offsetX = ballshadowOffsetX;
                offsetY = ballshadowOffsetY;
                oamSetSpriteXYEx();
                ballshadowOffsetIndex += 2;

            } else {
                oamSetEx(54<<2, OBJ_SMALL, OBJ_HIDE);
            }
            break;

        case BallStateMoveHighRightNotBlocked:
            if (stopBallOffset != 0xFFFF) {
                selectedSprites = ballSpritesHighRightNotBlocked + ballOffsetIndex;
                oamId = ballOamId;
                oamSetSprite();
                stopBallOffset = ballSpritesHighRightNotBlocked[ballOffsetIndex + 6];
                ballOffsetIndex += 6;

            } else {
                ballState = BallStateEnd;
            }

            if (stopBallshadowOffset != 0xFFF) {
                selectedSprites = ballshadowTinySprites;
                oamId = 54<<2;
                ballshadowOffsetX += ballshadowTinySpritesXYHighRightNotBlocked[ballshadowOffsetIndex];
                ballshadowOffsetY += ballshadowTinySpritesXYHighRightNotBlocked[ballshadowOffsetIndex + 1];
                stopBallshadowOffset = ballshadowTinySpritesXYHighRightNotBlocked[ballshadowOffsetIndex + 2];
                offsetX = ballshadowOffsetX;
                offsetY = ballshadowOffsetY;
                oamSetSpriteXYEx();
                ballshadowOffsetIndex += 2;
            }
            break;

        case BallStatePreMoveHigh:
            if (stopBallOffset != 0xFFFF) {
                selectedSprites = ballSpritesHigh + ballOffsetIndex;
                oamId = ballOamId;
                oamSetSprite();
                stopBallOffset = ballSpritesHigh[ballOffsetIndex + 6];
                ballOffsetIndex += 6;

                ballX = selectedSprites[0];
                ballY = selectedSprites[1];
            }

            // Big shadow

            if (showTinyShadow == 0 && stopBallshadowOffset != 0xFFF) {
                selectedSprites = ballshadowBigSprites;
                oamId = 54<<2;
                ballshadowOffsetX += ballshadowBigSpritesXYHigh[ballshadowOffsetIndex];
                ballshadowOffsetY += ballshadowBigSpritesXYHigh[ballshadowOffsetIndex + 1];
                stopBallshadowOffset = ballshadowBigSpritesXYHigh[ballshadowOffsetIndex + 2];
                offsetX = ballshadowOffsetX;
                offsetY = ballshadowOffsetY;
                oamSetMetaSpriteXYEx();
                ballshadowOffsetIndex += 2;

            } else if (showTinyShadow == 0 && stopBallshadowOffset == 0xFFF) {
                showTinyShadow = 1;
                loadSprite = 1;
                stopBallshadowOffset = 0;
                ballshadowOffsetX = 0;
                ballshadowOffsetY = 0;
                ballshadowOffsetIndex = 0;
            }

            // Tiny shadow

            if (showTinyShadow == 1 && stopBallshadowOffset != 0xFFF) {
                if (loadSprite == 1) {
                    selectedSprites = ballshadowTinySprites;
                    oamId = 54<<2;
                    oamSetMetaSprite();
                    loadSprite = 0;

                } else {
                    selectedSprites = ballshadowTinySprites;
                    oamId = 54<<2;
                    ballshadowOffsetX += ballshadowTinySpritesXYHigh[ballshadowOffsetIndex];
                    ballshadowOffsetY += ballshadowTinySpritesXYHigh[ballshadowOffsetIndex + 1];
                    stopBallshadowOffset = ballshadowTinySpritesXYHigh[ballshadowOffsetIndex + 2];
                    offsetX = ballshadowOffsetX;
                    offsetY = ballshadowOffsetY;
                    oamSetSpriteXYEx();
                    ballshadowOffsetIndex += 2;
                }
            }

            if (stopBallOffset == 0xFFFF) {
                if (previousGoalkeeperState == GoalkeeperStateJumpHigh 
                    && isCollided(ballX, ballY, 8, gkX - 24, gkY - 24, 48) == 1) {
                    ballState = BallStateMoveHighBlocked;
                    scoreState = 2;
                    
                } else {
                    ballState = BallStateMoveHighNotBlocked;
                    scoreState = 1;
                    animNet = 1;
                }

                stopBallOffset = 0;
                stopBallshadowOffset = 0;
                ballOffsetIndex = 0;
                ballshadowOffsetIndex = 0;
            }
            break;

        case BallStateMoveHighNotBlocked:
            if (stopBallOffset != 0xFFFF) {
                selectedSprites = ballSpritesHighNotBlocked + ballOffsetIndex;
                oamId = ballOamId;
                oamSetSprite();
                stopBallOffset = ballSpritesHighNotBlocked[ballOffsetIndex + 6];
                ballOffsetIndex += 6;

            } else {
                ballState = BallStateEnd;
            }

            if (stopBallshadowOffset != 0xFFF) {
                selectedSprites = ballshadowTinySprites;
                oamId = 54<<2;
                ballshadowOffsetX += ballshadowTinySpritesXYHighNotBlocked[ballshadowOffsetIndex];
                ballshadowOffsetY += ballshadowTinySpritesXYHighNotBlocked[ballshadowOffsetIndex + 1];
                stopBallshadowOffset = ballshadowTinySpritesXYHighNotBlocked[ballshadowOffsetIndex + 2];
                offsetX = ballshadowOffsetX;
                offsetY = ballshadowOffsetY;
                oamSetSpriteXYEx();
                ballshadowOffsetIndex += 2;
            }
            break;

        case BallStateMoveHighBlocked:
            if (stopBallOffset != 0xFFFF) {
                selectedSprites = ballSpritesHighBlocked + ballOffsetIndex;
                oamId = ballOamId;
                oamSetSprite();
                stopBallOffset = ballSpritesHighBlocked[ballOffsetIndex + 6];
                ballOffsetIndex += 6;

            } else {
                ballState = BallStateEnd;
            }

            if (stopBallshadowOffset != 0xFFF) {
                selectedSprites = ballshadowTinySprites;
                oamId = 54<<2;
                ballshadowOffsetX += ballshadowTinySpritesXYHighBlocked[ballshadowOffsetIndex];
                ballshadowOffsetY += ballshadowTinySpritesXYHighBlocked[ballshadowOffsetIndex + 1];
                stopBallshadowOffset = ballshadowTinySpritesXYHighBlocked[ballshadowOffsetIndex + 2];
                offsetX = ballshadowOffsetX;
                offsetY = ballshadowOffsetY;
                oamSetSpriteXYEx();
                ballshadowOffsetIndex += 2;
            }
            break;

        case BallStatePreMoveLowLeft:
            if (stopBallOffset != 0xFFFF) {
                selectedSprites = ballSpritesLowRight + ballOffsetIndex;
                oamId = ballOamId;
                oamSetSpriteInvertX();
                stopBallOffset = ballSpritesLowRight[ballOffsetIndex + 6];
                ballOffsetIndex += 6;

                ballX = 256 - selectedSprites[0] - 16;
                ballY = selectedSprites[1];
            }

            // Big shadow

            if (showTinyShadow == 0 && stopBallshadowOffset != 0xFFF) {
                selectedSprites = ballshadowBigSprites;
                oamId = 54<<2;
                ballshadowOffsetX += ballshadowBigSpritesXYLowRight[ballshadowOffsetIndex];
                ballshadowOffsetY += ballshadowBigSpritesXYLowRight[ballshadowOffsetIndex + 1];
                stopBallshadowOffset = ballshadowBigSpritesXYLowRight[ballshadowOffsetIndex + 2];
                offsetX = ballshadowOffsetX;
                offsetY = ballshadowOffsetY;
                oamSetMetaSpriteXYInvertXEx();
                ballshadowOffsetIndex += 2;

            } else if (showTinyShadow == 0 && stopBallshadowOffset == 0xFFF) {
                showTinyShadow = 1;
                loadSprite = 1;
                stopBallshadowOffset = 0;
                ballshadowOffsetX = 0;
                ballshadowOffsetY = 0;
                ballshadowOffsetIndex = 0;
            }

            // Tiny shadow

            if (showTinyShadow == 1 && stopBallshadowOffset != 0xFFF) {
                if (loadSprite == 1) {
                    selectedSprites = ballshadowTinySprites;
                    oamId = 54<<2;
                    oamSetMetaSprite();
                    ballshadowOffsetX = 0;
                    ballshadowOffsetY = 0;
                    ballshadowOffsetIndex = 0;
                    loadSprite = 0;
                }

                selectedSprites = ballshadowTinySprites;
                oamId = 54<<2;
                ballshadowOffsetX += ballshadowTinySpritesXYLowRight[ballshadowOffsetIndex];
                ballshadowOffsetY += ballshadowTinySpritesXYLowRight[ballshadowOffsetIndex + 1];
                stopBallshadowOffset = ballshadowTinySpritesXYLowRight[ballshadowOffsetIndex + 2];
                offsetX = ballshadowOffsetX;
                offsetY = ballshadowOffsetY;
                oamSetSpriteXYInvertXEx();
                ballshadowOffsetIndex += 2;
            }

            if (stopBallOffset == 0xFFFF) {
                if ((previousGoalkeeperState == GoalkeeperStateJumpLowSideLeft
                    || previousGoalkeeperState == GoalkeeperStateJumpLowSideLeftNotFar) 
                    && isCollided(ballX, ballY, 8, gkX - 24, gkY - 24, 48) == 1) {
                    ballState = BallStateMoveLowLeftBlocked;
                    scoreState = 2;

                } else {
                    ballState = BallStateMoveLowLeftNotBlocked;
                    scoreState = 1;
                    animNet = 1;
                }

                stopBallOffset = 0;
                stopBallshadowOffset = 0;
                ballOffsetIndex = 0;
                ballshadowOffsetIndex = 0;
            }
            break;

        case BallStatePreMoveLowRight:
            if (stopBallOffset != 0xFFFF) {
                selectedSprites = ballSpritesLowRight + ballOffsetIndex;
                oamId = ballOamId;
                oamSetSprite();
                stopBallOffset = ballSpritesLowRight[ballOffsetIndex + 6];
                ballOffsetIndex += 6;

                ballX = selectedSprites[0];
                ballY = selectedSprites[1];
            }

            // Big shadow

            if (showTinyShadow == 0 && stopBallshadowOffset != 0xFFF) {
                selectedSprites = ballshadowBigSprites;
                oamId = 54<<2;
                ballshadowOffsetX += ballshadowBigSpritesXYLowRight[ballshadowOffsetIndex];
                ballshadowOffsetY += ballshadowBigSpritesXYLowRight[ballshadowOffsetIndex + 1];
                stopBallshadowOffset = ballshadowBigSpritesXYLowRight[ballshadowOffsetIndex + 2];
                offsetX = ballshadowOffsetX;
                offsetY = ballshadowOffsetY;
                oamSetMetaSpriteXYEx();
                ballshadowOffsetIndex += 2;

            } else if (showTinyShadow == 0 && stopBallshadowOffset == 0xFFF) {
                showTinyShadow = 1;
                loadSprite = 1;
                stopBallshadowOffset = 0;
                ballshadowOffsetX = 0;
                ballshadowOffsetY = 0;
                ballshadowOffsetIndex = 0;
            }

            // Tiny shadow

            if (showTinyShadow == 1 && stopBallshadowOffset != 0xFFF) {
                if (loadSprite == 1) {
                    selectedSprites = ballshadowTinySprites;
                    oamId = 54<<2;
                    oamSetMetaSprite();
                    ballshadowOffsetX = 0;
                    ballshadowOffsetY = 0;
                    ballshadowOffsetIndex = 0;
                    loadSprite = 0;
                }

                selectedSprites = ballshadowTinySprites;
                oamId = 54<<2;
                ballshadowOffsetX += ballshadowTinySpritesXYLowRight[ballshadowOffsetIndex];
                ballshadowOffsetY += ballshadowTinySpritesXYLowRight[ballshadowOffsetIndex + 1];
                stopBallshadowOffset = ballshadowTinySpritesXYLowRight[ballshadowOffsetIndex + 2];
                offsetX = ballshadowOffsetX;
                offsetY = ballshadowOffsetY;
                oamSetSpriteXYEx();
                ballshadowOffsetIndex += 2;
            }
            
            if (stopBallOffset == 0xFFFF) {
                if (previousGoalkeeperState == GoalkeeperStateJumpLowSideRight 
                    && isCollided(ballX, ballY, 8, gkX - 24, gkY - 24, 48) == 1) {
                    ballState = BallStateMoveLowRightBlocked;
                    scoreState = 2;

                } else {
                    ballState = BallStateMoveLowRightNotBlocked;
                    scoreState = 1;
                    animNet = 1;
                }

                stopBallOffset = 0;
                stopBallshadowOffset = 0;
                ballOffsetIndex = 0;
                ballshadowOffsetIndex = 0;
            }

            break;

        case BallStatePreMoveLow:
            if (stopBallOffset != 0xFFFF) {
                selectedSprites = ballSpritesLow + ballOffsetIndex;
                oamId = ballOamId;
                oamSetSprite();
                stopBallOffset = ballSpritesLow[ballOffsetIndex + 6];
                ballOffsetIndex += 6;

                ballX = selectedSprites[0];
                ballY = selectedSprites[1];
            }

            // Big shadow

            if (showTinyShadow == 0 && stopBallshadowOffset != 0xFFF) {
                selectedSprites = ballshadowBigSprites;
                oamId = 54<<2;
                ballshadowOffsetX += ballshadowBigSpritesXYLow[ballshadowOffsetIndex];
                ballshadowOffsetY += ballshadowBigSpritesXYLow[ballshadowOffsetIndex + 1];
                stopBallshadowOffset = ballshadowBigSpritesXYLow[ballshadowOffsetIndex + 2];
                offsetX = ballshadowOffsetX;
                offsetY = ballshadowOffsetY;
                oamSetMetaSpriteXYEx();
                ballshadowOffsetIndex += 2;

            } else if (showTinyShadow == 0 && stopBallshadowOffset == 0xFFF) {
                showTinyShadow = 1;
                loadSprite = 1;
                stopBallshadowOffset = 0;
                ballshadowOffsetX = 0;
                ballshadowOffsetY = 0;
                ballshadowOffsetIndex = 0;
            }
            
            // Tiny shadow

            if (showTinyShadow == 1 && stopBallshadowOffset != 0xFFF) {
                if (loadSprite == 1) {
                    selectedSprites = ballshadowTinySprites;
                    oamId = 54<<2;
                    oamSetMetaSprite();
                    ballshadowOffsetX = 0;
                    ballshadowOffsetY = 0;
                    ballshadowOffsetIndex = 0;
                    loadSprite = 0;

                } else {
                    selectedSprites = ballshadowTinySprites;
                    oamId = 54<<2;
                    ballshadowOffsetX += ballshadowTinySpritesXYLow[ballshadowOffsetIndex];
                    ballshadowOffsetY += ballshadowTinySpritesXYLow[ballshadowOffsetIndex + 1];
                    stopBallshadowOffset = ballshadowTinySpritesXYLow[ballshadowOffsetIndex + 2];
                    offsetX = ballshadowOffsetX;
                    offsetY = ballshadowOffsetY;
                    oamSetSpriteXYEx();
                    ballshadowOffsetIndex += 2;
                }
            }
            
            if (stopBallOffset == 0xFFFF) {
                if (previousGoalkeeperState == GoalkeeperStateJumpLow 
                    && isCollided(ballX, ballY, 8, gkX, gkY, 48) == 1) {
                    ballState = BallStateMoveLowBlocked;
                    scoreState = 2;

                } else {
                    ballState = BallStateMoveLowNotBlocked;
                    scoreState = 1;
                    animNet = 1;
                }

                stopBallOffset = 0;
                stopBallshadowOffset = 0;
                ballOffsetIndex = 0;
                ballshadowOffsetIndex = 0;
            }
            break;

        case BallStateMoveLowLeftNotBlocked:
            if (stopBallOffset != 0xFFFF) {
                selectedSprites = ballTinySpritesLowRightNotBlocked + ballOffsetIndex;
                oamId = ballOamId;
                oamSetSpriteInvertX();
                stopBallOffset = ballTinySpritesLowRightNotBlocked[ballOffsetIndex + 6];
                ballOffsetIndex += 6;

            } else {
                ballState = BallStateEnd;
            }

            if (stopBallshadowOffset != 0xFFF) {
                selectedSprites = ballshadowTinySprites;
                oamId = 54<<2;
                ballshadowOffsetX += ballshadowTinySpritesXYLowRightNotBlocked[ballshadowOffsetIndex];
                ballshadowOffsetY += ballshadowTinySpritesXYLowRightNotBlocked[ballshadowOffsetIndex + 1];
                stopBallshadowOffset = ballshadowTinySpritesXYLowRightNotBlocked[ballshadowOffsetIndex + 2];
                offsetX = ballshadowOffsetX;
                offsetY = ballshadowOffsetY;
                oamSetSpriteXYInvertXEx();
                ballshadowOffsetIndex += 2;
            }
            break;
        
        case BallStateMoveLowRightNotBlocked:
            if (stopBallOffset != 0xFFFF) {
                selectedSprites = ballTinySpritesLowRightNotBlocked + ballOffsetIndex;
                oamId = ballOamId;
                oamSetSprite();
                stopBallOffset = ballTinySpritesLowRightNotBlocked[ballOffsetIndex + 6];
                ballOffsetIndex += 6;

            } else {
                ballState = BallStateEnd;
            }

            if (stopBallshadowOffset != 0xFFF) {
                selectedSprites = ballshadowTinySprites;
                oamId = 54<<2;
                ballshadowOffsetX += ballshadowTinySpritesXYLowRightNotBlocked[ballshadowOffsetIndex];
                ballshadowOffsetY += ballshadowTinySpritesXYLowRightNotBlocked[ballshadowOffsetIndex + 1];
                stopBallshadowOffset = ballshadowTinySpritesXYLowRightNotBlocked[ballshadowOffsetIndex + 2];
                offsetX = ballshadowOffsetX;
                offsetY = ballshadowOffsetY;
                oamSetSpriteXYEx();
                ballshadowOffsetIndex += 2;
            }
            break;

        case BallStateMoveLowNotBlocked:
            if (stopBallOffset != 0xFFFF) {
                selectedSprites = ballTinySpritesLowNotBlocked + ballOffsetIndex;
                oamId = ballOamId;
                oamSetSprite();
                stopBallOffset = ballTinySpritesLowNotBlocked[ballOffsetIndex + 6];
                ballOffsetIndex += 6;

            } else {
                ballState = BallStateEnd;
            }

            if (stopBallshadowOffset != 0xFFF) {
                selectedSprites = ballshadowTinySprites;
                oamId = 54<<2;
                ballshadowOffsetX += ballshadowTinySpritesXYLowNotBlocked[ballshadowOffsetIndex];
                ballshadowOffsetY += ballshadowTinySpritesXYLowNotBlocked[ballshadowOffsetIndex + 1];
                stopBallshadowOffset = ballshadowTinySpritesXYLowNotBlocked[ballshadowOffsetIndex + 2];
                offsetX = ballshadowOffsetX;
                offsetY = ballshadowOffsetY;
                oamSetSpriteXYEx();
                ballshadowOffsetIndex += 2;
            }
            break;
        
        case BallStateMoveLowLeftBlocked:
            if (stopBallOffset != 0xFFFF) {
                selectedSprites = ballTinySpritesLowRightBlocked + ballOffsetIndex;
                oamId = ballOamId;
                oamSetSpriteInvertX();
                stopBallOffset = ballTinySpritesLowRightBlocked[ballOffsetIndex + 6];
                ballOffsetIndex += 6;

            } else {
                oamSetEx(ballOamId, OBJ_SMALL, OBJ_HIDE);
                ballState = BallStateEnd;
            }

            if (stopBallshadowOffset != 0xFFF) {
                selectedSprites = ballshadowTinySprites;
                oamId = 54<<2;
                ballshadowOffsetX += ballshadowTinySpritesXYLowRightBlocked[ballshadowOffsetIndex];
                ballshadowOffsetY += ballshadowTinySpritesXYLowRightBlocked[ballshadowOffsetIndex + 1];
                stopBallshadowOffset = ballshadowTinySpritesXYLowRightBlocked[ballshadowOffsetIndex + 2];
                offsetX = ballshadowOffsetX;
                offsetY = ballshadowOffsetY;
                oamSetSpriteXYInvertXEx();
                ballshadowOffsetIndex += 2;

            } else {
                oamSetEx(54<<2, OBJ_SMALL, OBJ_HIDE);
            }
            break;

        case BallStateMoveLowRightBlocked:
            if (stopBallOffset != 0xFFFF) {
                selectedSprites = ballTinySpritesLowRightBlocked + ballOffsetIndex;
                oamId = ballOamId;
                oamSetSprite();
                stopBallOffset = ballTinySpritesLowRightBlocked[ballOffsetIndex + 6];
                ballOffsetIndex += 6;

            } else {
                oamSetEx(ballOamId, OBJ_SMALL, OBJ_HIDE);
                ballState = BallStateEnd;
            }

            if (stopBallshadowOffset != 0xFFF) {
                selectedSprites = ballshadowTinySprites;
                oamId = 54<<2;
                ballshadowOffsetX += ballshadowTinySpritesXYLowRightBlocked[ballshadowOffsetIndex];
                ballshadowOffsetY += ballshadowTinySpritesXYLowRightBlocked[ballshadowOffsetIndex + 1];
                stopBallshadowOffset = ballshadowTinySpritesXYLowRightBlocked[ballshadowOffsetIndex + 2];
                offsetX = ballshadowOffsetX;
                offsetY = ballshadowOffsetY;
                oamSetSpriteXYEx();
                ballshadowOffsetIndex += 2;

            } else {
                oamSetEx(54<<2, OBJ_SMALL, OBJ_HIDE);
            }
            break;

        case BallStateMoveLowBlocked:
            if (stopBallOffset != 0xFFFF) {
                selectedSprites = ballTinySpritesBlocked + ballOffsetIndex;
                oamId = ballOamId;
                oamSetSprite();
                stopBallOffset = ballTinySpritesBlocked[ballOffsetIndex + 6];
                ballOffsetIndex += 6;

            } else {
                ballState = BallStateEnd;
            }

            if (stopBallshadowOffset != 0xFFF) {
                selectedSprites = ballshadowTinySprites;
                oamId = 54<<2;
                ballshadowOffsetX += ballshadowTinySpritesXYBlocked[ballshadowOffsetIndex];
                ballshadowOffsetY += ballshadowTinySpritesXYBlocked[ballshadowOffsetIndex + 1];
                stopBallshadowOffset = ballshadowTinySpritesXYBlocked[ballshadowOffsetIndex + 2];
                offsetX = ballshadowOffsetX;
                offsetY = ballshadowOffsetY;
                oamSetSpriteXYEx();
                ballshadowOffsetIndex += 2;
            }
            break;
    }
}

/*!\brief Animate the net.
*/
void animateNet() {
    if (netFramesCount == 0) {
        bg1TileMap = stage1Bg1TileMapType2;
        refreshBg1 = 1;

    } else if (netFramesCount == 3) {
        bg1TileMap = stage1Bg1TileMapType3;
        refreshBg1 = 1;

    } else if (netFramesCount == 6) {
        bg1TileMap = stage1Bg1TileMapType4;
        refreshBg1 = 1;

    } else if (netFramesCount == 9) {
        bg1TileMap = stage1Bg1TileMapType5;
        refreshBg1 = 1;

    } else if (netFramesCount == 12) {
        bg1TileMap = stage1Bg1TileMapType6;
        refreshBg1 = 1;

    } else if (netFramesCount == 15) {
        bg1TileMap = stage1Bg1TileMapType1;
        refreshBg1 = 1;

    } else if (netFramesCount == 18) {
        bg1TileMap = stage1Bg1TileMapType6;
        refreshBg1 = 1;

    } else if (netFramesCount == 21) {
        bg1TileMap = stage1Bg1TileMapType5;
        refreshBg1 = 1;

    } else if (netFramesCount == 24) {
        bg1TileMap = stage1Bg1TileMapType4;
        refreshBg1 = 1;

    } else if (netFramesCount == 27) {
        bg1TileMap = stage1Bg1TileMapType3;
        refreshBg1 = 1;

    } else if (netFramesCount == 30) {
        bg1TileMap = stage1Bg1TileMapType2;
        refreshBg1 = 1;

    } else if (netFramesCount == 33) {
        bg1TileMap = stage1Bg1TileMapType1;
        refreshBg1 = 1;
    }

    netFramesCount += 1;

    if (netFramesCount == 36) {
        animNet = 0;
    }
}

/*!\brief Update the Level screen.
*/
void updateLevel() {
    if(ballState == BallStateMoveLowNotBlocked 
        || ballState == BallStateMoveLowLeftNotBlocked 
        || ballState == BallStateMoveLowRightNotBlocked 
        || ballState == BallStateMoveHighNotBlocked
        || ballState == BallStateMoveHighLeftNotBlocked
        || ballState == BallStateMoveHighRightNotBlocked) {
        if (zOrder == 0) {
            oamSet(ballOamId, 0, 0, prio, 0, 0, 0x06F, 0);
            goalkeeperOamId = ballOamId;
            ballOamId = goalkeeperOamId + (goalkeeperSpritesCount<<2);
            zOrder = 1;
        }
    }

    if (animNet == 1) {
        animateNet();
    }

    if (scoreState != 0) {
        if (circleBlinkCounter < 32) {
            if (ballGfxOffsetCounter == 1) {
                ballGfxOffset = 0x144;

            } else if (ballGfxOffsetCounter == 5) {
                ballGfxOffset = 0x140;

            } else if (ballGfxOffsetCounter == 9) {
                ballGfxOffset = 0x142;
            }

            if (ballGfxOffsetCounter == 12) {
                ballGfxOffsetCounter = 0;

            } else {
                ballGfxOffsetCounter++;
            }

            if (circleFrameCounter == 0) {
                oamSetGfxOffset(selectedBallCountryStartOamId, 0x6F);
                oamSetEx(selectedBallCountryStartOamId, OBJ_SMALL, OBJ_SHOW);

            } else if (circleFrameCounter == 1) {
                oamSetGfxOffset(selectedBallCountryStartOamId, ballGfxOffset);
                oamSetEx(selectedBallCountryStartOamId, OBJ_LARGE, OBJ_SHOW);
            }

            circleFrameCounter++;
        
            if (circleFrameCounter == 2) {
                circleFrameCounter = 0;
                circleBlinkCounter += 1;
            }

        } else if (displayBlueCircle == 0) {
            oamSetEx(selectedBallCountryStartOamId, OBJ_LARGE, OBJ_HIDE);
            if (scoreState == 1) {
                dmaCopyVram((u8 *)circleUpTileMap, goalUpAddress, 4);
                dmaCopyVram((u8 *)circleDownTileMap, goalDownAddress, 4);

            } else {
                dmaCopyVram((u8 *)crossUpTileMap, goalUpAddress, 4);
                dmaCopyVram((u8 *)crossDownTileMap, goalDownAddress, 4);
            }

            displayBlueCircle = 1;
        }
    }

    updateSupporters();
    updateBigFlag();
    updateLittleFlag();
    oamSetStriker();
    oamSetGoalkeeper();
    oamSetBall();

    switch(loadState) {
        case LoadStateStart:
            loadFrameCounter++;
            if (loadFrameCounter == 128) {
                loadState = LoadStateDo;
            }
            break;

        case LoadStateDo:
            clearLevel();

            countryTmptype = countryOfStrikertype;
            countryOfStrikertype = countryOfGoalkeepertype;
            countryOfGoalkeepertype = countryTmptype;

            if (isCpuStriker == 0) {
                remainingShoots1 -= 1;
                isCpuStriker = 1;
                scoreStates1[shootingCountry1Round] = scoreState;
                shootingCountry1Round++;
                if (scoreState == 1) {
                    score1++;
                }

            } else {
                remainingShoots2 -= 1;
                isCpuStriker = 0;
                goalAddressOffset += 2;
                scoreStates2[shootingCountry2Round] = scoreState;
                shootingCountry2Round++;
                if (scoreState == 1) {
                    score2++;
                }
            }

            switch (levelState) {
            case LevelStateNormal:
                if (shootingCountry2Round == 5) {
                    if (score1 != score2) {
                        initHome();
                        return;
                    }

                    levelState = LevelStateSpecial;
                    initLevelSession(country1type, country2type, 1, LevelStateSpecial);

                } else {
                    if (score1 > (score2 + remainingShoots2)) {
                        initHome();
                        return;
                    } else if (score2 > (score1 + remainingShoots1)) {
                        initHome();
                        return;
                    }
                    initLevel();
                }
                break;

            case LevelStateSpecial:
                if (shootingCountry2Round == 5) {
                    if (score1 != score2) {
                        initHome();
                        return;
                    }
                    levelState = LevelStateSpecial;
                    initLevelSession(country1type, country2type, 1, LevelStateSpecial);

                } else {
                    if (isCpuStriker == 0)  {
                        if (score1 != score2) {
                            initHome();
                            return;
                        }
                        ballsCounter++;
                    }
                    initLevel();
                }
                break;
            }
    }
}

/*!\brief Clear the Level screen.
*/
void clearLevel() {
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
