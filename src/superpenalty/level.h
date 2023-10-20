typedef enum LevelState {
    LevelStateNormal = 0, 
    LevelStateSpecial
} LevelState;

void initLevelSession(CountryType country1, CountryType country2, u8 ballsCount, LevelState state);
void handleLevelInput();
void updateLevel();
void clearLevel();
