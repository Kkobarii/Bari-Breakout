#include "definitions.h"

void scan_positions (Positions positions[NUMBER_OF_LEVELS]);

void rainbow_init (SDL_Color rainbow[BLOCK_ROWS]);

void blocks_init (Block blocks[BLOCK_ROWS][BLOCK_COLS], SDL_Color rainbow[BLOCK_ROWS], char status[BLOCK_ROWS][BLOCK_COLS]);

void block_draw (SDL_Renderer * renderer, Block blockForRender);

void block_drawLines();

bool test_win_state (Block blocks[BLOCK_ROWS][BLOCK_COLS]);