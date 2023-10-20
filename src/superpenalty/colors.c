#include <snes.h>
#include "striker.h"
#include "goalkeeper.h"
#include "../common/utils.h"

const u16 strikerBrazilPalette[] = {
    RGB8(0, 165, 0),
    RGB8(132, 99, 0),
    RGB8(132, 66, 24),
    RGB8(189, 90, 66),
    RGB8(206, 140, 115),
    RGB8(214, 189, 173),
    RGB8(231, 231, 222),
    RGB8(140, 140, 0),
    RGB8(181, 181, 0),
    RGB8(255, 255, 0),
    RGB8(0, 0, 173),
    RGB8(82, 82, 222),
    RGB8(132, 132, 255),
    RGB8(148, 148, 148),
    RGB8(222, 222, 222),
    RGB8(0, 74, 0)
};

const u16 strikerColombiaPalette[] = {
    RGB8(0, 165, 0),
    RGB8(49, 33, 0),
    RGB8(66, 24, 8), 
    RGB8(90, 41, 16), 
    RGB8(148, 82, 41), 
    RGB8(181, 115, 74),
    RGB8(214, 189, 165),
    RGB8(140, 140, 0),
    RGB8(181, 181, 0),
    RGB8(255, 255, 0),
    RGB8(0, 0, 173),
    RGB8(82, 82, 222),
    RGB8(132, 132, 255),
    RGB8(148, 49, 0),
    RGB8(239, 66, 0),
    RGB8(0, 74, 0)
};

const u16 strikerEnglandPalette[] = {
    RGB8(0, 165, 0),
    RGB8(222, 181, 0),
    RGB8(107, 41, 24), 
    RGB8(156, 82, 49), 
    RGB8(189, 140, 99), 
    RGB8(206, 173, 140),
    RGB8(231, 214, 198),
    RGB8(132, 132, 132),
    RGB8(189, 189, 189),
    RGB8(255, 255, 255),
    RGB8(49, 49, 49),
    RGB8(74, 74, 74),
    RGB8(132, 132, 132),
    RGB8(148, 148, 148),
    RGB8(222, 222, 222),
    RGB8(0, 74, 0)
};

const u16 strikerFrancePalette[] = {
    RGB8(0, 165, 0),
    RGB8(132, 99, 0),
    RGB8(107, 41, 24), 
    RGB8(189, 82, 57), 
    RGB8(206, 140, 115), 
    RGB8(214, 189, 173),
    RGB8(231, 231, 222),
    RGB8(0, 0, 222),
    RGB8(49, 49, 222),
    RGB8(107, 107, 231),
    RGB8(156, 156, 156),
    RGB8(206, 206, 206),
    RGB8(255, 255, 255),
    RGB8(140, 0, 0),
    RGB8(222, 0, 0),
    RGB8(0, 74, 0)
};