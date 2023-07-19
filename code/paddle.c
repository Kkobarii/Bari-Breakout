#include "libraries/libraries.h"

void paddle_init (Paddle * paddle)
{
    paddle->x1 = GAME_WIDTH/2 - PADDLE_WIDTH/2;
    paddle->y1 = 440;
    paddle->x2 = GAME_WIDTH/2 + PADDLE_WIDTH/2;
    paddle->y2 = 448;
}

void paddle_move_check (Paddle * paddle, Ball ball, Game_Controls controls)
{
    if (controls.right && paddle->x2 < GAME_WIDTH)
    {
        paddle_move(paddle, 1);
    }
    if (controls.left && paddle->x1 > 0)
    {
        paddle_move(paddle, -1);
    }
    if (controls.paddle_cheat && paddle->x2 <= GAME_WIDTH - 1 && paddle->x1 >= 0)
    {
        if (ball.x < (GAME_WIDTH - 1) - PADDLE_WIDTH/2 && ball.x > PADDLE_WIDTH/2)
        {
            paddle_move_cheat(ball, paddle);
        }
        else if (ball.x == (GAME_WIDTH - 1) - PADDLE_WIDTH/2 && ball.movement_horizontal == -1)
        {
            paddle_move_cheat(ball, paddle);
        }
        else if (ball.x == PADDLE_WIDTH/2 && ball.movement_horizontal == 1)
        {
            paddle_move_cheat(ball, paddle);
        }
    }
}

void paddle_move (Paddle * paddle, int direction)
{    
    paddle->x1 += direction*2;
    paddle->x2 += direction*2;
}

void paddle_move_cheat (Ball ball, Paddle * paddle)
{
    paddle->x1 = ball.x - PADDLE_WIDTH/2;
    paddle->x2 = paddle->x1 + PADDLE_WIDTH;
}

void paddle_draw (SDL_Renderer * renderer, Paddle paddle)
{
    SDL_Rect paddleForRender = {
        paddle.x1 + WIDTH_OFFSET,
        paddle.y1 + HEIGHT_OFFSET,
        PADDLE_WIDTH,
        PADDLE_HEIGHT
    };

    SDL_SetRenderDrawColor (renderer, 255, 0, 0, 255);

    SDL_RenderFillRect (renderer, &paddleForRender);
}