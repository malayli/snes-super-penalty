.include "hdr.asm"

.section ".rodata1" superfree

stage1Bg1Pic: .incbin "res/superpenalty/map/stage1BG1.pic" 
stage1Bg1Pic_end:

stage1Bg1Palette: .incbin "res/superpenalty/map/stage1BG1.pal"
stage1Bg1Palette_end:

.ends

.section ".rodata2" superfree

hudFontPic: .incbin "res/superpenalty/hud/hudFont.pic"
hudFontPic_end:

hudFontPalette: .incbin "res/superpenalty/hud/hudFont.pal"
hudFontPalette_end:

.ends

.section ".rodata3" superfree

; man
stagePeopleMan1Pic: .incbin "res/superpenalty/map/stagePeopleMan1.pic" 
stagePeopleMan1Pic_end:

stagePeopleMan2Pic: .incbin "res/superpenalty/map/stagePeopleMan2.pic" 
stagePeopleMan2Pic_end:

stagePeopleMan3Pic: .incbin "res/superpenalty/map/stagePeopleMan3.pic" 
stagePeopleMan3Pic_end:

; old man
stagePeopleOldMan1Pic: .incbin "res/superpenalty/map/stagePeopleOldMan1.pic" 
stagePeopleOldMan1Pic_end:

stagePeopleOldMan2Pic: .incbin "res/superpenalty/map/stagePeopleOldMan2.pic" 
stagePeopleOldMan2Pic_end:

stagePeopleOldMan3Pic: .incbin "res/superpenalty/map/stagePeopleOldMan3.pic" 
stagePeopleOldMan3Pic_end:

; big flag
stagePeopleBigFlag1Pic: .incbin "res/superpenalty/map/stagePeopleBigFlag1.pic" 
stagePeopleBigFlag1Pic_end:

stagePeopleBigFlag2Pic: .incbin "res/superpenalty/map/stagePeopleBigFlag2.pic" 
stagePeopleBigFlag2Pic_end:

stagePeopleBigFlag3Pic: .incbin "res/superpenalty/map/stagePeopleBigFlag3.pic" 
stagePeopleBigFlag3Pic_end:

; little flag
stagePeopleLittleFlagPic: .incbin "res/superpenalty/map/stagePeopleLittleFlag.pic" 
stagePeopleLittleFlagPic_end:

; OAM 1
oam1Pic: .incbin "res/superpenalty/sprites/oam1.pic" 
oam1Pic_end:

oam1Palette: .incbin "res/superpenalty/sprites/oam1.pal" 
oam1Palette_end:

; OAM 2
oam2Pic: .incbin "res/superpenalty/sprites/oam2.pic" 
oam2Pic_end:

; Numbers
number0Pic: .incbin "res/superpenalty/sprites/number0.pic" 
number0Pic_end:

number1Pic: .incbin "res/superpenalty/sprites/number1.pic" 
number1Pic_end:

number2Pic: .incbin "res/superpenalty/sprites/number2.pic" 
number2Pic_end:

number3Pic: .incbin "res/superpenalty/sprites/number3.pic" 
number3Pic_end:

number4Pic: .incbin "res/superpenalty/sprites/number4.pic" 
number4Pic_end:

number5Pic: .incbin "res/superpenalty/sprites/number5.pic" 
number5Pic_end:

number6Pic: .incbin "res/superpenalty/sprites/number6.pic" 
number6Pic_end:

number7Pic: .incbin "res/superpenalty/sprites/number7.pic" 
number7Pic_end:

number8Pic: .incbin "res/superpenalty/sprites/number8.pic" 
number8Pic_end:

number9Pic: .incbin "res/superpenalty/sprites/number9.pic" 
number9Pic_end:

.ends

.section ".rodata4" superfree

; Striker
strikerPic: .incbin "res/superpenalty/sprites/striker.pic" 
strikerPic_end:

.ends

.section ".rodata5" superfree

; Goal keeper
goalkeeperPic: .incbin "res/superpenalty/sprites/goalkeeper.pic" 
goalkeeperPic_end:

.ends

.section ".rodata6" superfree

; Flags
flagbrazilPic: .incbin "res/superpenalty/sprites/flagbrazil.pic" 
flagbrazilPic_end:

flagbrazilPalette: .incbin "res/superpenalty/sprites/flagbrazil.pal" 
flagbrazilPalette_end:

flagenglandPic: .incbin "res/superpenalty/sprites/flagengland.pic" 
flagenglandPic_end:

flagenglandPalette: .incbin "res/superpenalty/sprites/flagengland.pal" 
flagenglandPalette_end:

flagfrancePic: .incbin "res/superpenalty/sprites/flagfrance.pic" 
flagfrancePic_end:

flagfrancePalette: .incbin "res/superpenalty/sprites/flagfrance.pal" 
flagfrancePalette_end:

flagcolombiaPic: .incbin "res/superpenalty/sprites/flagcolombia.pic" 
flagcolombiaPic_end:

flagcolombiaPalette: .incbin "res/superpenalty/sprites/flagcolombia.pal" 
flagcolombiaPalette_end:

; Home

homePic: .incbin "res/superpenalty/home/home.pic" 
homePic_end:

homeMap: .incbin "res/superpenalty/home/home.map" 
homeMap_end:

homePalette: .incbin "res/superpenalty/home/home.pal" 
homePalette_end:

.ends
