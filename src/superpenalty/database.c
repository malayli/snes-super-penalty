#include <snes.h>
#include "database.h"

// ROM

extern char number5Pic, number5Pic_end;
extern char number6Pic, number6Pic_end;
extern char number7Pic, number7Pic_end;
extern char number8Pic, number8Pic_end;
extern char number9Pic, number9Pic_end;

const char *playersName[] = {
    "Ederson", "Neymar", "Romario", "Rivaldo", "Fabinho", "R.Carlos",
    "R.Banks", "T.Keegan", "A.Regis", "J.Platt", "F.Morley", "Owen",
    "Barthez", "Platini", "Guivarch", "Pires", "Blanc", "Guerin",
    "Costa", "Valderama", "Rodriguez", "James", "Cruz", "Pablo",
    0xFFFF
};

const u8 playersNumber[] = {
    1, 0, 9, 3, 8, 2, 7, 2, 6, 2, 5, 1, // Brazil

    1, 0, 9, 3, 8, 3, 7, 2, 6, 2, 5, 1, // England

    1, 0, 8, 3, 9, 3, 7, 2, 5, 1, 6, 2, // France

    1, 0, 6, 0, 9, 0, 9, 0, 9, 0, 9, 0, // Colombia
    0xFF
};

const u8 *numberSource[] = {
    &number6Pic, &number6Pic, &number6Pic, &number6Pic, &number6Pic, 
    &number5Pic, &number6Pic, &number7Pic, &number8Pic, &number9Pic
};

const char *countriesName[] = {
    "BRAZIL",
    "ENGLAND",
    "FRANCE",
    "COLOMBIA"
};
