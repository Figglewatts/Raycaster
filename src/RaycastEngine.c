#include "RaycastEngine.h"

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "Player.h"
#include "Window.h"
#include <stdio.h>

int worldMap[mapWidth][mapHeight]=
{
  {8,8,8,8,8,8,8,8,8,8,8,4,4,6,4,4,6,4,6,4,4,4,6,4},
  {8,0,0,0,0,0,0,0,0,0,8,4,0,0,0,0,0,0,0,0,0,0,0,4},
  {8,0,3,3,0,0,0,0,0,8,8,4,0,0,0,0,0,0,0,0,0,0,0,6},
  {8,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6},
  {8,0,3,3,0,0,0,0,0,8,8,4,0,0,0,0,0,0,0,0,0,0,0,4},
  {8,0,0,0,0,0,0,0,0,0,8,4,0,0,0,0,0,6,6,6,0,6,4,6},
  {8,8,8,8,0,8,8,8,8,8,8,4,4,4,4,4,4,6,0,0,0,0,0,6},
  {7,7,7,7,0,7,7,7,7,0,8,0,8,0,8,0,8,4,0,4,0,6,0,6},
  {7,7,0,0,0,0,0,0,7,8,0,8,0,8,0,8,8,6,0,0,0,0,0,6},
  {7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,6,0,0,0,0,0,4},
  {7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,6,0,6,0,6,0,6},
  {7,7,0,0,0,0,0,0,7,8,0,8,0,8,0,8,8,6,4,6,0,6,6,6},
  {7,7,7,7,0,7,7,7,7,8,8,4,0,6,8,4,8,3,3,3,0,3,3,3},
  {2,2,2,2,0,2,2,2,2,4,6,4,0,0,6,0,6,3,0,0,0,0,0,3},
  {2,2,0,0,0,0,0,2,2,4,0,0,0,0,0,0,4,3,0,0,0,0,0,3},
  {2,0,0,0,0,0,0,0,2,4,0,0,0,0,0,0,4,3,0,0,0,0,0,3},
  {1,0,0,0,0,0,0,0,1,4,4,4,4,4,6,0,6,3,3,0,0,0,3,3},
  {2,0,0,0,0,0,0,0,2,2,2,1,2,2,2,6,6,0,0,5,0,5,0,5},
  {2,2,0,0,0,0,0,2,2,2,0,0,0,2,2,0,5,0,5,0,0,0,5,5},
  {2,0,0,0,0,0,0,0,2,0,0,0,0,0,2,5,0,5,0,5,0,5,0,5},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5},
  {2,0,0,0,0,0,0,0,2,0,0,0,0,0,2,5,0,5,0,5,0,5,0,5},
  {2,2,0,0,0,0,0,2,2,2,0,0,0,2,2,0,5,0,5,0,0,0,5,5},
  {2,2,2,2,1,2,2,2,2,2,2,1,2,2,2,5,5,5,5,5,5,5,5,5}
};

uint32_t textures[MAX_TEXTURES][texWidth * texHeight] = {};

void draw_line(int x, int y1, int y2, SDL_Color color, SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(renderer, x, y1, x, y2);
}

void shade_pixel(uint32_t *pixel, uint32_t shadeColor, float shadeFactor)
{
    int blue = (*pixel & 0xFF);
    int green = (*pixel & 0xFF00) >> 8;
    int red = (*pixel & 0xFF0000) >> 16;

    int shadeBlue = (shadeColor & 0xFF);
    int shadeGreen = (shadeColor & 0xFF00) >> 8;
    int shadeRed = (shadeColor & 0xFF0000) >> 16;

    red = red + (shadeRed - red) * shadeFactor;
    green = green + (shadeGreen - green) * shadeFactor;
    blue = blue + (shadeBlue - blue) * shadeFactor;

    *pixel = (red << 16) + (green << 8) + (blue);
}

void apply_fog(uint32_t *pixelcol, float distance)
{
    if (distance < MIN_FOG_DIST)
    {
        return;
    }
    else if (distance > MAX_FOG_DIST)
    {
        *pixelcol = FOG_COLOR;
    }
    
    for (int i = MIN_FOG_DIST; i < MAX_FOG_DIST; i+=FOG_INCREMENT)
    {
        if (distance >= i && distance < i + FOG_INCREMENT)
        {
            float shadeFactor = (float)(i - MIN_FOG_DIST) / (float)(MAX_FOG_DIST - MIN_FOG_DIST);
            shade_pixel(pixelcol, FOG_COLOR, shadeFactor);
            break;
        }
    }
}

void draw_scanline(uint32_t* framebuffer, int windowWidth, int windowHeight, Scanline* scanline)
{
    for (int y = scanline->start; y < scanline->end; y++)
    {
        long d = y * 256 - windowHeight * 128 + scanline->height * 128;
        int texY = ((d * texHeight) / scanline->height) / 256;
        uint32_t color = textures[scanline->texNum][texHeight * texY + scanline->texX];
        apply_fog(&color, scanline->distance);
        // make color darker for y-sides, R,G,B bytes div by 2 with shift and AND
        if (scanline->side == 1) color = (color >> 1) & 8355711;
        framebuffer[y * windowWidth + scanline->x] = color;
    }
}

void draw_floor_ceiling(uint32_t* framebuffer, int windowWidth, int windowHeight, Scanline* scanline, FloorCast* floorCast, Player* player)
{
    // draw the floor from scanline end to bottom of screen
    for (int y = scanline->end + 1; y < windowHeight; y++)
    {
        // this calculation is an excellent candidate for a lookup table
        floorCast->currentDist = windowHeight / (2.0 * y - windowHeight);

        float weight = (floorCast->currentDist - floorCast->distPlayer) 
            / (floorCast->distWall - floorCast->distPlayer);

        float currentFloorX = weight * floorCast->floorXWall + (1.0 - weight) * player->posX;
        float currentFloorY = weight * floorCast->floorYWall + (1.0 - weight) * player->posY;

        int floorTexX, floorTexY;
        floorTexX = (int)(currentFloorX * texWidth) % texWidth;
        floorTexY = (int)(currentFloorY * texHeight) % texHeight;

        uint32_t floor_pixel = textures[FLOOR_TEXTURE][texWidth * floorTexY + floorTexX];
        uint32_t ceil_pixel = (textures[CEILING_TEXTURE][texWidth * floorTexY + floorTexX] >> 1) & 8355711;

        float dist = scanline->distance * weight;
        apply_fog(&floor_pixel, dist);
        apply_fog(&ceil_pixel, dist);

        framebuffer[y * windowWidth + scanline->x] = floor_pixel;
        framebuffer[(windowHeight - y) * windowWidth + scanline->x] = ceil_pixel;
    }
}

void engine_raycast_player_view(Player* player, SDLWindow* window, uint32_t *framebuffer)
{
	Scanline currentScanline;
    FloorCast currentFloorCast;
    for (int x = 0; x < window->width; x++)
    {
        currentScanline = EmptyScanline;
        currentScanline.x = x;
        
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

        currentScanline.side = side;

        // calculate distance projected on camera direction
        if (side == 0)
        {
            perpWallDist = (mapX - player->posX + (1 - stepX) / 2) / rayDirX;
        }
        else
        {
            perpWallDist = (mapY - player->posY + (1 - stepY) / 2) / rayDirY;
        }

        currentScanline.distance = perpWallDist;

        // calculate height of line to draw on screen
        currentScanline.height = (int)(window->height / perpWallDist);

        // calculate lowest and highest pixel to fill in current stripe
        currentScanline.start = -currentScanline.height / 2 + window->height / 2;
        if (currentScanline.start < 0) currentScanline.start = 0;
        currentScanline.end = currentScanline.height / 2 + window->height / 2;
        if (currentScanline.end >= window->height) currentScanline.end = window->height - 1;

        currentScanline.texNum = worldMap[mapX][mapY] - 1;

        // calculate value of wallX
        double wallX; // where exactly the wall was hit
        if (side == 0) wallX = player->posY + perpWallDist * rayDirY;
        else           wallX = player->posX + perpWallDist * rayDirX;
        wallX -= floor(wallX);

        // x coord on the texture
        currentScanline.texX = (int)(wallX * (double)texWidth);
        if (side == 0 && rayDirX > 0) currentScanline.texX = texWidth - currentScanline.texX - 1;
        if (side == 1 && rayDirY < 0) currentScanline.texX = texWidth - currentScanline.texX - 1;

        draw_scanline(framebuffer, window->width, window->height, &currentScanline);

        // begin floor casting
        currentFloorCast = EmptyFloorCast;

        // 4 different dirs possible
        if (side == 0 && rayDirX > 0)
        {
            currentFloorCast.floorXWall = mapX;
            currentFloorCast.floorYWall = mapY + wallX;
        }
        else if (side == 0 && rayDirX < 0)
        {
            currentFloorCast.floorXWall = mapX + 1.0;
            currentFloorCast.floorYWall = mapY + wallX;
        }
        else if (side == 1 && rayDirY > 0)
        {
            currentFloorCast.floorXWall = mapX + wallX;
            currentFloorCast.floorYWall = mapY;
        }
        else
        {
            currentFloorCast.floorXWall = mapX + wallX;
            currentFloorCast.floorYWall = mapY + 1.0;
        }

        currentFloorCast.currentDist = 0.0;
        currentFloorCast.distWall = perpWallDist;
        currentFloorCast.distPlayer = 0.0;

        // becomes < 0 on int overflow
        if (currentScanline.end < 0) currentScanline.end = window->height; 

        draw_floor_ceiling(framebuffer, window->width, window->height, &currentScanline, &currentFloorCast, player);
    }
}

bool engine_check_world_collision(int x, int y)
{
	return worldMap[x][y] > 0;
}

void set_texture(int texNum, unsigned char* image, int width, int height)
{
    for (int i = 0; i < width * height * 4; i+=4)
    {
        uint32_t color = 0;
        color += image[i+2];
        color += (image[i+1] << 8);
        color += (image[i] << 16);
        color += 0xFF000000;
        textures[texNum][i/4] = color;
    }
}