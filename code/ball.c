#include "libraries/libraries.h"

void SDL_RenderFillCircle(SDL_Renderer * renderer, int x, int y, int radius)
{
    int offsetx = 0;
    int offsety = radius;
    int d = radius -1;

    while (offsety >= offsetx) 
    {
        SDL_RenderDrawLine(renderer, x - offsety, y + offsetx,
                           x + offsety, y + offsetx);
        SDL_RenderDrawLine(renderer, x - offsetx, y + offsety,
                           x + offsetx, y + offsety);
        SDL_RenderDrawLine(renderer, x - offsetx, y - offsety,
                           x + offsetx, y - offsety);
        SDL_RenderDrawLine(renderer, x - offsety, y - offsetx,
                           x + offsety, y - offsetx);

        if (d >= 2 * offsetx) 
        {
            d -= 2 * offsetx + 1;
            offsetx += 1;
        }
        else if (d < 2 * (radius - offsety)) 
        {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else 
        {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }
}

void ball_init (Ball * ball, Paddle paddle)
{
    srand(time(NULL));
    int random = rand() % 2 + 1; // 1 - 2

    ball->movement_horizontal = (random == 1 ? 1 : -1);
    ball->movement_vertical = -1;
    ball->x = (paddle.x1 + paddle.x2) / 2;
    ball->y = paddle.y1 - 11;
    ball->radius = 8;
}

void ball_draw (SDL_Renderer * renderer, Ball ball)
{
    SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
    SDL_RenderFillCircle
        (renderer, 
        WIDTH_OFFSET + ball.x, 
        HEIGHT_OFFSET + ball.y, 
        ball.radius + 1);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillCircle
        (renderer, 
        WIDTH_OFFSET + ball.x, 
        HEIGHT_OFFSET + ball.y, 
        ball.radius);
}

bool ball_check_bounce_sides (Ball * ball)
{
    float distanceXright = abs(ball->x - GAME_WIDTH);
    float distanceXleft = abs(ball->x - 0);
    float distanceY = abs(ball->y - 0);

    if (distanceXright <= ball->radius)
    {
        ball->movement_horizontal *= -1;
        return true;
    }
    if (distanceXleft <= ball->radius)
    {
        ball->movement_horizontal *= -1;
        return true;
    }
    if (distanceY <= ball->radius)
    {
        ball->movement_vertical *= -1;
        return true;
    }
    return false;
}

bool ball_check_bounce_paddle (Ball * ball, Paddle paddle)
{
    float distanceY = abs(ball->y - paddle.y1);

    if (distanceY <= ball->radius && 
        ball->x >= paddle.x1 && ball->x <= paddle.x2)
    {
        ball->movement_vertical = -1;
        return true;
    }

    return false;
}

bool ball_block_check (Ball ball, Block block)
{
    float distanceX = abs(ball.x - (block.x1 + block.x2)/2);
    float distanceY = abs(ball.y - ((block.y1 + block.y2)/2 + BLOCK_ROWS_OFFSET));

    if (distanceX > (BLOCK_WIDTH/2 + ball.radius))
        return false;
    if (distanceY > (BLOCK_HEIGHT/2 + ball.radius))
        return false;

    if (distanceX <= (BLOCK_WIDTH/2 + ball.radius))
        return true;
    if (distanceY <= (BLOCK_HEIGHT/2 + ball.radius))
        return true;
    
    float corner_distance_sq = 
        (distanceX - BLOCK_WIDTH/2)*(distanceX - BLOCK_WIDTH/2) +
        (distanceY - BLOCK_HEIGHT/2)*(distanceY - BLOCK_HEIGHT/2);

    return (corner_distance_sq <= (ball.radius * ball.radius));      
}

bool ball_check_bounce_block (Ball * ball, Block * block, int * score, int * speedup_counter)
{
    if (block->alive)
    {
        if (ball_block_check(*ball, *block))
        {
            // Odraz od horni strany
            if (ball->x >= block->x1
                && ball->x <= block->x2
                && ball->movement_vertical == 1)
            {
                ball->movement_vertical *= -1;
                block->alive = false;
                (*speedup_counter)++;
                *score += block->points;
                return true;
            }
            // Odraz od dolni strany
            if (ball->x >= block->x1
                && ball->x <= block->x2
                && ball->movement_vertical == -1)
            {
                ball->movement_vertical *= -1;
                block->alive = false;
                (*speedup_counter)++;
                *score += block->points;
                return true;
            }
            // Odraz od prave strany
            if (ball->y >= block->y1 + BLOCK_ROWS_OFFSET
                && ball->y <= block->y2 + BLOCK_ROWS_OFFSET
                && ball->movement_horizontal == -1)
            {
                ball->movement_horizontal *= -1;
                block->alive = false;
                (*speedup_counter)++;
                *score += block->points;
                return true;
            }
            // Odraz od leve strany
            if (ball->y >= block->y1 + BLOCK_ROWS_OFFSET
                && ball->y <= block->y2 + BLOCK_ROWS_OFFSET
                && ball->movement_horizontal == 1)
            {
                ball->movement_horizontal *= -1;
                block->alive = false;
                (*speedup_counter)++;
                *score += block->points;
                return true;
            }
        }
    }
    return false;
}

void ball_check_respawn (Ball * ball, int * speedup_counter, Paddle paddle, int * lives, Audio audio, Wav life_lost)
{
    if (ball->y > WINDOW_HEIGHT)
    {
        ball_init(ball, paddle);
        (*lives)--;
        *speedup_counter = 0;

        if (ball->speed > 1)
            ball->speed--;

        if (audio.play)
        {
            SDL_ClearQueuedAudio(audio.device);
            SDL_QueueAudio(audio.device, life_lost.wav_buffer, life_lost.wav_length);
        }
    }
}

void ball_move (Ball * ball)
{
    ball->x += (int)(ball->speed * (float)ball->movement_horizontal);
    ball->y += (int)(ball->speed * (float)ball->movement_vertical);
}

void ball_move_check (int * score, int * lives, int * speedup_counter, 
                      Ball * ball, Paddle paddle, Block blocks[BLOCK_ROWS][BLOCK_COLS],
                      Audio audio, Wav bounce_paddle, Wav bounce_wall, Wav bounce_brick, Wav life_lost)
{
    ball_check_respawn(ball, speedup_counter, paddle, lives, audio, life_lost);

    if (ball_check_bounce_sides(ball))
    {
        if (audio.play)
        {
            SDL_ClearQueuedAudio(audio.device);
            SDL_QueueAudio(audio.device, bounce_wall.wav_buffer, bounce_wall.wav_length);
        }
    }
    if (ball_check_bounce_paddle(ball, paddle))
    {
        if (audio.play)
        {
            SDL_ClearQueuedAudio(audio.device);
            SDL_QueueAudio(audio.device, bounce_paddle.wav_buffer, bounce_paddle.wav_length);
        }
    }

    for (int rows = 0; rows < BLOCK_ROWS; rows++)
    {
        for (int cols = 0; cols < BLOCK_COLS; cols++)
        {
            bool bounced = ball_check_bounce_block(ball, &(blocks[rows][cols]), score, speedup_counter);
            if (bounced && audio.play)
            {
                SDL_ClearQueuedAudio(audio.device);
                SDL_QueueAudio(audio.device, bounce_brick.wav_buffer, bounce_brick.wav_length);
            }
        }
    }
    
    ball_move(ball);
}