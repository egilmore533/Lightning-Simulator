
#include "simple_logger.h"
#include "graphics.h"

void init_all_systems();


int main(int argc, char *argv[])
{
	int done = 0;
	const Uint8 *keys = NULL;
	SDL_Renderer *the_renderer;

	init_all_systems();

	the_renderer = graphics_get_renderer();
	do
	{
		SDL_RenderClear(the_renderer);

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
}