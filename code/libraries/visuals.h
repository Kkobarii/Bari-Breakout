#include "definitions.h"

void initWindowBackground (SDL_Rect * menu_background, SDL_Rect * game_background);

void drawWindowBackground (SDL_Renderer * renderer, SDL_Rect menu_background, SDL_Rect game_background);

void drawText (SDL_Renderer * renderer, TTF_Font * font, SDL_Color color, SDL_Rect rect, char * text);

void drawScore (SDL_Renderer * renderer, int score, TTF_Font * font);

void drawLevel (SDL_Renderer * renderer, int level, TTF_Font * font);

void drawHearts (SDL_Renderer * renderer, SDL_Texture * heart1, SDL_Texture * heart2);

void drawLives (SDL_Renderer * renderer, int lives, 
                SDL_Texture * heart_full, SDL_Texture * heart_half, SDL_Texture * heart_empty);

void drawGameWindow (SDL_Renderer * renderer, Paddle paddle, Ball ball,
                     Block blocks[BLOCK_ROWS][BLOCK_COLS], 
                     int level, int score, int lives, TTF_Font * font,
                     SDL_Texture * heart_full, SDL_Texture * heart_half, SDL_Texture * heart_empty, 
                     SDL_Rect menu_background, SDL_Rect game_background);

void drawPauseScreen (SDL_Renderer * renderer, SDL_Texture * overlay, Game_State state,
                      TTF_Font * big_font, TTF_Font * medium_font, Menu_Controls pause_controls, Audio audio);

void drawEndScreen (bool win, SDL_Renderer * renderer, SDL_Texture * overlay, 
                    TTF_Font * big_font, TTF_Font * medium_font, 
                    int score, int highscores[NUMBER_OF_LEVELS], Game_State * state, int level,
                    SDL_Color rainbow[BLOCK_ROWS], int rainbow_offset, 
                    Menu_Controls pause_controls, Audio audio);

void drawMenuWindow (SDL_Renderer * renderer, SDL_Rect menu_background, SDL_Rect game_background,
                     TTF_Font * big_font, TTF_Font * medium_font, TTF_Font * small_font, 
                     SDL_Color rainbow[BLOCK_ROWS], int rainbow_offset,
                     int highscores[NUMBER_OF_LEVELS], Menu_Controls * menu_controls);

void drawPauseMenuButtons (SDL_Renderer * renderer, Menu_Controls pause_controls, 
                           Audio audio, TTF_Font * medium_font, Game_State state);

void drawRainbowText (char text[DEFAULT_TEXT_LENGTH], int x, int y, SDL_Renderer * renderer, 
                      TTF_Font * font, SDL_Color rainbow[BLOCK_ROWS], int rainbow_offset);