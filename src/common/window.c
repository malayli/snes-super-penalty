/*---------------------------------------------------------------------------------


    Transparent window effect in mode 1 
    -- digifox


---------------------------------------------------------------------------------*/
#include <snes.h>
#include "window.h"
#include "dmaex.h"

#define REG_HDMA0 (*(vuint8 *)0x4300)
#define REG_HDMA1 (*(vuint8 *)0x4310)
#define REG_HDMA2 (*(vuint8 *)0x4320)
#define REG_HDMA3 (*(vuint8 *)0x4330)
#define REG_HDMA4 (*(vuint8 *)0x4340)
#define REG_HDMA5 (*(vuint8 *)0x4350)
#define REG_HDMA6 (*(vuint8 *)0x4360)
#define REG_HDMA7 (*(vuint8 *)0x4370)

u8 selectedBackground;
u8 selectedWindow1;
u8 selectedWindow2;

/*!\brief Create HDMA transparent overlays.
    \param hdmaTableLeft the HDMA table left.
    \param hdmaTableRight the HDMA table right.
    \param hdmaTableLeft2 the second HDMA table left.
    \param hdmaTableRight2 the second HDMA table right.
    \param bgNumber the background number where the transparent overlays are displayed.
    \param color the overlays color (BGR format, see REG_COLDATA).
    \param colorIntensity the overlays color intensity (max value is 0b00011111).
    \param colorMath the color math to use (Add/Subtract, Half, Backdrop - see REG_CGADSUB)
    \warning Only works on BG1 and BG2.
    \note This function must be improved to work on BG3 and BG4.
*/
void createOverlays(u8 *hdmaTableLeft, u8 *hdmaTableRight, 
    u8 *hdmaTableLeft2, u8 *hdmaTableRight2, 
    u8 bgNumber, u8 color, u8 colorIntensity, u8 colorMath) {
    // Enable Color Math and Activate MathWindow (on REG_CGWSEL)
    // Activate the passed colorMath when Main Screen = bgNumber (on REG_CGADSUB)
    setColorEffect(0b00010000, colorMath | (bgNumber + 1));

    // Set color with an intensity
    REG_COLDATA = color | colorIntensity;

    switch(bgNumber) {
        case 0:
            selectedBackground = MSWIN_BG1; 
            selectedWindow1 = MSWIN1_BG1MSKENABLE;
            selectedWindow2 = MSWIN2_BG1MSKENABLE;
            break;

        case 1:
            selectedBackground = MSWIN_BG2;
            selectedWindow1 = MSWIN1_BG2MSKENABLE;
            selectedWindow2 = MSWIN1_BG2MSKENABLE;
            break;
    }
    
    // Create an HDMA Window using HDMA Channels 4 and 5
    // Activate Window1 on bgNumber (see: REG_W12SEL)
    setModeHdmaWindow(selectedBackground, 
        selectedWindow1, 
        hdmaTableLeft, 
        hdmaTableRight);

    // Create an HDMA Window using HDMA Channels 6 and 7
    // Activate Window2 on bgNumber (see: REG_W12SEL)
    setModeHdmaWindow67(selectedBackground, 
        selectedWindow2, 
        hdmaTableLeft2, 
        hdmaTableRight2);

    setModeHdmaReset(0b11110000);

    // Disable main screen windows
    REG_TMW = 0;

    // Activate Color Math on Window1 and Window2 Area
    REG_WOBJSEL = 0b10100000;

    // Reset HDMA channels 4, 5, 6, 7
    REG_HDMA4 = 0;
    REG_HDMA5 = 0;
    REG_HDMA6 = 0;
    REG_HDMA7 = 0;

    REG_W12SEL = 0;

    REG_WH0 = 1;
    REG_WH1 = 0;
    REG_WH2 = 1;
    REG_WH3 = 0;
}
