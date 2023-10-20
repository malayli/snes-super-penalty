extern const u16 bgTextSize;

extern u16 bg3TextAddress;

void initBg3FontMapEx(char *fontTileSource,
    char *fontTileSource_end,
    char *fontTilePalette,
    u8 paletteNumber,
    u16 mapAddress,
    u16 tileAddress);

void clearBgText(u16 *tileMap);

void bg3PrintInt(u16 x, u16 y, u16 number, u8 paletteNumber);

void bg3PrintIntEx(u16 x, u16 y, u16 number, u8 paletteNumber, u8 hflip);
