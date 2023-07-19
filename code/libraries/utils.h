#include "definitions.h"

void resetHighscores (int highscores[NUMBER_OF_LEVELS]);

void writeHighscores (int highscores[NUMBER_OF_LEVELS]);

void readHighscores (int highscores[NUMBER_OF_LEVELS]);

void readSettings (Audio * audio);

void switchMuteStatus (Audio * audio);

void loadLevel (int level, Positions positions[NUMBER_OF_LEVELS], Block blocks[BLOCK_ROWS][BLOCK_COLS], Paddle * paddle, Ball * ball,
                SDL_Color rainbow[BLOCK_ROWS], int * lives, int * score,
                Game_State * state, int * speedup_counter);

SDL_Texture * loadTexture (SDL_Renderer * renderer, char * path);

bool checkMouseHover(SDL_Rect level_rect);

bool mouseHoverOverLevel();

bool mouseHoverOverSetting(Game_State state);

void selectSetting (Menu_Controls pause_controls, Game_State * state, Audio * audio,
                    int level, Positions positions[NUMBER_OF_LEVELS], Block blocks[BLOCK_ROWS][BLOCK_COLS], Paddle * paddle, Ball * ball,
                    SDL_Color rainbow[BLOCK_ROWS], int * lives, int * score, int * speedup_counter);

void checkMenuInputs (SDL_Event e, Menu_Controls * menu_controls,
                      Audio audio, Wav scroll);

void checkGameInputs (SDL_Event e, Game_Controls * game_controls);

void checkPauseInputs (SDL_Event e, Menu_Controls * pause_controls, Game_Controls * game_controls,
                       Audio audio, Wav scroll, Game_State state);