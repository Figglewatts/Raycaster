#include "Player.h"
#include "RaycastEngine.h"

void rotate_player(float rad, Player* player)
{
	double oldDirX = player->dirX;
	player->dirX = player->dirX * cos(rad) 
					- player->dirY * sin(rad);
	player->dirY = oldDirX * sin(rad) 
					+ player->dirY * cos(rad);
	double oldPlaneX = player->planeX;
	player->planeX = player->planeX * cos(rad)
					- player->planeY * sin(rad);
	player->planeY = oldPlaneX * sin(rad)
					+ player->planeY * cos(rad);
}

void move_player(float x, float y, Player* player)
{
	int worldPosX = (int)(player->posX + x);
	int worldPosY = (int)(player->posY);

	if (!engine_check_world_collision(worldPosX, worldPosY))
		player->posX += x;

	worldPosX = (int)(player->posX);
	worldPosY = (int)(player->posY + y);

	if (!engine_check_world_collision(worldPosX, worldPosY))
		player->posY += y;
}