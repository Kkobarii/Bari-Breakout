#include "libraries/libraries.h"

void resetHighscores (int highscores[NUMBER_OF_LEVELS])
{
    for (int lvl = 0; lvl < NUMBER_OF_LEVELS; lvl++)
    {
        highscores[lvl] = 0;
    }

    FILE * file = fopen("assets/highscores.txt", "w");
    if (file == NULL)
    {
        printf("ERROR: highscores file not found");
        exit(1);
    }

    for (int lvl = 0; lvl < NUMBER_OF_LEVELS; lvl++)
    {
        fprintf (file, "%d\n", highscores[lvl]);
    }
    fclose(file);
}

void writeHighscores (int highscores[NUMBER_OF_LEVELS])
{
    FILE * file = fopen("assets/highscores.txt", "w");
    if (file == NULL)
    {
        printf("ERROR: highscores file not found");
        exit(1);
    }

    for (int lvl = 0; lvl < NUMBER_OF_LEVELS; lvl++)
    {
        fprintf (file, "%d\n", highscores[lvl]);
    }
    fclose(file);
}

void readHighscores (int highscores[NUMBER_OF_LEVELS])
{
    FILE * file = fopen("assets/highscores.txt", "r");
    if (file == NULL)
    {
        printf("ERROR: highscores file not found");
        exit(1);
    }

    for (int lvl = 0; lvl < NUMBER_OF_LEVELS; lvl++)
    {
        fscanf (file, " %d", &(highscores[lvl]));
    }
    fclose(file);
}

void readSettings (Audio * audio)
{
    int tmp;

    FILE * file = fopen("./assets/settings.txt", "r");
    if (file == NULL)
    {
        printf("ERROR: settings file not found");
        exit(1);
    }
    fscanf (file, "%d", &tmp);

    audio->play = tmp == 1 ? true : false;

    fclose(file);
}

void switchMuteStatus (Audio * audio)
{
    audio->play = !(audio->play);

    int tmp;

    FILE * file = fopen("assets/settings.txt", "w");
    if (file == NULL)
    {
        printf("ERROR: highscores file not found");
        exit(1);
    }
    tmp = audio->play ? 1 : 0;

    fprintf (file, "%d\n", tmp);
    
    fclose(file);
}

void loadLevel (int level, Positions positions[NUMBER_OF_LEVELS], Block blocks[BLOCK_ROWS][BLOCK_COLS], Paddle * paddle, Ball * ball,
                SDL_Color rainbow[BLOCK_ROWS], int * lives, int * score,
                Game_State * state, int * speedup_counter)
{
    blocks_init(blocks, rainbow, (positions[level]).status);
    paddle_init(paddle);
    ball_init(ball, *paddle);

    *lives = MAX_LIVES;
    ball->speed = 1;
    *score = 0;
    state->play = true;
    state->menu = false;
    state->pause = false;
    state->end = false;
    state->highscore = false;
    *speedup_counter = 0;
}

SDL_Texture * loadTexture (SDL_Renderer * renderer, char * path)
{
    SDL_Surface * image_surface = IMG_Load(path);
    SDL_Texture * image = SDL_CreateTextureFromSurface(renderer, image_surface);
    SDL_FreeSurface(image_surface);

    return image;
}

void speedupCheck (int * speedup_counter, Ball * ball)
{
    if (ball->speed <= MAX_SPEED)
    {
        if (*speedup_counter >= 32)
        {
            *speedup_counter = 0;
            ball->speed++;
        }
    }
}

bool checkMouseHover(SDL_Rect level_rect)
{
    int mouse_x, mouse_y;

    SDL_PumpEvents();

    SDL_GetMouseState(&mouse_x, &mouse_y);
    if ((mouse_x > level_rect.x) && (mouse_x < level_rect.x + level_rect.w) &&
        (mouse_y > level_rect.y) && (mouse_y < level_rect.y + level_rect.h)) 
    {
        return true;
    }
    return false;
}

bool mouseHoverOverLevel()
{
    SDL_Rect level_rect = {0, 0, 80, 80};

    for (int rows = 0; rows < 2; rows++)
    {
        for (int cols = 0; cols < 4; cols++)
        {
            level_rect.y = HEIGHT_OFFSET + 60 + 10 + rows * (level_rect.h + 20);
            level_rect.x = WIDTH_OFFSET + 10 + cols * (level_rect.w + 20);

            if (checkMouseHover(level_rect))
                return true;
        }
    }

    return false;
}

bool mouseHoverOverSetting(Game_State state)
{
    SDL_Rect button_rect = {0, 0, 180, 50};

    for (int rows = 0; rows < 2; rows++)
    {
        for (int cols = 0; cols < 2; cols++)
        {
            button_rect.y = HEIGHT_OFFSET + 300 + 10 + rows * (button_rect.h + 10);
            button_rect.x = WIDTH_OFFSET + 15 + cols * (button_rect.w + 10);
        
            if (checkMouseHover(button_rect))
                return true;
        }
    }

    if (!state.end)
    {
        button_rect.y = HEIGHT_OFFSET + 300 + 10 - (button_rect.h + 10);
        button_rect.x = (WINDOW_WIDTH - button_rect.w)/2;

        if (checkMouseHover(button_rect))
                return true;
    }

    return false;
}

void selectSetting (Menu_Controls pause_controls, Game_State * state, Audio * audio,
                    int level, Positions positions[NUMBER_OF_LEVELS], Block blocks[BLOCK_ROWS][BLOCK_COLS], Paddle * paddle, Ball * ball,
                    SDL_Color rainbow[BLOCK_ROWS], int * lives, int * score, int * speedup_counter)
{
    if (pause_controls.button_selected == 0)
    {
        loadLevel(level, positions, blocks, paddle, ball,
            rainbow, lives, score, state, speedup_counter); 
    }
    else if (pause_controls.button_selected == 1)
    {
        state->menu = true;
        state->win = false;
    }
    else if (pause_controls.button_selected == 2)
    {
        switchMuteStatus(audio); 
    }
    else if (pause_controls.button_selected == 3)
    {
        state->quit = true; 
    }
    else if (pause_controls.button_selected == 4 && !state->end)
    {
        state->pause = false;
        state->play = true;
    }
}

void checkMenuInputs (SDL_Event e, Menu_Controls * menu_controls,
                      Audio audio, Wav scroll)
{
    switch (e.key.keysym.scancode)
    {
    case SDL_SCANCODE_RIGHT:
        if (menu_controls->button_selected < NUMBER_OF_LEVELS - 1)
        {
            menu_controls->button_selected++;
            if (audio.play)
            {
                SDL_ClearQueuedAudio(audio.device);
                SDL_QueueAudio(audio.device, scroll.wav_buffer, scroll.wav_length);
            }
        }
        break;
    
    case SDL_SCANCODE_LEFT:
        if (menu_controls->button_selected > 0)
        {
            menu_controls->button_selected--;
            if (audio.play)
            {
                SDL_ClearQueuedAudio(audio.device);
                SDL_QueueAudio(audio.device, scroll.wav_buffer, scroll.wav_length);
            }
        }
        break;

    case SDL_SCANCODE_DOWN:
        if (menu_controls->button_selected < 4)
        {
            menu_controls->button_selected += 4;
            if (audio.play)
            {
                SDL_ClearQueuedAudio(audio.device);
                SDL_QueueAudio(audio.device, scroll.wav_buffer, scroll.wav_length);
            }
        }                            
        break;

    case SDL_SCANCODE_UP:
        if (menu_controls->button_selected > 3)
        {
            menu_controls->button_selected -= 4;
            if (audio.play)
            {
                SDL_ClearQueuedAudio(audio.device);
                SDL_QueueAudio(audio.device, scroll.wav_buffer, scroll.wav_length);
            }
        } 
        break;

    case SDL_SCANCODE_RETURN:
    case SDL_SCANCODE_E:
        menu_controls->button_pressed = true;
        break;

    case SDL_SCANCODE_1:
        menu_controls->button_selected = 0;
        menu_controls->button_pressed = true;
        break;

    case SDL_SCANCODE_2:
        menu_controls->button_selected = 1;
        menu_controls->button_pressed = true;
        break;

    case SDL_SCANCODE_3:
        menu_controls->button_selected = 2;
        menu_controls->button_pressed = true;
        break;

    case SDL_SCANCODE_4:
        menu_controls->button_selected = 3;
        menu_controls->button_pressed = true;
        break;

    case SDL_SCANCODE_5:
        menu_controls->button_selected = 4;
        menu_controls->button_pressed = true;
        break;

    case SDL_SCANCODE_6:
        menu_controls->button_selected = 5;
        menu_controls->button_pressed = true;
        break;

    case SDL_SCANCODE_7:
        menu_controls->button_selected = 6;
        menu_controls->button_pressed = true;
        break;

    case SDL_SCANCODE_8:
        menu_controls->button_selected = 7;
        menu_controls->button_pressed = true;
        break;
    
    default:
        break;
    }
}

void checkGameInputs (SDL_Event e, Game_Controls * game_controls)
{
    switch (e.key.keysym.scancode)
    {
    // Pohyb doprava
    case SDL_SCANCODE_D:
    case SDL_SCANCODE_RIGHT:
        game_controls->right = true;
        break;

    // Pohyb doleva
    case SDL_SCANCODE_A:
    case SDL_SCANCODE_LEFT:
        game_controls->left = true;
        break;

    // Paddle cheat
    case SDL_SCANCODE_Q:
        game_controls->paddle_cheat = !(game_controls->paddle_cheat);
        break;

    // Cheat vyhry
    case SDL_SCANCODE_C:
        game_controls->win_cheat = true;
        break;

    // Cheat prohry
    case SDL_SCANCODE_X:
        game_controls->death_cheat = true;
        break;

    // Pauznuti hry
    case SDL_SCANCODE_ESCAPE:
        game_controls->pause_switch = true;
        break;

    default:
        break;
    }
}

void checkPauseInputs (SDL_Event e, Menu_Controls * pause_controls, Game_Controls * game_controls,
                       Audio audio, Wav scroll, Game_State state)
{
    switch (e.key.keysym.scancode)
    {
    // Unpause
    case SDL_SCANCODE_ESCAPE:
        game_controls->pause_switch = true;
        break;
    // Restart levelu
    case SDL_SCANCODE_R:     
    case SDL_SCANCODE_1:                
        pause_controls->button_selected = 0;  
        pause_controls->button_pressed = true;                        
        break;

    // Vraceni do menu
    case SDL_SCANCODE_M:
    case SDL_SCANCODE_2:
        pause_controls->button_selected = 1;
        pause_controls->button_pressed = true;  
        break;

    // Mutnuti zvuku
    case SDL_SCANCODE_N:
    case SDL_SCANCODE_3:
        pause_controls->button_selected = 2;   
        pause_controls->button_pressed = true;                       
        break;

    // Vypnuti hry
    case SDL_SCANCODE_B:
    case SDL_SCANCODE_4:
        pause_controls->button_selected = 3;
        pause_controls->button_pressed = true;  
        break;

    case SDL_SCANCODE_RIGHT:
        if (pause_controls->button_selected < 3)
        {
            pause_controls->button_selected++;
            if (audio.play)
            {
                SDL_ClearQueuedAudio(audio.device);
                SDL_QueueAudio(audio.device, scroll.wav_buffer, scroll.wav_length);
            }
        }
        else if (pause_controls->button_selected == 4)
        {
            pause_controls->button_selected = 0;
        }
        break;
    
    case SDL_SCANCODE_LEFT:
        if (pause_controls->button_selected > 0 && pause_controls->button_selected < 4)
        {
            pause_controls->button_selected--;
            if (audio.play)
            {
                SDL_ClearQueuedAudio(audio.device);
                SDL_QueueAudio(audio.device, scroll.wav_buffer, scroll.wav_length);
            }
        }
        else if (!state.end && pause_controls->button_selected == 0)
        {
            pause_controls->button_selected = 4;
        }
        break;

    case SDL_SCANCODE_DOWN:
        if (pause_controls->button_selected == 0 || pause_controls->button_selected == 1)
        {
            pause_controls->button_selected += 2;
            if (audio.play)
            {
                SDL_ClearQueuedAudio(audio.device);
                SDL_QueueAudio(audio.device, scroll.wav_buffer, scroll.wav_length);
            }
        }
        else if (pause_controls->button_selected == 4)
        {
            pause_controls->button_selected = 0;
        }
        break;

    case SDL_SCANCODE_UP:
        if (pause_controls->button_selected == 2 || pause_controls->button_selected == 3)
        {
            pause_controls->button_selected -= 2;
            if (audio.play)
            {
                SDL_ClearQueuedAudio(audio.device);
                SDL_QueueAudio(audio.device, scroll.wav_buffer, scroll.wav_length);
            }
        }
        else if (!state.end && (pause_controls->button_selected == 0 || pause_controls->button_selected == 1))
            pause_controls->button_selected = 4;
        break;

    case SDL_SCANCODE_RETURN:
    case SDL_SCANCODE_E:
        pause_controls->button_pressed = true;
        break;

    default:
        break;
    }
}