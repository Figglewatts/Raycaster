#ifndef RAYCAST_ENGINE_H
#define RAYCAST_ENGINE_H

#define mapWidth 24
#define mapHeight 24

#define texWidth 64
#define texHeight 64

#define MAX_TEXTURES 8

#define FOG_INCREMENT 2
#define MAX_FOG_DIST 20
#define MIN_FOG_DIST 3
#define FOG_COLOR 0x0#

#include "Player.h"
#include "Window.h"

typedef struct Scanline
{
	int start;
	int end;
	int height;
	int texX;
	int texNum;
	int side;
	int x;
	float distance;
} Scanline;

static const Scanline EmptyScanline;

struct Player;

extern int worldMap[mapWidth][mapHeight];
extern uint32_t textures[MAX_TEXTURES][texWidth * texHeight];

void draw_line(int x, int y1, int y2, SDL_Color color, SDL_Renderer* renderer);

void draw_scanline(uint32_t* framebuffer, int windowWidth, int windowHeight, Scanline* scanline);

void engine_raycast_player_view(Player* player, SDLWindow* window, uint32_t* framebuffer);

void set_texture(int texNum, unsigned char* image, int width, int height);

bool engine_check_world_collision(int x, int y);

#endif // RAYCAST_ENGINE_H