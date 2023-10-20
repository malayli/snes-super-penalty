typedef enum CountryType {
    CountryTypeBrazil = 0,
    CountryTypeEngland,
    CountryTypeFrance,
    CountryTypeColombia
} CountryType;

extern char hudFontPic, hudFontPic_end;
extern char hudFontMap, hudFontMap_end;
extern char hudFontPalette;
extern u16 titleX;
extern u16 countryNameOffsetX;

void initHud(u16 tilesetAddress);
void printName(char *name, u16 x);
void printRole(u8 role, u16 x);
void oamSetString(char *name);
