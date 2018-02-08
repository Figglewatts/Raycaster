#ifndef RAYCAST_ENGINE_H
#define RAYCAST_ENGINE_H

#include "Player.h"
#include "Window.h"
#include "f16.h"

#define mapWidth 24
#define mapHeight 24

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

void engine_raycast_player_view(Player* player, SDLWindow* window)
{
	    // perform raycasting
    for (int x = 0; x < window->width; x++)
    {
        // calculate position and dir
		F16 twoX = create_f16(2 * x, 0);

		// camX is screen space coord of ray, between -1 and 1
		F16 camX = div_f16(twoX, create_f16(window->width - 1, 0));
		Vec2 planeVec = create_vec2i(
			mul_f16(player->plane.x, camX),
			mul_f16(player->plane.y, camX)
		);
		Vec2 rayDir = create_vec2i(
			add_f16(player->dir.x, planeVec.x),
			add_f16(player->dir.y, planeVec.y)
		);

        // which box of map we're in
        I16 mapX = mag_f16(player->pos.x);
        I16 mapY = mag_f16(player->pos.y);

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

#endif // RAYCAST_ENGINE_H