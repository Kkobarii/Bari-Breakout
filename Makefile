all:

ifeq ($(OS),Windows_NT)
# Unrecognized environment detected :|
	gcc ./code/*.c -Isrc/include -Lsrc/lib -o main -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lm
else
# Operating system detected :)
	gcc ./code/*.c -Isrc/include -o main -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lm
endif

# Uncomment me if want to run the game as soon as you build it
#	./main