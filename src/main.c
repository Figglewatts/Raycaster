#include <SDL2/SDL.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#include "Window.h"
#include "Player.h"

#define FLOAT_ARITHMETIC
#include "f16.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const double MOVE_SPEED = 50.0;
const double ROT_SPEED = 50.0;

Player gPlayer;

void drawLine(int x, int y1, int y2, SDL_Color color, SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(renderer, x, y1, x, y2);
}

void update(SDLWindow *window)
{

}

void handleInput(SDL_Keycode keycode, double dt)
{
    int worldPosX, worldPosY;
    switch (keycode)
    {
        case SDLK_UP:
        {
            worldPosX = (int)(gPlayer.posX + gPlayer.dirX * MOVE_SPEED * dt);
            worldPosY = (int)(gPlayer.posY);
            if (worldMap[worldPosX][worldPosY] == false) 
            {
                gPlayer.posX += gPlayer.dirX * MOVE_SPEED * dt;
            }

            worldPosX = (int)(gPlayer.posX);
            worldPosY = (int)(gPlayer.posY + gPlayer.dirY * MOVE_SPEED * dt);
            if (worldMap[worldPosX][worldPosY] == false)
            {
                gPlayer.posY += gPlayer.dirY * MOVE_SPEED * dt;
            }
        } break;
        case SDLK_DOWN:
        {
            worldPosX = (int)(gPlayer.posX - gPlayer.dirX * MOVE_SPEED * dt);
            worldPosY = (int)(gPlayer.posY);
            if (worldMap[worldPosX][worldPosY] == false) 
            {
                gPlayer.posX -= gPlayer.dirX * MOVE_SPEED * dt;
            }

            worldPosX = (int)(gPlayer.posX);
            worldPosY = (int)(gPlayer.posY - gPlayer.dirY * MOVE_SPEED * dt);
            if (worldMap[worldPosX][worldPosY] == false)
            {
                gPlayer.posY -= gPlayer.dirY * MOVE_SPEED * dt;
            }
        } break;
        case SDLK_LEFT:
        {
            double oldDirX = gPlayer.dirX;
            gPlayer.dirX = gPlayer.dirX * cos(ROT_SPEED * dt) 
                         - gPlayer.dirY * sin(ROT_SPEED * dt);
            gPlayer.dirY = oldDirX * sin(ROT_SPEED * dt) 
                         + gPlayer.dirY * cos(ROT_SPEED * dt);
            double oldPlaneX = gPlayer.planeX;
            gPlayer.planeX = gPlayer.planeX * cos(ROT_SPEED * dt)
                         - gPlayer.planeY * sin(ROT_SPEED * dt);
            gPlayer.planeY = oldPlaneX * sin(ROT_SPEED * dt)
                         + gPlayer.planeY * cos(ROT_SPEED * dt);
        } break;
        case SDLK_RIGHT:
        {
            double oldDirX = gPlayer.dirX;
            gPlayer.dirX = gPlayer.dirX * cos(-ROT_SPEED * dt) 
                         - gPlayer.dirY * sin(-ROT_SPEED * dt);
            gPlayer.dirY = oldDirX * sin(-ROT_SPEED * dt) 
                         + gPlayer.dirY * cos(-ROT_SPEED * dt);
            double oldPlaneX = gPlayer.planeX;
            gPlayer.planeX = gPlayer.planeX * cos(-ROT_SPEED * dt)
                         - gPlayer.planeY * sin(-ROT_SPEED * dt);
            gPlayer.planeY = oldPlaneX * sin(-ROT_SPEED * dt)
                         + gPlayer.planeY * cos(-ROT_SPEED * dt);
        } break;
    }
}

void print_f16(F16 f)
{
    double fl = float_f16(f);
    printf("%f\n", float_f16(f));
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

    F16 test = create_f16f(-5);
    F16 test2 = create_f16f(2.5);
    F16 res = mul_f16(test, test2);

    print_f16(test);
    print_f16(test2);
    print_f16(res);

    while (!quit)
    {
        SDL_SetRenderDrawColor(window.renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(window.renderer);

        oldTime = t;
        t = clock();
        double frameTime = ((double)t - (double)oldTime) / CLOCKS_PER_SEC;

        while( SDL_PollEvent( &e ))
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                handleInput(e.key.keysym.sym, frameTime);
            }
        }

        update(&window);

        SDL_RenderPresent(window.renderer);
    }

    sdl_destroy(&window);
    
    return 0;
};