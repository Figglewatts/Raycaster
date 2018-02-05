#include <SDL2/SDL.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "Window.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool load_media(SDL_Surface **media, const char* path)
{
    *media = SDL_LoadBMP(path);
    if (*media == NULL)
    {
        SDL_ERR("Unable to load image!");
        return false;
    }
    return true;
}

int main(int argc, char** argv)
{
    SDLWindow window = {.title="Raycaster", .width=SCREEN_WIDTH, 
        .height=SCREEN_HEIGHT};
    SDL_Surface *hello_world = NULL;
    
    if (!sdl_init()) return -1;

    if (!init_window(&window)) return -1;

    if (!load_media(&hello_world, "assets/hello-world.bmp")) return -1;

    bool quit = false;
    SDL_Event e;

    while (!quit)
    {
        while( SDL_PollEvent( &e ))
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        SDL_BlitSurface(hello_world, NULL, window.windowSurface, NULL);

        SDL_UpdateWindowSurface(window.window);
    }

    SDL_FreeSurface(hello_world);
    sdl_destroy(&window);
    
    return 0;
};