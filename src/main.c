#include <SDL2/SDL.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

#define SDL_ERR(ERR_MSG) 	do \
							{ \
								char buf[256]; \
								snprintf(buf, sizeof buf, "%s%s", ERR_MSG, " | SDL_Error: %s\n"); \
								fprintf(stderr, buf, SDL_GetError()); \
							} while(0)

SDL_Window *gWindow = NULL;
SDL_Surface *gScreenSurface = NULL;
SDL_Surface *gHelloWorld = NULL;

bool init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		SDL_ERR("SDL could not init!");
		return false;
	}
	else
	{
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			SDL_ERR("Window could not be created!");
			return false;
		}
		else
		{
			gScreenSurface = SDL_GetWindowSurface(gWindow);
		}
	}

	return true;
}

bool loadMedia()
{
	gHelloWorld = SDL_LoadBMP("hello-world.bmp");
	if (gHelloWorld == NULL)
	{
		SDL_ERR("Unable to load image!");
		return false;
	}
	return true;
}

void QuitSDL()
{
	SDL_FreeSurface(gHelloWorld);
	gHelloWorld = NULL;
	
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	SDL_Quit();
}

int main(int argc, char* args[])
{
	if (!init()) return -1;

	if (!loadMedia()) return -1;

	bool quit = false;
	SDL_Event e;

	while (!quit)
	{
		while( SDL_PollEvent( &e ))
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
		}

		SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, NULL);

		SDL_UpdateWindowSurface(gWindow);
	}

	QuitSDL();
	
	return 0;
};