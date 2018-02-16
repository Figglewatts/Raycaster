#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>

#include <stdbool.h>

#define SDL_ERR(ERR_MSG)    do                                          \
                            {                                           \
                                char buf[256];                          \
                                snprintf(buf, sizeof buf, "%s%s",       \
                                ERR_MSG, " | SDL_Error: %s\n");         \
                                fprintf(stderr, buf, SDL_GetError());   \
                            } while(0)

typedef struct SDLWindow
{
    SDL_Window *window;
    SDL_Surface *windowSurface;
    SDL_Renderer *renderer;
    const char* title;
    const int width, height;
} SDLWindow;

bool init_window(SDLWindow *window);

bool sdl_init();

void sdl_destroy(SDLWindow *window);

#endif // WINDOW_H