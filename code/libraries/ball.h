#include "definitions.h"

void SDL_RenderFillCircle(SDL_Renderer * renderer, int x, int y, int radius);

void ball_init (Ball * ball, Paddle paddle);

void ball_draw (SDL_Renderer * renderer, Ball ball);

bool ball_check_bounce_sides (Ball * ball);

bool ball_check_bounce_paddle (Ball * ball, Paddle paddle);

bool ball_check_bounce_block (Ball * ball, Block * block, int * score, int * speedup_counter);

void ball_check_respawn (Ball * ball, int * speedup_counter, Paddle paddle, int * lives, Audio audio, Wav life_lost);

void ball_move (Ball * ball);

void ball_move_check (int * score, int * lives, int * speedup_counter, Ball * ball, Paddle paddle, Block blocks[BLOCK_ROWS][BLOCK_COLS],
                      Audio audio, Wav bounce_paddle, Wav bounce_wall, Wav bounce_brick, Wav life_lost);

void speedupCheck (int * speedup_counter, Ball * ball);