#include <SDL2/SDL.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

#define SDL_ERR(ERR_MSG)    do                                          \
                            {                                           \
                                char buf[256];                          \
                                snprintf(buf, sizeof buf, "%s%s",       \
                                ERR_MSG, " | SDL_Error: %s\n");         \
                                fprintf(stderr, buf, SDL_GetError());   \
                            } while(0)

bool sdl_init(SDL_Window **window, SDL_Surface **surface)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_ERR("SDL could not init!");
        return false;
    }
    else
    {
        *window = SDL_CreateWindow("SDL Tutorial", 
            SDL_WINDOWPOS_UNDEFINED, 
            SDL_WINDOWPOS_UNDEFINED, 
            SCREEN_WIDTH, SCREEN_HEIGHT, 
            SDL_WINDOW_SHOWN);
        if (*window == NULL)
        {
            SDL_ERR("Window could not be created!");
            return false;
        }
        else
        {
            *surface = SDL_GetWindowSurface(*window);
        }
    }

    return true;
}

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

void sdl_destroy(SDL_Window **window, SDL_Surface **media)
{
    SDL_FreeSurface(*media);
    *media = NULL;
    
    SDL_DestroyWindow(*window);
    *window = NULL;

    SDL_Quit();
}

int main(int argc, char* args[])
{
    SDL_Window *window = NULL;
    SDL_Surface *surface = NULL;
    SDL_Surface *hello_world = NULL;
    
    if (!sdl_init(&window, &surface)) return -1;

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

        SDL_BlitSurface(hello_world, NULL, surface, NULL);

        SDL_UpdateWindowSurface(window);
    }

    sdl_destroy(&window, &hello_world);
    
    return 0;
};