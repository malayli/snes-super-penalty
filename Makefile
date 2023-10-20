ifeq ($(strip $(PVSNESLIB_HOME)),)
$(error "Please create an environment variable PVSNESLIB_HOME with path to its folder and restart application. (you can do it on windows with <setx PVSNESLIB_HOME "/c/snesdev">)")
endif

# BEFORE including snes_rules :
# list in AUDIOFILES all your .it files in the right order. It will build to generate soundbank file
AUDIODIR :=	res/superpenalty/musics
export AUDIOFILES :=	$(foreach dir, $(AUDIODIR), \
	$(dir)/*.it)

# then define the path to generate soundbank data. The name can be different but do not forget to update your include in .c file !
export SOUNDBANK := src/soundbank

# include ./snes_rules
include ${PVSNESLIB_HOME}/devkitsnes/snes_rules

.PHONY: all

#---------------------------------------------------------------------------------
# ROMNAME is used in snes_rules file
export ROMNAME := superpk

TARGET := SUPERPK
GTITLE := -ht"$(TARGET)"

# to build musics, define SMCONVFLAGS with parameters you want
SMCONVFLAGS	:= -s -o $(SOUNDBANK) -V -b 5
musics: $(SOUNDBANK).obj

all: musics stages numbers countriesflag man oldman bigflag littleflag $(ROMNAME).sfc
	$(SNTOOLS) -hi! $(GTITLE) $(ROMNAME).sfc
	cp $(ROMNAME).sfc $(ROMNAME)_ntsc.sfc
	$(SNTOOLS) -hi! -hS! $(GTITLE) -hc03 $(ROMNAME).sfc
	mv $(ROMNAME).sfc $(ROMNAME)_pal.sfc

cleanRom:
	@echo clean rom
	@rm -f $(ROMNAME).sfc $(ROMNAME).sym $(ROMNAME)_ntsc.sfc $(ROMNAME)_pal.sfc

cleanGfxSuperPenalty:
	@echo clean SuperPenalty graphics data
	@rm -f res/superpenalty/home/*.pic res/superpenalty/home/*.map res/superpenalty/home/*.pal
	@rm -f res/superpenalty/map/*.pic res/superpenalty/map/*.map res/superpenalty/map/*.pal
	@rm -f res/superpenalty/hud/*.pic res/superpenalty/hud/*.map res/superpenalty/hud/*.pal
	@rm -f res/superpenalty/sprites/*.pic res/superpenalty/sprites/*.map res/superpenalty/sprites/*.pal

clean: cleanBuildRes cleanRom cleanGfx cleanGfxSuperPenalty cleanAudio

# Home
home.pic: res/superpenalty/home/home.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -pr -pc16 -n -gs8 -pe1 -fbmp -m -m32p $<

# Stage
stage1BG1.pic: res/superpenalty/map/stage1BG1.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -pr -pc16 -n -gs8 -pe0 -fbmp $<

# Hud
hudFont.pic: res/superpenalty/hud/hudFont.bmp
	@echo convert font with no tile reduction ... $(notdir $@)
	$(GFXCONV) -pc4 -n -gs8 -pe0 -fbmp  $<

# OAM Sprites
oam1.pic: res/superpenalty/sprites/oam1.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -pr -pc16 -n -gs8 -pe0 -fbmp $<

oam2.pic: res/superpenalty/sprites/oam2.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -pr -pc16 -n -gs8 -pe0 -fbmp $<

# Number Sprites

number5.pic: res/superpenalty/sprites/number5.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -pr -pc16 -n -gs8 -pe0 -fbmp $<

number6.pic: res/superpenalty/sprites/number6.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -pr -pc16 -n -gs8 -pe0 -fbmp $<

number7.pic: res/superpenalty/sprites/number7.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -pr -pc16 -n -gs8 -pe0 -fbmp $<

number8.pic: res/superpenalty/sprites/number8.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -pr -pc16 -n -gs8 -pe0 -fbmp $<

number9.pic: res/superpenalty/sprites/number9.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -pr -pc16 -n -gs8 -pe0 -fbmp $<

# Man
stagePeopleMan1.pic: res/superpenalty/map/stagePeopleMan1.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -pr -pc16 -n -gs8 -pe0 -fbmp $<

stagePeopleMan2.pic: res/superpenalty/map/stagePeopleMan2.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -pr -pc16 -n -gs8 -pe0 -fbmp $<

stagePeopleMan3.pic: res/superpenalty/map/stagePeopleMan3.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -pr -pc16 -n -gs8 -pe0 -fbmp $<

# Old man
stagePeopleOldMan1.pic: res/superpenalty/map/stagePeopleOldMan1.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -pr -pc16 -n -gs8 -pe0 -fbmp $<

stagePeopleOldMan2.pic: res/superpenalty/map/stagePeopleOldMan2.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -pr -pc16 -n -gs8 -pe0 -fbmp $<

stagePeopleOldMan3.pic: res/superpenalty/map/stagePeopleOldMan3.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -pr -pc16 -n -gs8 -pe0 -fbmp $<

# Big flag
stagePeopleBigFlag1.pic: res/superpenalty/map/stagePeopleBigFlag1.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -pr -pc16 -n -gs8 -pe0 -fbmp $<

stagePeopleBigFlag2.pic: res/superpenalty/map/stagePeopleBigFlag2.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -pr -pc16 -n -gs8 -pe0 -fbmp $<

stagePeopleBigFlag3.pic: res/superpenalty/map/stagePeopleBigFlag3.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -pr -pc16 -n -gs8 -pe0 -fbmp $<

# Little flag
stagePeopleLittleFlag.pic: res/superpenalty/map/stagePeopleLittleFlag.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -pr -pc16 -n -gs8 -pe0 -fbmp $<

# Striker
striker.pic: res/superpenalty/sprites/striker.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -pr -pc16 -n -gs8 -pe0 -fbmp $<

# Goal keeper
goalkeeper.pic: res/superpenalty/sprites/goalkeeper.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -pr -pc16 -n -gs8 -pe0 -fbmp $<

# Flags
flagbrazil.pic: res/superpenalty/sprites/flagbrazil.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -pc16 -n -gs8 -pe4 -fbmp $<

flagengland.pic: res/superpenalty/sprites/flagengland.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -pc16 -n -gs8 -pe5 -fbmp $<

flagfrance.pic: res/superpenalty/sprites/flagfrance.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -pc16 -n -gs8 -pe5 -fbmp $<

flagcolombia.pic: res/superpenalty/sprites/flagcolombia.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -pc16 -n -gs8 -pe5 -fbmp $<

stages: hudFont.pic home.pic stage1BG1.pic oam1.pic oam2.pic striker.pic goalkeeper.pic
numbers: number5.pic number6.pic number7.pic number8.pic number9.pic
countriesflag: flagbrazil.pic flagengland.pic flagfrance.pic flagcolombia.pic
man: stagePeopleMan1.pic stagePeopleMan2.pic stagePeopleMan3.pic
oldman: stagePeopleOldMan1.pic stagePeopleOldMan2.pic stagePeopleOldMan3.pic
bigflag: stagePeopleBigFlag1.pic stagePeopleBigFlag2.pic stagePeopleBigFlag3.pic
littleflag: stagePeopleLittleFlag.pic
