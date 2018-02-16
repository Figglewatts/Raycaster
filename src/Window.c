#include "Window.h"

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