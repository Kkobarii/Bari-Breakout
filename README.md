# Bari Breakout

Project for my UPR course

## Description

A Brick Breaker clone written in C with visuals built with SDL. This is the first programming project I ever made, so I'm putting it here just for completion's sake.

The game includes:
- Basic Breakout gameplay
- Functional level selection & pause menu screens
- Controlled by designated keys, arrows or even mouse!
- Sounds (mutable)
- Highscores stored in a very private .txt file
- All the rainbows your heart desires

## How to compile

### Linux

- Install all necessary prerequisites: 
    - `gcc`
    - `make`
- Install SDL2 libraries with
    >`sudo apt-get install libsdl2-dev libsdl2-ttf-dev libsdl2-image-dev`
- Unzip archive with sources into a folder
- Build with `make`

### Windows

- Install all necessary prerequisites: 
    - `mingw32`
    - `gcc`
    - `SDL2`
    - `make`
- Unzip archive with sources into a folder
- Make sure you have all  `.dll` files accessible for the compiler:
    - `SDL2.dll`
    - `SDL2_image.dll`
    - `SDL2_ttf.dll`
    - `libfreetype-6.dll`
    - `libpng16-16.dll`
    - `zlib1.dll`
- Build with `make`

Make sure to check `Makefile` for more options!

## How to use

### Menu

- 1-8: start level (in the boring way)
- Arrow keys: select level
- ENTER or E: start selected level

### Game Controls

- AD or arrows: move paddle
- ESCAPE: pause the game

#### *Super secret cheats*

- *Q: paddle cheat*
- *C: instant win*
- *X: instant death*

### Pause Screen & Win / Game Over Screen

- ESCAPE: unpause the game (pause screen only)
- R or 1: restart level
- M or 2: return to menu
- N or 3: mute/unmute sounds
- B or 4: quit the game
- Arrow keys: select option
- ENTER or E: confirm selected option

You can also navigate all the menus with your **mouse**!

> Originally made in November 2021