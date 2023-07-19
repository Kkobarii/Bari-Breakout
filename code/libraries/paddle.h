#include "definitions.h"

void paddle_init (Paddle * paddle);

void paddle_move_check (Paddle * paddle, Ball ball, Game_Controls controls);

void paddle_move (Paddle * paddle, int direction);

void paddle_move_cheat (Ball ball, Paddle * paddle);

void paddle_draw (SDL_Renderer * renderer, Paddle paddle);