#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "SDL.h"

#include "vector.h"

/**
 * @file	graphics.h
 * @brief	the graphic and rendering pipeline for the project, also keeps track of gametime
 */

/**
 * @brief	initializes the main window and the main renderer.
 * @param   [in]	windowName	If non-null, name of the window, will be displayed at the top of the window.
 * @param	viewSize		  	Width and Height of the view.
 * @param	renderSize		  	Width and Height of the render.
 * @param	fullscreen		  	flag to use fullscreen or not.
 */
void graphics_init(char *windowName, Vect2d viewSize, Vect2d renderSize, int fullscreen);

/** @brief closes the window and the renderer at exit */
void graphics_close();

/** @brief	goes to the next frame then holds for a frame delay. */
void graphics_next_frame();

/** @brief	delay's frame rate so the screen and code are synched up properly */
void graphics_frame_delay();

/**
 * @brief	getter for the game's renderer so the rest of the code can use it.
 * @return	a SDL_Renderer pointer used for all the game's rendering.
 */
SDL_Renderer *graphics_get_renderer();

/**
 * @brief	getter for the game's time
 * @return	the graphicsNow
 */
Uint32 get_time();

#endif