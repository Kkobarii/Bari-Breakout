#include "libraries/libraries.h"

void scan_positions (Positions positions[NUMBER_OF_LEVELS])
{
    FILE * file = fopen("assets/block_positions.txt", "r");

    if (file == NULL)
    {
        printf("ERROR: couldn't load block positions\n");
        exit(1);
    }
    
    for (int lvls = 0; lvls < NUMBER_OF_LEVELS; lvls++)
    {
        (positions[lvls]).level = lvls;

        for (int rows = 0; rows < BLOCK_ROWS; rows++)
        {
            for (int cols = 0; cols < BLOCK_COLS; cols++)
            {
                fscanf(file, " %c", &(positions[lvls]).status[rows][cols]);
            }
        }
    }

    fclose(file);
}

void rainbow_init (SDL_Color rainbow[BLOCK_ROWS])
{
    (rainbow[0]).r = 255;
    (rainbow[0]).g = 0;
    (rainbow[0]).b = 0;
    (rainbow[0]).a = 255;

    (rainbow[1]).r = 255;
    (rainbow[1]).g = 127;
    (rainbow[1]).b = 0;
    (rainbow[1]).a = 255;

    (rainbow[2]).r = 240;
    (rainbow[2]).g = 240;
    (rainbow[2]).b = 0;
    (rainbow[2]).a = 255;

    (rainbow[3]).r = 0;
    (rainbow[3]).g = 255;
    (rainbow[3]).b = 0;
    (rainbow[3]).a = 255;

    (rainbow[4]).r = 0;
    (rainbow[4]).g = 200;
    (rainbow[4]).b = 255;
    (rainbow[4]).a = 255;

    (rainbow[5]).r = 0;
    (rainbow[5]).g = 127;
    (rainbow[5]).b = 255;
    (rainbow[5]).a = 255;

    (rainbow[6]).r = 200;
    (rainbow[6]).g = 0;
    (rainbow[6]).b = 255;
    (rainbow[6]).a = 255;

    (rainbow[7]).r = 255;
    (rainbow[7]).g = 0;
    (rainbow[7]).b = 150;
    (rainbow[7]).a = 255;
}

void blocks_init (Block blocks[BLOCK_ROWS][BLOCK_COLS], SDL_Color rainbow[BLOCK_ROWS], char status[BLOCK_ROWS][BLOCK_COLS])
{
    for (int rows = 0; rows < BLOCK_ROWS; rows++)
        {
            for (int cols = 0; cols < BLOCK_COLS; cols++)
            {
                (blocks[rows][cols]).x1 = cols * BLOCK_WIDTH;
                (blocks[rows][cols]).y1 = rows * BLOCK_HEIGHT;
                (blocks[rows][cols]).x2 = (blocks[rows][cols]).x1 + BLOCK_WIDTH - 1;
                (blocks[rows][cols]).y2 = (blocks[rows][cols]).y1 + BLOCK_HEIGHT - 1;
                if (status[rows][cols] == '#')
                {
                    (blocks[rows][cols]).alive = true;
                }
                else
                {
                    (blocks[rows][cols]).alive = false;
                }

                (blocks[rows][cols]).points = pow(2, BLOCK_ROWS - rows - 1);
                (blocks[rows][cols]).color = rainbow[rows];
            }
        }
    return;
}

void block_draw (SDL_Renderer * renderer, Block blockForRender)
{
    if (blockForRender.alive)
    {    
        SDL_Rect rectForRender;
            rectForRender.w = BLOCK_WIDTH;
            rectForRender.h = BLOCK_HEIGHT;
            rectForRender.x = blockForRender.x1 + WIDTH_OFFSET;
            rectForRender.y = blockForRender.y1 + HEIGHT_OFFSET + BLOCK_ROWS_OFFSET;

        SDL_SetRenderDrawColor
            (renderer, 
            (blockForRender.color).r, 
            (blockForRender.color).g, 
            (blockForRender.color).b, 
            255);

        SDL_RenderFillRect(renderer, &rectForRender);
    }
}

void block_drawLines (SDL_Renderer * renderer)
{
    SDL_SetRenderDrawColor(renderer, 
                           GAME_BACKGROUND_COLOR, 
                           GAME_BACKGROUND_COLOR, 
                           GAME_BACKGROUND_COLOR, 
                           255);

        // Vykresleni horizontalnich car na vizualni oddeleni blocku
        for (int rows = 1; rows < BLOCK_ROWS; rows++)
        {
            SDL_RenderDrawLine(renderer, 
                               WIDTH_OFFSET, 
                               BLOCK_ROWS_OFFSET + HEIGHT_OFFSET + rows*BLOCK_HEIGHT, 
                               WIDTH_OFFSET + BLOCK_COLS*BLOCK_WIDTH-1, 
                               BLOCK_ROWS_OFFSET + HEIGHT_OFFSET + rows*BLOCK_HEIGHT);
            SDL_RenderDrawLine(renderer, 
                               WIDTH_OFFSET, 
                               BLOCK_ROWS_OFFSET + HEIGHT_OFFSET + rows*BLOCK_HEIGHT - 1, 
                               WIDTH_OFFSET + BLOCK_COLS*BLOCK_WIDTH-1, 
                               BLOCK_ROWS_OFFSET + HEIGHT_OFFSET + rows*BLOCK_HEIGHT - 1);
        }
        SDL_RenderDrawLine(renderer, 
                               WIDTH_OFFSET, 
                               BLOCK_ROWS_OFFSET + HEIGHT_OFFSET, 
                               WIDTH_OFFSET + BLOCK_COLS*BLOCK_WIDTH-1, 
                               BLOCK_ROWS_OFFSET + HEIGHT_OFFSET);
        SDL_RenderDrawLine(renderer, 
                           WIDTH_OFFSET, 
                           BLOCK_ROWS_OFFSET + HEIGHT_OFFSET + BLOCK_ROWS*BLOCK_HEIGHT-1, 
                           WIDTH_OFFSET + BLOCK_COLS*BLOCK_WIDTH-1, 
                           BLOCK_ROWS_OFFSET + HEIGHT_OFFSET + BLOCK_ROWS*BLOCK_HEIGHT-1);

        // Vykresleni vertikalnich car na vizualni oddeleni blocku
        for (int cols = 1; cols < BLOCK_COLS; cols++)
        {
            SDL_RenderDrawLine(renderer, 
                               WIDTH_OFFSET + cols*BLOCK_WIDTH, 
                               BLOCK_ROWS_OFFSET + HEIGHT_OFFSET, 
                               WIDTH_OFFSET + cols*BLOCK_WIDTH, 
                               BLOCK_ROWS_OFFSET + HEIGHT_OFFSET + BLOCK_ROWS*BLOCK_HEIGHT-1);
            SDL_RenderDrawLine(renderer, 
                               WIDTH_OFFSET + cols*BLOCK_WIDTH - 1, 
                               BLOCK_ROWS_OFFSET + HEIGHT_OFFSET, 
                               WIDTH_OFFSET + cols*BLOCK_WIDTH - 1, 
                               BLOCK_ROWS_OFFSET + HEIGHT_OFFSET + BLOCK_ROWS*BLOCK_HEIGHT-1);
        }
        SDL_RenderDrawLine(renderer, 
                               WIDTH_OFFSET, 
                               BLOCK_ROWS_OFFSET + HEIGHT_OFFSET, 
                               WIDTH_OFFSET, 
                               BLOCK_ROWS_OFFSET + HEIGHT_OFFSET + BLOCK_ROWS*BLOCK_HEIGHT-1);
        SDL_RenderDrawLine(renderer, 
                           WIDTH_OFFSET + BLOCK_COLS*BLOCK_WIDTH-1, 
                           BLOCK_ROWS_OFFSET + HEIGHT_OFFSET, 
                           WIDTH_OFFSET + BLOCK_COLS*BLOCK_WIDTH-1, 
                           BLOCK_ROWS_OFFSET + HEIGHT_OFFSET + BLOCK_ROWS*BLOCK_HEIGHT-1);
}

bool test_win_state (Block blocks[BLOCK_ROWS][BLOCK_COLS])
{
    for (int rows = 0; rows < BLOCK_ROWS; rows++)
    {
        for (int cols = 0; cols < BLOCK_COLS; cols++)
        {
            if (blocks[rows][cols].alive) return false;
        }
    }
    return true;
}