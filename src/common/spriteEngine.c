#include <snes.h>

#include "spriteEngine.h"
#include "vblank.h"
#include "utils.h"

// ROM

const u16 oamAddressOrigin = 0x6000;

// RAM

u8 selectedOamSize;

/*!\brief Initialize the sprite engine.
    \param oamSize the OAM Size.
*/
void initSpriteEngine(u8 oamSize) {
    oamInit();
    selectedOamSize = oamSize;
    oamAddressOffset = 0;
    oamIdOffset = 0;
    oamInitGfxAttr(oamAddressOrigin, selectedOamSize);
}
