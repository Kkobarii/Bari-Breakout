#ifndef DEFINITIONS
    #define DEFINITIONS

    #define UNUSED(x) (void)(x)

    #define WINDOW_WIDTH 440
    #define WINDOW_HEIGHT 580
    #define GAME_WIDTH 400
    #define GAME_HEIGHT 500
    #define WIDTH_OFFSET 20
    #define HEIGHT_OFFSET 80
    #define BLOCK_ROWS_OFFSET 60

    #define MAX_SPEED 4
    #define MAX_LIVES 4
    #define HEART_SIZE 40

    #define BLOCK_WIDTH 25
    #define BLOCK_HEIGHT 15
    #define BLOCK_ROWS 8
    #define BLOCK_COLS 16

    #define PADDLE_WIDTH 80
    #define PADDLE_HEIGHT 8

    #define GAME_BACKGROUND_COLOR 40
    #define MENU_BACKGROUND_COLOR 30

    #define NUMBER_OF_LEVELS 8

    #define DEFAULT_TEXT_LENGTH 64

    typedef struct {
        int movement_horizontal; // 1 = right, -1 = left
        int movement_vertical;   // 1 = down, -1 = up
        double speed;
        int x;
        int y;
        int radius;
    } Ball;

    typedef struct {
        int x1;
        int y1;
        int x2;
        int y2;
    } Paddle;

    typedef struct {
        char status[BLOCK_ROWS][BLOCK_COLS];
        int level;
    } Positions;

    typedef struct {
        // Levy horni roh
        int x1;
        int y1;
        // Pravy dolni roh
        int x2;
        int y2;
        // Barva
        SDL_Color color;
        // Zbytek
        int points;
        bool alive;
    } Block;

    typedef struct {
        bool right;
        bool left;
        bool button_pressed;
        int button_selected;
        int old_button_selected;
    } Menu_Controls;

    typedef struct {
        bool right;
        bool left;
        bool paddle_cheat;
        bool win_cheat;
        bool death_cheat;
        bool pause_switch;
    } Game_Controls;

    typedef struct {
        bool quit;
        bool win;
        bool highscore;
        bool play;
        bool menu;
        bool pause;
        bool end;
    } Game_State;

    typedef struct {
        SDL_AudioSpec wav_spec;
        Uint32 wav_length;
        Uint8 * wav_buffer;
    } Wav;
    
    typedef struct {
        SDL_AudioDeviceID device;
        bool play;
    } Audio;

#endif