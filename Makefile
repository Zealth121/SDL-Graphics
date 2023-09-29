CC=gcc
CFLAGS=`sdl2-config --libs --cflags` -ggdb3 -O0 --std=c99 -Wall -lSDL2_image -lm
HDRS=
SRCS=SDL_Window.c
EXEC=SDLwindow

sdlmake: SDL_Window.c
	$(CC) $(SRCS) $(CFLAGS) -o $(EXEC)