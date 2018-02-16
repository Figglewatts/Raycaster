#include "RaycastEngine.h"

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "Player.h"
#include "Window.h"

int worldMap[mapWidth][mapHeight] =
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

void draw_line(int x, int y1, int y2, SDL_Color color, SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(renderer, x, y1, x, y2);
}

void engine_raycast_player_view(Player* player, SDLWindow* window)
{
	    // perform raycasting
    for (int x = 0; x < window->width; x++)
    {
        // calculate position and dir
        double camX = 2 * x / (double)window->width - 1; // x coord, cam space
        double rayDirX = player->dirX + player->planeX * camX;
        double rayDirY = player->dirY + player->planeY * camX;

        // which box of map we're in
        int mapX = (int)player->posX;
        int mapY = (int)player->posY;

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
            sideDistX = (player->posX - mapX) * deltaDistX;
        }
        else
        {
            stepX = 1;
            sideDistX = (mapX + 1.0 - player->posX) * deltaDistX;
        }
        if (rayDirY < 0)
        {
            stepY = -1;
            sideDistY = (player->posY - mapY) * deltaDistY;
        }
        else
        {
            stepY = 1;
            sideDistY = (mapY + 1.0 - player->posY) * deltaDistY;
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
            perpWallDist = (mapX - player->posX + (1 - stepX) / 2) / rayDirX;
        }
        else
        {
            perpWallDist = (mapY - player->posY + (1 - stepY) / 2) / rayDirY;
        }

        // calculate height of line to draw on screen
        int lineHeight = (int)(window->height / perpWallDist);

        // calculate lowest and highest pixel to fill in current stripe
        int drawStart = -lineHeight / 2 + window->height / 2;
        if (drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2 + window->height / 2;
        if (drawEnd >= window->height) drawEnd = window->height - 1;

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

        draw_line(x, drawStart, drawEnd, wallCol, window->renderer);
    }
}

bool engine_check_world_collision(int x, int y)
{
	return worldMap[x][y] > 0;
}