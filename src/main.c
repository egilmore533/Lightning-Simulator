#include "simple_logger.h"

#include "graphics.h"
#include "sprite.h"

void init_all_systems();


int main(int argc, char *argv[])
{
	int done = 0;
	const Uint8 *keys = NULL;
	SDL_Renderer *the_renderer;
	Sprite *lightning = NULL;
	SDL_Point *center = NULL;

	init_all_systems();

	center = (SDL_Point *) malloc(sizeof(SDL_Point));
	memset(center, 0, sizeof(SDL_Point));
	

	the_renderer = graphics_get_renderer();

	lightning = sprite_load("images/10390930_940528392639550_6986969352470416642_n.jpg", vect2d_new(1, 18), 1, 1);
	do
	{
		SDL_RenderClear(the_renderer);

		center->x = lightning->frameSize.x / 2;
		center->y = lightning->frameSize.y / 2;
		sprite_draw(lightning, 1, vect2d_new(100, 100), vect2d_new(120,1), center, 0, SDL_FLIP_NONE);

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
}