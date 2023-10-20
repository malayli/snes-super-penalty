// Palette numbers for tileset
#define PAL0 0
#define PAL1 1
#define PAL2 2
#define PAL3 3
#define PAL4 4
#define PAL5 5
#define PAL6 6
#define PAL7 7
#define PAL8 8

// Palette numbers for tilemap
#define TPAL0 PAL0<<10
#define TPAL1 PAL1<<10
#define TPAL2 PAL2<<10
#define TPAL3 PAL3<<10
#define TPAL4 PAL4<<10
#define TPAL5 PAL5<<10
#define TPAL6 PAL6<<10
#define TPAL7 PAL7<<10
#define TPAL8 PAL8<<10

// Flip a tile horizontally
#define THFLIP (1 << 14)

// Flip a tile vertically
#define TVFLIP (1 << 15)

// Background numbers
#define BG0 0
#define BG1 1
#define BG2 2
#define BG3 3
#define BG4 4 // used for OAM

extern u8 refreshBg1;
extern u8 refreshBg3, refreshScore, refreshScore2;
extern u16 *bg1TileMap;
extern u16 bg3TileMap[1024];

extern u16 oamAddressOffset;
extern u16 oamIdOffset;
