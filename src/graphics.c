#include <stdio.h>

#include "simple_logger.h"
#include "graphics.h"

/* rendering pipeline data */
static SDL_Window			*graphicsMainWindow = NULL;
static SDL_Renderer			*graphicsRenderer = NULL;

/* timing */
static Uint32				graphicsFrameDelay = 30;
static Uint32				graphicsNow = 0;
static Uint32				graphicsThen = 0;
static Uint8				graphicsPrintFPS = 1;
static float				graphicsFPS = 0; 

/**
 * @brief	initializes the main window and the main renderer.
 * @param   [in]	windowName	If non-null, name of the window, will be displayed at the top of the window.
 * @param	viewSize		  	Width and Height of the view.
 * @param	renderSize		  	Width and Height of the render.
 * @param	fullscreen		  	flag to use fullscreen or not.
 */
void graphics_init(char *windowName, Vect2d viewSize, Vect2d renderSize, int fullscreen)
{
	Uint32 flags = 0;
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        slog("Unable to initilaize SDL system: %s",SDL_GetError());
        return;
    }
	atexit(SDL_Quit);
	if (fullscreen)
    {
        if (renderSize.w == 0)
        {
            flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
        }
        else
        {
            flags |= SDL_WINDOW_FULLSCREEN;
        }
    }

	graphicsMainWindow = SDL_CreateWindow(windowName, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, renderSize.w, renderSize.h, flags);
    if (!graphicsMainWindow)
    {
        slog("failed to create main window: %s",SDL_GetError());
        graphics_close();
        return;
    }

	graphicsRenderer = SDL_CreateRenderer(graphicsMainWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    if (!graphicsRenderer)
    {
        slog("failed to create renderer: %s",SDL_GetError());
        graphics_close();
        return;
    }

	SDL_RenderClear(graphicsRenderer);
    SDL_RenderPresent(graphicsRenderer);
	//sets a hint with normal priority, this specifically sets the scaling quality to be linear, this is supported by OpenGL and Direct3D
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    //sets a device independent resolution for rendering
	SDL_RenderSetLogicalSize(graphicsRenderer, renderSize.w, renderSize.h);
    
    atexit(graphics_close);
}

/** @brief closes the window and the renderer at exit */
void graphics_close()
{
	if (graphicsRenderer)
    {
        SDL_DestroyRenderer(graphicsRenderer);
    }
    if (graphicsMainWindow)
    {
        SDL_DestroyWindow(graphicsMainWindow);
    }
    graphicsMainWindow = NULL;
    graphicsRenderer = NULL;
}

/** @brief	goes to the next frame then holds for a frame delay. */
void graphics_next_frame()
{
	SDL_RenderPresent(graphicsRenderer);
	graphics_frame_delay(); 
}

/** @brief	delay's frame rate so the screen and code are synched up properly */
void graphics_frame_delay()
{
	Uint32 diff;
	graphicsThen = graphicsNow;
    graphicsNow = SDL_GetTicks();
    diff = (graphicsNow - graphicsThen);
    if (diff < graphicsFrameDelay)
    {
        SDL_Delay(graphicsFrameDelay - diff);
    }
    graphicsFPS = 1000.0/MAX(graphicsNow - graphicsThen,0.001);
    if (graphicsPrintFPS)
    {
        //printf("FPS: %f\n",graphicsFPS); //printf so the log isn't filled with frame rate
    }
}

/**
 * @brief	getter for the game's renderer so the rest of the code can use it.
 * @return	a SDL_Renderer pointer used for all the game's rendering.
 */
SDL_Renderer *graphics_get_renderer()
{
	return graphicsRenderer;
}

/**
 * @brief	getter for the game's time
 * @return	the graphicsNow
 */
Uint32 get_time()
{
	return graphicsNow;
}