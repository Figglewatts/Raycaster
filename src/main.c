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

#define FLOAT_ARITHMETIC
#include "f16.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const double MOVE_SPEED = 2.0;
const double ROT_SPEED = 3.0;

Player gPlayer;

KeyboardState currentKeyboardState;
KeyboardState lastKeyboardState;

void update(SDLWindow *window, double dt)
{
    engine_raycast_player_view(&gPlayer, window);
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

    while (!quit)
    {
        SDL_SetRenderDrawColor(window.renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(window.renderer);

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

        lastKeyboardState = currentKeyboardState;

        SDL_RenderPresent(window.renderer);
    }

    sdl_destroy(&window);
    
    return 0;
};