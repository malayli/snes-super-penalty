.include "hdr.asm"

.ramsection "ram.data" bank $7E

refreshBg1 DSB 1
refreshBg3 DSB 1
refreshScore DSB 1
refreshScore2 DSB 1

oamAddressOffset DSW 1
oamIdOffset DSW 1

bg1TileMap: DSW $800
bg3TileMap: DSW $800

.ends
