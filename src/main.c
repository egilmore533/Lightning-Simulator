#include <stdio.h>
#include <stdlib.h>

#include "simple_logger.h"

#include "graphics.h"
#include "lightning.h"
#include "sprite.h"

void init_all_systems();


int main(int argc, char *argv[])
{
	int done = 0;
	int x, y;
	const Uint8 *keys = NULL;
	SDL_Renderer *the_renderer;
	Lightning *lightning = NULL;
	SDL_Point *center = NULL;
	Sprite *test = NULL;

	init_all_systems();

	center = (SDL_Point *) malloc(sizeof(SDL_Point));
	memset(center, 0, sizeof(SDL_Point));
	

	the_renderer = graphics_get_renderer();

	lightning = lightning_new(vect2d_new(100, 100), vect2d_new(200, 300), 16, 90);
	//test = sprite_load("images/test.jpg", vect2d_new(960, 720), 1, 1);
	do
	{
		SDL_RenderClear(the_renderer);

		center->x = 0;
		center->y = 0;
		lightning_draw(lightning);
		sprite_draw(test, 1, vect2d_new(200, 300), vect2d_new(1, 1), center, 0, SDL_FLIP_NONE);

		SDL_GetMouseState(&x, &y);
		printf("Mouse %d, %d\n", x, y);

		graphics_next_frame();
		SDL_PumpEvents();

		keys = SDL_GetKeyboardState(NULL);
		if(keys[SDL_SCANCODE_ESCAPE])
		{
			done = 1;
		}

	}while(!done);

	slog("\n\n ============== QUIT ===================\n\n");
	exit(0);
	return 0;
}

void init_all_systems()
{
	init_logger("log.txt"); //init simple logger from DJ's source code
	slog("\n\n ============= START ====================\n\n");

	graphics_init("Lightning Simulator", vect2d_new(WINDOW_WIDTH, WINDOW_HEIGHT), vect2d_new(WINDOW_WIDTH, WINDOW_HEIGHT), 0);
	slog("\n\n ============= GRAPHICS START ====================\n\n");

	sprite_init_system(100);
	slog("\n\n ============= SPRITE START ====================\n\n");

	lightning_init_system(100);
	slog("\n\n ============= LIGHTNING START ====================\n\n");
}