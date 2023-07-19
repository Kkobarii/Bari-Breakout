#include "libraries/libraries.h"

void initWindowBackground (SDL_Rect * menu_background, SDL_Rect * game_background)
{
    menu_background->x = 0;
    menu_background->y = 0;
    menu_background->w = WINDOW_WIDTH;
    menu_background->h = WINDOW_HEIGHT;

    game_background->x = WIDTH_OFFSET;
    game_background->y = HEIGHT_OFFSET;
    game_background->w = GAME_WIDTH;
    game_background->h = GAME_HEIGHT;
}

void drawWindowBackground (SDL_Renderer * renderer, SDL_Rect menu_background, SDL_Rect game_background)
{
    // Vymazani minule instance okna & vykresleni pozadi menu
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 
                            MENU_BACKGROUND_COLOR, 
                            MENU_BACKGROUND_COLOR, 
                            MENU_BACKGROUND_COLOR, 
                            255);
    SDL_RenderFillRect(renderer, &menu_background);

    // Vykresleni pozadi hry
    SDL_SetRenderDrawColor(renderer, 
                            GAME_BACKGROUND_COLOR, 
                            GAME_BACKGROUND_COLOR, 
                            GAME_BACKGROUND_COLOR, 
                            255);
    SDL_RenderFillRect(renderer, &game_background);
}

void drawText (SDL_Renderer * renderer, TTF_Font * font, SDL_Color color, SDL_Rect rect, char * text) 
{
    SDL_Surface * textSurface = TTF_RenderText_Blended(font, text, color);
    SDL_Texture * message = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_RenderCopy(renderer, message, NULL, &rect);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(message);
}

void drawScore (SDL_Renderer * renderer, int score, TTF_Font * font)
{
    char text[DEFAULT_TEXT_LENGTH];
    sprintf(text, "Score: %d", score);

    int text_width;
    int text_height;
    TTF_SizeText(font, text, &text_width, &text_height);

    SDL_Color color = {255, 255, 255, 0};

    SDL_Rect text_rect;
        text_rect.h = text_height;
        text_rect.w = text_width;
        text_rect.x = WIDTH_OFFSET;
        text_rect.y = 32;

    drawText (renderer, font, color, text_rect, text);
}

void drawLevel (SDL_Renderer * renderer, int level, TTF_Font * font)
{
    char text[DEFAULT_TEXT_LENGTH];
    sprintf(text, "LEVEL %d", level + 1);

    int text_width;
    int text_height;
    TTF_SizeText(font, text, &text_width, &text_height);

    SDL_Color color = {255, 255, 255, 0};

    SDL_Rect text_rect;
        text_rect.h = text_height;
        text_rect.w = text_width;
        text_rect.x = GAME_WIDTH + WIDTH_OFFSET - text_width;
        text_rect.y = 32;

    drawText (renderer, font, color, text_rect, text);
}

void drawHearts (SDL_Renderer * renderer, SDL_Texture * heart1, SDL_Texture * heart2)
{
    SDL_Rect heart1_rect;
        heart1_rect.h = HEART_SIZE;
        heart1_rect.w = HEART_SIZE;
        heart1_rect.x = WINDOW_WIDTH / 2 - HEART_SIZE - 2;
        heart1_rect.y = 32;

    SDL_Rect heart2_rect;
        heart2_rect.h = HEART_SIZE;
        heart2_rect.w = HEART_SIZE;
        heart2_rect.x = WINDOW_WIDTH / 2 + 2;
        heart2_rect.y = 32;
    
    SDL_RenderCopy (renderer, heart1, NULL, &heart1_rect);
    SDL_RenderCopy (renderer, heart2, NULL, &heart2_rect);
}

void drawLives (SDL_Renderer * renderer, int lives, SDL_Texture * heart_full, SDL_Texture * heart_half, SDL_Texture * heart_empty)
{
    switch (lives)
    {
    case 4:
        drawHearts (renderer, heart_full, heart_full);
        break;

    case 3:
        drawHearts (renderer, heart_full, heart_half);
        break;
        
    case 2:
        drawHearts (renderer, heart_full, heart_empty);
        break;
        
    case 1:
        drawHearts (renderer, heart_half, heart_empty);
        break;

    case 0:
        drawHearts (renderer, heart_empty, heart_empty);
        break;
    
    default:
        printf("ERROR: Negative lives\n");
        exit(1);
        break;
    }
}

void drawGameWindow (SDL_Renderer * renderer, Paddle paddle, Ball ball,
                     Block blocks[BLOCK_ROWS][BLOCK_COLS], 
                     int level, int score, int lives, TTF_Font * font,
                     SDL_Texture * heart_full, SDL_Texture * heart_half, SDL_Texture * heart_empty, 
                     SDL_Rect menu_background, SDL_Rect game_background)
{
    // Vykresleni pozadi
    drawWindowBackground(renderer, menu_background, game_background);

    //Vykresleni padla
    paddle_draw(renderer, paddle);

    // Vykresleni blocku  & kulicky
    for (int rows = 0; rows < BLOCK_ROWS; rows++)
    {
        for (int cols = 0; cols < BLOCK_COLS; cols++)
        {
            block_draw(renderer, blocks[rows][cols]);
        }
    }

    block_drawLines(renderer);

    ball_draw(renderer, ball);

    // Vykresleni prvku zahlavi
    drawLives(renderer, lives, heart_full, heart_half, heart_empty);
    drawScore(renderer, score, font);
    drawLevel(renderer, level, font);
}

void drawPauseScreen (SDL_Renderer * renderer, SDL_Texture * overlay, Game_State state,
                      TTF_Font * big_font, TTF_Font * medium_font, Menu_Controls pause_controls, Audio audio)
{
    // Ztmaveni herni plochy
    SDL_Rect overlay_rect;
        overlay_rect.h = GAME_HEIGHT;
        overlay_rect.w = GAME_WIDTH;
        overlay_rect.x = WIDTH_OFFSET;
        overlay_rect.y = HEIGHT_OFFSET;

    SDL_RenderCopy (renderer, overlay, NULL, &overlay_rect);

    // Vypsani PAUSED
    char text[DEFAULT_TEXT_LENGTH];
    sprintf(text, "PAUSED");

    int text_width;
    int text_height;
    TTF_SizeText(big_font, text, &text_width, &text_height);

    SDL_Color color = {255, 255, 255, 0};

    SDL_Rect text_rect;
        text_rect.h = text_height;
        text_rect.w = text_width;
        text_rect.x = (WINDOW_WIDTH - text_width) / 2;
        text_rect.y = HEIGHT_OFFSET + BLOCK_ROWS_OFFSET + BLOCK_HEIGHT*1.5;

    drawText (renderer, big_font, color, text_rect, text);

    // Vypsani instrukci
    drawPauseMenuButtons(renderer, pause_controls, audio, medium_font, state);
}

void drawEndScreen (bool win, SDL_Renderer * renderer, SDL_Texture * overlay, 
                    TTF_Font * big_font, TTF_Font * medium_font, 
                    int score, int highscores[NUMBER_OF_LEVELS], Game_State * state, int level,
                    SDL_Color rainbow[BLOCK_ROWS], int rainbow_offset, Menu_Controls pause_controls, Audio audio)
{
    // Ztmaveni herni plochy
    SDL_Rect overlay_rect;
        overlay_rect.h = GAME_HEIGHT;
        overlay_rect.w = GAME_WIDTH;
        overlay_rect.x = WIDTH_OFFSET;
        overlay_rect.y = HEIGHT_OFFSET;

    SDL_RenderCopy (renderer, overlay, NULL, &overlay_rect);

    // Vypsani GAME OVER nebo LEVEL CLEARED
    char text[DEFAULT_TEXT_LENGTH];

    if (win)
    {
        sprintf(text, "LEVEL CLEARED");
    }
    else
    {
        sprintf(text, "GAME OVER");
    }

    int text_width;
    int text_height;
    TTF_SizeText(big_font, text, &text_width, &text_height);

    SDL_Color color = {255, 255, 255, 0};

    SDL_Rect text_rect;
        text_rect.h = text_height;
        text_rect.w = text_width;
        text_rect.x = (WINDOW_WIDTH - text_width) / 2;
        text_rect.y = HEIGHT_OFFSET + BLOCK_ROWS_OFFSET + BLOCK_HEIGHT*1.5;

    drawText (renderer, big_font, color, text_rect, text);

    // Vypsani score 
    sprintf(text, "Score: %d", score);
    TTF_SizeText(medium_font, text, &text_width, &text_height);

    text_rect.h = text_height;
    text_rect.w = text_width;
    text_rect.x = WIDTH_OFFSET * 2;
    text_rect.y = HEIGHT_OFFSET + 200;

    drawText (renderer, medium_font, color, text_rect, text);

    // Highscore oceneni
    if (score > highscores[level])
    {
        state->highscore = true;
    }
    if (state->highscore)
    {
        highscores[level] = score;
        writeHighscores(highscores);

        sprintf(text, "NEW HIGHSCORE!");
        TTF_SizeText(medium_font, text, &text_width, &text_height);

        drawRainbowText(text, WIDTH_OFFSET * 2, HEIGHT_OFFSET + 200 + text_height, 
                        renderer, medium_font, rainbow, rainbow_offset);
    }

    // Vypsani instrukci
    drawPauseMenuButtons(renderer, pause_controls, audio, medium_font, *state);
}

void drawMenuWindow (SDL_Renderer * renderer, SDL_Rect menu_background, SDL_Rect game_background,
                     TTF_Font * big_font, TTF_Font * medium_font, TTF_Font * small_font, SDL_Color rainbow[BLOCK_ROWS], int rainbow_offset,
                     int highscores[NUMBER_OF_LEVELS], Menu_Controls * menu_controls)
{
    char text[DEFAULT_TEXT_LENGTH];
    SDL_Color color = {255, 255, 255, 0};
    SDL_Rect text_rect;
    int text_width;
    int text_height;

    // Vykresleni pozadi
    drawWindowBackground(renderer, menu_background, game_background);

    // Vypsani nazvu hry
    int letter_width;
    int letter_height;
    TTF_SizeText(big_font, text, &letter_width, &letter_height);
    sprintf(text, "BREAKOUT");
    drawRainbowText(text, WIDTH_OFFSET, (HEIGHT_OFFSET - letter_height)/2, renderer, big_font, rainbow, rainbow_offset);

    // Vypsani jmena autorky
    sprintf(text, "Made by:");
    TTF_SizeText(small_font, text, &text_width, &text_height);

    text_rect.h = text_height;
    text_rect.w = text_width;
    text_rect.x = WINDOW_WIDTH - WIDTH_OFFSET - text_width;
    text_rect.y = HEIGHT_OFFSET / 2 - text_height;

    drawText (renderer, small_font, color, text_rect, text);

    sprintf(text, "Barbora Kovalska");
    TTF_SizeText(small_font, text, &text_width, &text_height);

    text_rect.h = text_height;
    text_rect.w = text_width;
    text_rect.x = WINDOW_WIDTH - WIDTH_OFFSET - text_width;
    text_rect.y = HEIGHT_OFFSET / 2;

    drawText (renderer, small_font, color, text_rect, text);
    
    // Vykresleni carky nad a (nezbytne)
    int line_x = text_rect.x + 113;
    int line_y = text_rect.y + 6;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawLine(renderer, line_x, line_y, line_x + 3, line_y - 1);

    // Vypsani levelu & nadpisu
    sprintf(text, "Choose a level:");
    TTF_SizeText(medium_font, text, &text_width, &text_height);

    text_rect.h = text_height;
    text_rect.w = text_width;
    text_rect.x = WIDTH_OFFSET * 2;
    text_rect.y = HEIGHT_OFFSET + WIDTH_OFFSET;

    drawText (renderer, medium_font, color, text_rect, text);

    SDL_Rect level_rect = {0, 0, 80, 80};

    for (int rows = 0; rows < 2; rows++)
    {
        for (int cols = 0; cols < 4; cols++)
        {
            level_rect.y = HEIGHT_OFFSET + 60 + 10 + rows * (level_rect.h + 20);
            level_rect.x = WIDTH_OFFSET + 10 + cols * (level_rect.w + 20);

            if ((cols + rows * 4) == menu_controls->button_selected)
                SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
            else
                SDL_SetRenderDrawColor(renderer, 55, 55, 55, 255);

            SDL_RenderFillRect(renderer, &level_rect);

            sprintf(text, "%d", cols + rows * 4 + 1);
            TTF_SizeText(big_font, text, &text_width, &text_height);

            text_rect.h = text_height;
            text_rect.w = text_width;
            text_rect.x = WIDTH_OFFSET +  cols * 100 + (100 - text_width)/2;
            text_rect.y = HEIGHT_OFFSET + 60 + rows * 100 + (100 - text_height)/2;

            drawText (renderer, big_font, color, text_rect, text);
        }
    }

    // Vykresleni momentalne oznaceneho levelu

    SDL_SetRenderDrawColor(renderer, 55, 55, 55, 255);
    SDL_Rect selected_level_rect;
        selected_level_rect.w = 300;
        selected_level_rect.h = 200;
        selected_level_rect.x = WIDTH_OFFSET + (GAME_WIDTH - selected_level_rect.w)/2;
        selected_level_rect.y = 270 + HEIGHT_OFFSET;
    SDL_RenderFillRect(renderer, &selected_level_rect);

    sprintf(text, "LEVEL %d", menu_controls->button_selected + 1);
    TTF_SizeText(big_font, text, &text_width, &text_height);

    text_rect.h = text_height;
    text_rect.w = text_width;
    text_rect.x = (WINDOW_WIDTH - text_width) / 2;
    text_rect.y = selected_level_rect.y + 10;

    drawText (renderer, big_font, color, text_rect, text);

    sprintf(text, "Highscore:");
    TTF_SizeText(medium_font, text, &text_width, &text_height);

    text_rect.h = text_height;
    text_rect.w = text_width;
    text_rect.x = (WINDOW_WIDTH - text_width) / 2;
    text_rect.y = selected_level_rect.y + selected_level_rect.h/2;

    drawText (renderer, medium_font, color, text_rect, text);

    sprintf(text, "%d", highscores[menu_controls->button_selected]);
    TTF_SizeText(medium_font, text, &text_width, &text_height);

    text_rect.h = text_height;
    text_rect.w = text_width;
    text_rect.x = (WINDOW_WIDTH - text_width) / 2;
    text_rect.y = selected_level_rect.y + selected_level_rect.h/2 + text_height;

    drawText (renderer, medium_font, color, text_rect, text);
}

void drawPauseMenuButtons (SDL_Renderer * renderer, Menu_Controls pause_controls, Audio audio, TTF_Font * medium_font, Game_State state)
{
    SDL_Rect button_rect = {0, 0, 180, 50};
    SDL_Rect text_rect;
    int text_width;
    int text_height;
    char text[DEFAULT_TEXT_LENGTH];
    SDL_Color color = {255, 255, 255, 0};

    for (int rows = 0; rows < 2; rows++)
    {
        for (int cols = 0; cols < 2; cols++)
        {
            button_rect.y = HEIGHT_OFFSET + 300 + 10 + rows * (button_rect.h + 10);
            button_rect.x = WIDTH_OFFSET + 15 + cols * (button_rect.w + 10);

            if ((cols + rows * 2) == pause_controls.button_selected)
                SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
            else
                SDL_SetRenderDrawColor(renderer, 55, 55, 55, 255);

            SDL_RenderFillRect(renderer, &button_rect);

            switch (cols + rows * 2)
            {
            case 0:
                sprintf(text, "Restart the level");
                break;
                
            case 1:
                sprintf(text, "Return to menu");
                break;

            case 2:
                if(audio.play)
                    sprintf(text, "Mute sounds");
                else
                    sprintf(text, "Unmute sounds");

                break;

            case 3:
                sprintf(text, "Quit the game");
                break;            
            
            default:
                break;
            }

            TTF_SizeText(medium_font, text, &text_width, &text_height);

            text_rect.h = text_height;
            text_rect.w = text_width;
            text_rect.x = button_rect.x + (button_rect.w - text_width)/2;
            text_rect.y = button_rect.y + (button_rect.h - text_height)/2;

            drawText (renderer, medium_font, color, text_rect, text);
        }

        if (!state.end)
        {        
            if (4 == pause_controls.button_selected)
                SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
            else
                SDL_SetRenderDrawColor(renderer, 55, 55, 55, 255);

            button_rect.y = HEIGHT_OFFSET + 300 + 10 - (button_rect.h + 10);
            button_rect.x = (WINDOW_WIDTH - button_rect.w)/2;

            SDL_RenderFillRect(renderer, &button_rect);

            sprintf(text, "Continue");

            TTF_SizeText(medium_font, text, &text_width, &text_height);

            text_rect.h = text_height;
            text_rect.w = text_width;
            text_rect.x = button_rect.x + (button_rect.w - text_width)/2;
            text_rect.y = button_rect.y + (button_rect.h - text_height)/2;

            drawText (renderer, medium_font, color, text_rect, text);
        }
    }
}

void drawRainbowText (char text[DEFAULT_TEXT_LENGTH], int x, int y, SDL_Renderer * renderer, TTF_Font * font, SDL_Color rainbow[BLOCK_ROWS], int rainbow_offset)
{
    char letter[2];
    int letter_width;
    int letter_height;

    // Zjisteni vysky textu
    sprintf(letter, ".");
    TTF_SizeText(font, letter, &letter_width, &letter_height);
    SDL_Rect letter_rect;
    letter_rect.x = x;
    letter_rect.y = y;
    letter_rect.w = 0;

    SDL_Color color = {255, 255, 255, 255};
    
    // Vykresleni pismenek
    for (int i = 0; i < (int)SDL_strlen(text); i++)
    {
        sprintf(letter, "%c", text[i]);
        color = rainbow[(i + rainbow_offset) % BLOCK_ROWS];

        TTF_SizeText(font, letter, &letter_width, &letter_height);

        letter_rect.x = letter_rect.x + letter_rect.w;
        letter_rect.y = letter_rect.y;
        letter_rect.h = letter_height;
        letter_rect.w = letter_width;

        drawText (renderer, font, color, letter_rect, letter);
    }
}