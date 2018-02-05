#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>

#define SDL_ERR(ERR_MSG)    do                                          \
                            {                                           \
                                char buf[256];                          \
                                snprintf(buf, sizeof buf, "%s%s",       \
                                ERR_MSG, " | SDL_Error: %s\n");         \
                                fprintf(stderr, buf, SDL_GetError());   \
                            } while(0)

typedef struct 
{
    SDL_Window *window;
    SDL_Surface *windowSurface;
    const char* title;
    const int width, height;
} SDLWindow;

bool init_window(SDLWindow *window)
{
    window->window = SDL_CreateWindow(window->title,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        window->width, window->height,
        SDL_WINDOW_SHOWN);
    if (window->window == NULL)
    {
        SDL_ERR("Window could not be created!");
        return false;
    }
    window->windowSurface = SDL_GetWindowSurface(window->window);
    return true;
}

bool sdl_init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_ERR("SDL could not init!");
        return false;
    }
    return true;
}

void sdl_destroy(SDLWindow *window)
{
    SDL_DestroyWindow(window->window);
    window->window = NULL;

    SDL_Quit();
}

#endif // WINDOW_H