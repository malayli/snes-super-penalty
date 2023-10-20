#include <snes.h>
#include "gameState.h"

GameState gameState = GameStateHome;

void (*inputs[14])();
void (*updates[14])();

void emptyFunction() {
}
