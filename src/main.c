#include <SDL2/SDL.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#include "Window.h"
#include "Player.h"
#include "PlayerOperations.h"
#include "RaycastEngine.h"
#include "Input.h"
#include "lodepng.h"

#define FLOAT_ARITHMETIC
#include "f16.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

const double MOVE_SPEED = 2.0;
const double ROT_SPEED = 3.0;

Player gPlayer;

KeyboardState currentKeyboardState;
KeyboardState lastKeyboardState;

uint32_t framebuffer[SCREEN_HEIGHT * SCREEN_WIDTH];

unsigned char* load_image(const char* filepath, 
    unsigned* width, unsigned* height)
{
    unsigned char* image;
    unsigned error;

    error = lodepng_decode32_file(&image, width, height, filepath);
    if(error)
    {
        printf("decoder error %u: %s\n", error, lodepng_error_text(error));
    }

    return image;
}

void clear_framebuffer(uint32_t* framebuffer)
{
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
    {
        framebuffer[i] = 0;
    }
}

void update(SDLWindow *window, double dt)
{
    engine_raycast_player_view(&gPlayer, window, framebuffer);
}

void handleInput(double dt)
{
    if (key_held(SDL_SCANCODE_UP, &currentKeyboardState))
    {
        move_player(gPlayer.dirX * MOVE_SPEED * dt,
                gPlayer.dirY * MOVE_SPEED * dt,
                &gPlayer);
    }
    else if (key_held(SDL_SCANCODE_DOWN, &currentKeyboardState))
    {
        move_player(-gPlayer.dirX * MOVE_SPEED * dt,
                -gPlayer.dirY * MOVE_SPEED * dt,
                &gPlayer);
    }

    if (key_held(SDL_SCANCODE_LEFT, &currentKeyboardState))
    {
        rotate_player(ROT_SPEED * dt, &gPlayer);
    }
    else if (key_held(SDL_SCANCODE_RIGHT, &currentKeyboardState))
    {
        rotate_player(-ROT_SPEED * dt, &gPlayer);
    }
}

int main(int argc, char** argv)
{
    SDLWindow window = {.title="Raycaster", .width=SCREEN_WIDTH, 
        .height=SCREEN_HEIGHT};
    
    if (!sdl_init()) return -1;

    if (!init_window(&window)) return -1;

    bool quit = false;
    SDL_Event e;
    clock_t t = 0;
    clock_t oldTime = 0;

    gPlayer.posX = 22;
    gPlayer.posY = 12;
    gPlayer.dirX = -1;
    gPlayer.dirY = 0;
    gPlayer.planeX = 0;
    gPlayer.planeY = 0.66;

    unsigned width, height;
    unsigned char* greystone = load_image("assets/greystone.png", &width, &height);
    unsigned char* bluestone = load_image("assets/bluestone.png", &width, &height);
    unsigned char* redbrick = load_image("assets/redbrick.png", &width, &height);
    unsigned char* wood = load_image("assets/wood.png", &width, &height);
    unsigned char* colorstone = load_image("assets/colorstone.png", &width, &height);
    unsigned char* mossy = load_image("assets/mossy.png", &width, &height);
    unsigned char* eagle = load_image("assets/eagle.png", &width, &height);
    unsigned char* purplestone = load_image("assets/purplestone.png", &width, &height);
    set_texture(0, greystone, width, height);
    set_texture(1, bluestone, width, height);
    set_texture(2, redbrick, width, height);
    set_texture(3, wood, width, height);
    set_texture(4, colorstone, width, height);
    set_texture(5, mossy, width, height);
    set_texture(6, eagle, width, height);
    set_texture(7, purplestone, width, height);

    SDL_Texture *screen = SDL_CreateTexture(window.renderer, SDL_PIXELFORMAT_ARGB8888, 
        SDL_TEXTUREACCESS_STREAMING, window.width, window.height);

    while (!quit)
    {
        SDL_SetRenderDrawColor(window.renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        clear_framebuffer(framebuffer);

        oldTime = t;
        t = clock();
        double frameTime = ((double)t - (double)oldTime) / CLOCKS_PER_SEC;

        while ( SDL_PollEvent( &e ))
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        get_key_state(&currentKeyboardState);

        handleInput(frameTime);

        update(&window, frameTime);

        SDL_UpdateTexture(screen, NULL, framebuffer, window.width * sizeof(Uint32));

        lastKeyboardState = currentKeyboardState;

        SDL_RenderClear(window.renderer);
        SDL_RenderCopy(window.renderer, screen, NULL, NULL);
        SDL_RenderPresent(window.renderer);
    }

    SDL_DestroyTexture(screen);
    sdl_destroy(&window);
    
    return 0;
};