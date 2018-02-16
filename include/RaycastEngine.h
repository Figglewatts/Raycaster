#ifndef RAYCAST_ENGINE_H
#define RAYCAST_ENGINE_H

#define mapWidth 24
#define mapHeight 24

#include "Player.h"
#include "Window.h"

struct Player;

extern int worldMap[mapWidth][mapHeight];

void draw_line(int x, int y1, int y2, SDL_Color color, SDL_Renderer* renderer);

void engine_raycast_player_view(Player* player, SDLWindow* window);

bool engine_check_world_collision(int x, int y);

#endif // RAYCAST_ENGINE_H