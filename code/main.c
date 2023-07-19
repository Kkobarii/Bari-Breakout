#include "libraries/libraries.h"

int main(int argc, char *argv[])
{
    // Aby na me Kelvin nekricel
    UNUSED(argc);
    UNUSED(argv);

    // Priprava okna
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        printf("ERROR: SDL_Init failed\n");
        exit(1);
    }

    SDL_Window * window = SDL_CreateWindow("Bari Breakout",
                                           SDL_WINDOWPOS_CENTERED,
                                           SDL_WINDOWPOS_CENTERED,
                                           WINDOW_WIDTH, WINDOW_HEIGHT,
                                           SDL_WINDOW_SHOWN);

    if (!window)
    {
        printf("ERROR: SDL_CreateWindow failed\n");
        exit(1);
    }

    //Nacteni & nastaveni ikony okna
    SDL_Surface * icon = IMG_Load("assets/images/icon.png");
    if (!icon)
    {
        printf("ERROR: couldn't create surface\n");
        exit(1);
    }
    SDL_SetWindowIcon(window, icon);
    SDL_FreeSurface(icon);

    // Priprava rendereru
    SDL_Renderer *renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED);

    if (!renderer)
    {
        printf("ERROR: couldn't create renderer\n");
        exit(1);
    }

    // Spusteni TTF & nacteni fontu
    if (TTF_Init() < 0)
    {
        printf("ERROR: TTF_Init failed\n");
        exit(1);
    }

    char * font_path = "./assets/fonts/UniversCondensed.ttf";

    TTF_Font * small_font = TTF_OpenFont(font_path, 20);
    TTF_Font * medium_font = TTF_OpenFont(font_path, 32);
    TTF_Font * big_font = TTF_OpenFont(font_path, 64);
    
    if (small_font == NULL || medium_font == NULL || big_font == NULL)
    {
        printf("ERROR: font asset file not found\n");
        exit(1);
    }

    // Nacteni zvuku
    Wav bounce_paddle;
    Wav bounce_wall;
    Wav bounce_brick;
    Wav celebration;
    Wav fail;
    Wav life_lost;
    Wav scroll;
    Wav select;

    Audio audio;
    Audio music;

    if (!SDL_LoadWAV("./assets/sounds/bounce_paddle.wav", &(bounce_paddle.wav_spec), &(bounce_paddle.wav_buffer), &(bounce_paddle.wav_length)))
    {
        printf("ERROR: bounce_paddle asset file not found\n");
        exit(1);
    }
    if (!SDL_LoadWAV("./assets/sounds/bounce_wall.wav", &(bounce_wall.wav_spec), &(bounce_wall.wav_buffer), &(bounce_wall.wav_length)))
    {
        printf("ERROR: bounce_wall asset file not found\n");
        exit(1);
    }
    if (!SDL_LoadWAV("./assets/sounds/bounce_brick.wav", &(bounce_brick.wav_spec), &(bounce_brick.wav_buffer), &(bounce_brick.wav_length)))
    {
        printf("ERROR: bounce_brick asset file not found\n");
        exit(1);
    }
    if (!SDL_LoadWAV("./assets/sounds/celebration.wav", &(celebration.wav_spec), &(celebration.wav_buffer), &(celebration.wav_length)))
    {
        printf("ERROR: celebration asset file not found\n");
        exit(1);
    }
    if (!SDL_LoadWAV("./assets/sounds/fail.wav", &(fail.wav_spec), &(fail.wav_buffer), &(fail.wav_length)))
    {
        printf("ERROR: fail asset file not found\n");
        exit(1);
    }
    if (!SDL_LoadWAV("./assets/sounds/life_lost.wav", &(life_lost.wav_spec), &(life_lost.wav_buffer), &(life_lost.wav_length)))
    {
        printf("ERROR: life_lost asset file not found\n");
        exit(1);
    }
    if (!SDL_LoadWAV("./assets/sounds/scroll.wav", &(scroll.wav_spec), &(scroll.wav_buffer), &(scroll.wav_length)))
    {
        printf("ERROR: scroll asset file not found\n");
        exit(1);
    }
    if (!SDL_LoadWAV("./assets/sounds/select.wav", &(select.wav_spec), &(select.wav_buffer), &(select.wav_length)))
    {
        printf("ERROR: select asset file not found\n");
        exit(1);
    }

    music.device = SDL_OpenAudioDevice(NULL, 0, &(bounce_brick.wav_spec), NULL, 0);
    audio.device = SDL_OpenAudioDevice(NULL, 0, &(bounce_brick.wav_spec), NULL, 0);
    readSettings(&audio);

    SDL_PauseAudioDevice(audio.device, 0); 
    SDL_PauseAudioDevice(music.device, 0); 

    // Nacteni srdicek & overlay
    char * heart_full_path = "./assets/images/heart_full.png";
    char * heart_half_path = "./assets/images/heart_half.png";
    char * heart_empty_path = "./assets/images/heart_empty.png";

    SDL_Texture * heart_full = loadTexture (renderer, heart_full_path);
    SDL_Texture * heart_half = loadTexture (renderer, heart_half_path);
    SDL_Texture * heart_empty = loadTexture (renderer, heart_empty_path);    

    if (heart_full == NULL || heart_half == NULL || heart_empty == NULL)
    {
        printf("ERROR: heart asset file not found\n");
        exit(1);
    }

    char * overlay_path = "./assets/images/overlay_black.png";

    SDL_Texture * overlay = loadTexture (renderer, overlay_path);

    if (overlay == NULL)
    {
        printf("ERROR: overlay asset file not found\n");
        exit(1);
    }

    SDL_SetTextureAlphaMod (overlay, 200);

    // Definice promennych
    int speedup_counter = 0;
    int running_counter = 0;
    int level = 0;
    int score = 0;
    int lives = MAX_LIVES;

    int highscores[NUMBER_OF_LEVELS];
    //resetHighscores(highscores);
    readHighscores(highscores);

    SDL_Rect menu_background;
    SDL_Rect game_background;
    initWindowBackground(&menu_background, &game_background);

    Positions positions[NUMBER_OF_LEVELS];
    scan_positions(positions);

    SDL_Color rainbow[BLOCK_ROWS];
    rainbow_init(rainbow);
    int rainbow_offset = 0;

    Block blocks[BLOCK_ROWS][BLOCK_COLS];
    blocks_init(blocks, rainbow, (positions[level]).status);

    Paddle paddle;
    paddle_init(&paddle);

    Ball ball;
    ball_init(&ball, paddle);

    SDL_Event e;
    
    Game_State state;
        state.quit = false;
        state.win = false;
        state.highscore = false;
        state.play = true;
        state.menu = true;
        state.pause = false;
        state.end = false;

    Menu_Controls menu_controls;
        menu_controls.right = false;
        menu_controls.left = false;
        menu_controls.button_pressed = false;
        menu_controls.button_selected = 0;
        menu_controls.old_button_selected = 0;

    Menu_Controls pause_controls;
        pause_controls.right = false;
        pause_controls.left = false;
        pause_controls.button_pressed = false;
        pause_controls.button_selected = 0;
        pause_controls.old_button_selected = 0;

    Game_Controls game_controls;
        game_controls.right = false;
        game_controls.left = false;
        game_controls.paddle_cheat = false;
        game_controls.win_cheat = false;
        game_controls.death_cheat = false;
        game_controls.pause_switch = false;

    // Game loop
    while (!state.quit)
    {
        // Kontrola eventu
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            // Osetreni uzavreni okna
            case SDL_QUIT:
                state.quit = true;
                break;
            
            // Vyhodnoceni stisknuti mysi
            case SDL_MOUSEBUTTONDOWN:
                if (e.button.button == SDL_BUTTON_LEFT)
                {
                    if (state.menu && mouseHoverOverLevel())
                    {
                        level = menu_controls.button_selected;
                        loadLevel(level, positions, blocks, &paddle, &ball,
                                    rainbow, &lives, &score, &state, &speedup_counter);
                        if (audio.play)
                        {
                            SDL_ClearQueuedAudio(audio.device);
                            SDL_QueueAudio(audio.device, select.wav_buffer, select.wav_length);
                        }
                    }
                    if (!state.menu && !state.play && mouseHoverOverSetting(state))
                    {
                        selectSetting(pause_controls, &state, &audio, level, positions, blocks, &paddle, &ball, rainbow, &lives, &score, &speedup_counter);
                        if (audio.play)
                        {
                            SDL_ClearQueuedAudio(audio.device);
                            SDL_QueueAudio(audio.device, select.wav_buffer, select.wav_length);
                        }
                    }
                }
                break;

            // Cteni z klaves
            case SDL_KEYDOWN:
                // Moznosti v menu
                if (state.menu)
                {
                    checkMenuInputs(e, &menu_controls, audio, scroll);
                }
                // Moznosti ve hre
                else if (state.play)
                {
                    checkGameInputs(e, &game_controls);
                }
                // Moznosti v pause menu
                else
                {
                    checkPauseInputs(e, &pause_controls, &game_controls, audio, scroll, state);     
                }
                break;

            // Cteni keyup (pro lepsi pohyb padla)
            case SDL_KEYUP:
                switch (e.key.keysym.scancode)
                {
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    game_controls.right = false;
                    break;

                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    game_controls.left = false;
                    break;

                default:
                    break;
                }
                break;
            }
        }

        if (state.play)
        {
            // Posun padla
            paddle_move_check (&paddle, ball, game_controls);

            // Kontrola odrazu kulicky & posun jeji pozice
            ball_move_check (&score,
                             &lives,
                             &speedup_counter,
                             &ball,
                             paddle,
                             blocks,
                             audio,
                             bounce_paddle,
                             bounce_wall,
                             bounce_brick,
                             life_lost);

            speedupCheck (&speedup_counter, &ball);
        }

        state.win = test_win_state(blocks);
        state.end = state.win || lives == 0;

        // Vyhodnoceni eventu
        if (state.menu)
        {
            if (menu_controls.button_pressed)
            {
                level = menu_controls.button_selected;
                loadLevel(level, positions, blocks, &paddle, &ball,
                    rainbow, &lives, &score, &state, &speedup_counter);
                if (audio.play)
                {
                    SDL_ClearQueuedAudio(audio.device);
                    SDL_QueueAudio(audio.device, select.wav_buffer, select.wav_length);
                }
                menu_controls.button_pressed = false;
            }
        }
        else if (state.play)
        {
            if (game_controls.win_cheat)
            {
                for (int rows = 0; rows < BLOCK_ROWS; rows++)
                {
                    for (int cols = 0; cols < BLOCK_COLS; cols++)
                    {
                        if ((blocks[rows][cols]).alive)
                        {
                            (blocks[rows][cols]).alive = false;
                        }
                    }
                }
                game_controls.win_cheat = false;
            }
            else if(game_controls.death_cheat)
            {
                lives = 0;
                game_controls.death_cheat = false;
            }
            else if (game_controls.pause_switch)
            {
                state.pause = true;
                pause_controls.button_selected = 4;
                game_controls.pause_switch = false;
            }
        }
        else
        {
            if (game_controls.pause_switch)
            {
                state.pause = false;
                game_controls.pause_switch = false;
            }
            if (pause_controls.button_pressed)
            {                
                selectSetting(pause_controls, &state, &audio, level, positions, blocks, &paddle, &ball, rainbow, &lives, &score, &speedup_counter);
            }
            pause_controls.button_pressed = false;
        }

        // Vykresleni menu
        if (state.menu)
        {
            state.play = false;

            SDL_Rect level_rect = {0, 0, 80, 80};

            for (int rows = 0; rows < 2; rows++)
            {
                for (int cols = 0; cols < 4; cols++)
                {
                    level_rect.y = HEIGHT_OFFSET + 60 + 10 + rows * (level_rect.h + 20);
                    level_rect.x = WIDTH_OFFSET + 10 + cols * (level_rect.w + 20);
                    
                    if (checkMouseHover(level_rect))
                        menu_controls.button_selected = cols + rows * 4;
                    
                    if (audio.play && menu_controls.button_selected != menu_controls.old_button_selected)
                    {
                        SDL_ClearQueuedAudio(audio.device);
                        SDL_QueueAudio(audio.device, scroll.wav_buffer, scroll.wav_length);
                    }
                    menu_controls.old_button_selected = menu_controls.button_selected;
                }
            }
            
            drawMenuWindow (renderer, menu_background, game_background,
                            big_font, medium_font, small_font, rainbow, rainbow_offset, 
                            highscores, &menu_controls);
        }
        // Vykresleni pause menu
        else if (state.pause)
        {
            state.play = false;

            SDL_Rect button_rect = {0, 0, 180, 50};

            for (int rows = 0; rows < 2; rows++)
            {
                for (int cols = 0; cols < 2; cols++)
                {
                    button_rect.y = HEIGHT_OFFSET + 300 + 10 + rows * (button_rect.h + 10);
                    button_rect.x = WIDTH_OFFSET + 15 + cols * (button_rect.w + 10);
                    
                    if (checkMouseHover(button_rect))
                        pause_controls.button_selected = cols + rows * 2;
                }
                button_rect.y = HEIGHT_OFFSET + 300 + 10 - (button_rect.h + 10);
                button_rect.x = (WINDOW_WIDTH - button_rect.w)/2;
                
                if (checkMouseHover(button_rect))
                    pause_controls.button_selected = 4;

                if (audio.play && pause_controls.button_selected != pause_controls.old_button_selected)
                {
                    SDL_ClearQueuedAudio(audio.device);
                    SDL_QueueAudio(audio.device, scroll.wav_buffer, scroll.wav_length);
                }
                pause_controls.old_button_selected = pause_controls.button_selected;
            }

            drawGameWindow (renderer, paddle, ball, blocks,
                            level, score, lives, medium_font,
                            heart_full, heart_half, heart_empty,
                            menu_background, game_background);

            drawPauseScreen (renderer, overlay, state, big_font, medium_font, pause_controls, audio);
        }
        // Vykresleni Game End / Level Completed obrazovky
        else if (state.end)
        {
            if (state.play)
            {
                if (audio.play && state.win)
                {
                    SDL_ClearQueuedAudio(music.device);
                    SDL_QueueAudio(music.device, celebration.wav_buffer, celebration.wav_length);
                } 
                else if (audio.play && lives == 0)
                {
                    SDL_ClearQueuedAudio(music.device);
                    SDL_QueueAudio(music.device, fail.wav_buffer, fail.wav_length);
                } 
                state.play = false;
            }
            state.play = false;
            drawGameWindow (renderer, paddle, ball, blocks,
                            level, score, lives, medium_font,
                            heart_full, heart_half, heart_empty,
                            menu_background, game_background);

            SDL_Rect button_rect = {0, 0, 180, 50};
            for (int rows = 0; rows < 2; rows++)
            {
                for (int cols = 0; cols < 2; cols++)
                {
                    button_rect.y = HEIGHT_OFFSET + 300 + 10 + rows * (button_rect.h + 10);
                    button_rect.x = WIDTH_OFFSET + 15 + cols * (button_rect.w + 10);
                    
                    if (checkMouseHover(button_rect))
                        pause_controls.button_selected = cols + rows * 2;
                    
                    if (audio.play && pause_controls.button_selected != pause_controls.old_button_selected)
                    {
                        SDL_ClearQueuedAudio(audio.device);
                        SDL_QueueAudio(audio.device, scroll.wav_buffer, scroll.wav_length);
                    }
                    pause_controls.old_button_selected = pause_controls.button_selected;
                }
            }

            drawEndScreen (state.win, renderer, overlay, 
                            big_font, medium_font, 
                            score, highscores, &state, level, rainbow, rainbow_offset, pause_controls, audio);
        }
        // Vykresleni Hry
        else
        {
            state.play = true;
            drawGameWindow (renderer, paddle, ball, blocks,
                            level, score, lives, medium_font,
                            heart_full, heart_half, heart_empty,
                            menu_background, game_background);
        }

        // Zvetsovani rainbow_offset - na vykreslovani duhoveho textu
        running_counter++;
        if (running_counter % 12 == 0)
        {
            rainbow_offset = (rainbow_offset + 1) % BLOCK_ROWS;
            running_counter = 0;
        }

        // Zobrazeni vykreslenych prvku na obrazovku
        SDL_RenderPresent(renderer);
        SDL_Delay(3);
    }

    // Uvolneni
    TTF_CloseFont(big_font);
    TTF_CloseFont(medium_font);
    TTF_CloseFont(small_font);
    TTF_Quit();

    SDL_CloseAudioDevice(audio.device);
    SDL_CloseAudioDevice(music.device);

    SDL_FreeWAV(bounce_paddle.wav_buffer);
    SDL_FreeWAV(bounce_wall.wav_buffer);
    SDL_FreeWAV(bounce_brick.wav_buffer);
    SDL_FreeWAV(scroll.wav_buffer);
    SDL_FreeWAV(select.wav_buffer);
    SDL_FreeWAV(life_lost.wav_buffer);
    SDL_FreeWAV(celebration.wav_buffer);
    SDL_FreeWAV(fail.wav_buffer);

    SDL_DestroyTexture(heart_full);
    SDL_DestroyTexture(heart_half);
    SDL_DestroyTexture(heart_empty);
    SDL_DestroyTexture(overlay);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}