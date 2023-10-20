extern u8 spriteObjIndex;
extern u16 oamId;
extern u8 prio;
extern u16 *selectedSprites;
extern s16 offsetX;
extern s16 offsetY;
extern u8 spritesCount;

void oamSetSprite();
void oamSetSpriteInvertX();
void oamSetMetaSpriteXYEx();
void oamSetMetaSpriteXYInvertXEx();
void oamSetSpriteXYEx();
void oamSetSpriteXYInvertXEx();
void oamSetMetaSprite();
void oamSetMetaSpriteEx();
