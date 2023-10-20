typedef enum GameState {
    GameStateHome = 0,
    GameStateLevel,
    GameStateTestAnim,
    GameStateSelectLevel,
    GameStateOptions,
    GameStateSandbox,
    GameStateLevelPause,
    GameStateLoadingLevel,
    GameStateLoadingOptions,
    GameStateTestAnimPause,
    GameStateLoadingSelectYourTurtle,
    GameStateSelectPlayer,
    GameStateLevelIntro, 
    GameStateLogo} GameState;
extern enum GameState gameState;

extern void (*inputs[14])();
extern void (*updates[14])();

void emptyFunction();
