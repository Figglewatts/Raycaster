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
    SDL_Renderer *renderer;
    const char* title;
    const int width, height;
} SDLWindow;

bool init_window(SDLWindow *window)
{
    SDL_CreateWindowAndRenderer(window->width, window->height,
        0, &window->window, &window->renderer);
    SDL_SetWindowTitle(window->window, window->title);
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
    SDL_DestroyRenderer(window->renderer);
    window->renderer = NULL;
    
    SDL_DestroyWindow(window->window);
    window->window = NULL;

    SDL_Quit();
}

#endif // WINDOW_H