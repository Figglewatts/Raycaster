#include <SDL2/SDL.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#include "Window.h"

#define FLOAT_ARITHMETIC
#include "f16.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const double MOVE_SPEED = 50.0;
const double ROT_SPEED = 50.0;

#define mapWidth 24
#define mapHeight 24

int worldMap[mapWidth][mapHeight]=
{
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

typedef struct Player {
    double posX, posY;
    double dirX, dirY;
    double planeX, planeY;
} Player;

Player gPlayer;

void drawLine(int x, int y1, int y2, SDL_Color color, SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(renderer, x, y1, x, y2);
}

void update(SDLWindow *window)
{
    // perform raycasting
    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
        // calculate position and dir
        double camX = 2 * x / (double)SCREEN_WIDTH - 1; // x coord, cam space
        double rayDirX = gPlayer.dirX + gPlayer.planeX * camX;
        double rayDirY = gPlayer.dirY + gPlayer.planeY * camX;

        // which box of map we're in
        int mapX = (int)gPlayer.posX;
        int mapY = (int)gPlayer.posY;

        // length of ray from curr pos to next X or Y square boundary
        double sideDistX, sideDistY;

        // length of ray from one x or y boundary to next
        double deltaDistX = fabs(1 / rayDirX);
        double deltaDistY = fabs(1 / rayDirY);
        double perpWallDist;

        // in what dir to step in x or y (+1 or -1)
        int stepX, stepY;

        bool hit = false; // was there a wall hit?
        int side; // which side of the wall did we hit?

        // calculate step and initial sideDist
        if (rayDirX < 0)
        {
            stepX = -1;
            sideDistX = (gPlayer.posX - mapX) * deltaDistX;
        }
        else
        {
            stepX = 1;
            sideDistX = (mapX + 1.0 - gPlayer.posX) * deltaDistX;
        }
        if (rayDirY < 0)
        {
            stepY = -1;
            sideDistY = (gPlayer.posY - mapY) * deltaDistY;
        }
        else
        {
            stepY = 1;
            sideDistY = (mapY + 1.0 - gPlayer.posY) * deltaDistY;
        }

        // perform DDA
        while (!hit)
        {
            // jump to next map square
            if (sideDistX < sideDistY)
            {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            }
            else
            {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }

            // check if ray has hit a wall
            if (worldMap[mapX][mapY] > 0) hit = true;
        }

        // calculate distance projected on camera direction
        if (side == 0)
        {
            perpWallDist = (mapX - gPlayer.posX + (1 - stepX) / 2) / rayDirX;
        }
        else
        {
            perpWallDist = (mapY - gPlayer.posY + (1 - stepY) / 2) / rayDirY;
        }

        // calculate height of line to draw on screen
        int lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);

        // calculate lowest and highest pixel to fill in current stripe
        int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
        if (drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
        if (drawEnd >= SCREEN_HEIGHT) drawEnd = SCREEN_HEIGHT - 1;

        // choose wall color
        SDL_Color wallCol = {0, 0, 0, 255};
        switch (worldMap[mapX][mapY])
        {
            case 1: wallCol.r = 255; break;
            case 2: wallCol.g = 255; break;
            case 3: wallCol.b = 255; break;
            case 4: wallCol.r = wallCol.g = wallCol.b = 255; break;
            default: wallCol.r = wallCol.g = 255; break;
        }

        // give x and y sides diff brightness
        if (side == 1)
        {
            wallCol.r = wallCol.r / 2;
            wallCol.g = wallCol.g / 2;
            wallCol.b = wallCol.b / 2;
        }

        drawLine(x, drawStart, drawEnd, wallCol, window->renderer);
    }
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

    F16 test = create_f16(-5, 0);
    F16 test2 = create_f16(2, 0);
    F16 res = div_f16(test, test2);

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