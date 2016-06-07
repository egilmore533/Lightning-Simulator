#ifndef __SPRITE_H__
#define __SPRITE_H__

#include "SDL_image.h"

#include "vector.h"

/**
 * @file	sprite.h
 * @brief	2D sprite memory management and rendering
 */

/**
 * @struct the sprite structure, it contains information on the sprite and a pointer to the SDL_Texture
 * @brief a container for the SDL_Texture so it can be referenced mutliple times without haveing to reload it, also contains useful info on the texture
 */
typedef struct Sprite_t
{
	char *filename;			/**< path from the working directory to the image, used to uniquely id sprites and know if we are loading multiples of the same sprite */
	Vect2d frameSize;		/**< size of one frame from the sprite sheet */
	Vect2d imageSize;		/**< total image size of the sprite */
	SDL_Texture *image;		/**< pointer to the loaded texture */
	int refCount;			/**< how many times this sprite has been referenced */
	int fpl;				/**< frames per line on the sprite sheet */
	int frames;				/**< total frames in the sprite */
}Sprite;

/**
 * @brief	removes one reference from spriteList, if the
 * 			refCount is 0 frees the sprite from spriteList and frees the sprite pointer.
 * @param [in,out]	sprite	double pointer to the sprite.
 */
void sprite_free(Sprite **free);

/** @brief	closes the sprite system by freeing the entire sprite list and setting the number of sprites in the list to 0*/
void sprite_close_system();

/**
 *  @brief opens the sprite system by allocating and memsetting the spriteList to have room for the provided number of sprites 
 *  @param	maxSprites	the maximum number of different sprites the spriteList will be able to support
 */
void sprite_init_system(int maxSprites);

/** 
 * @brief loads a sprite into the spriteList using the given info
 * @param	[in] filename	the filepath for the image
 * @param	frameSize		2d vector defining how large a frame of the image will be
 * @param	fpl				the frames per line on the image
 * @param	frames			the total number of frames that the image has, used to know when the sprite has gone through the animation
 * @return A pointer to the sprite with the info provided
 */
Sprite *sprite_load(char *filename, Vect2d frameSize, int fpl, int frames);

/**
 * @brief draws the sprite frame to the screen at the given position
 * @param	[in] sprite		the image reference to be drawn from
 * @param	frame			the frame of  the image to draw
 * @param	drawPos			2D vector of where the sprite should be drawn in the game world
 * @param	scale			how much to scale the image
 * @param	[in] center		the center point of the image to rotate around
 * @param	angle			the angle to rotate it by
 * @param	flip			whether or not to flip the image
 */
void sprite_draw(Sprite *sprite, int frame, Vect2d drawPos, Vect2d scale, SDL_Point *center, float angle, SDL_RendererFlip flip);

/**
 * @brief draws the sprite frame to the screen at the position given, adds alpha and size variation to scale the sprite and create a bloom
 * @param	[in] sprite		the image reference to be drawn from
 * @param	frame			the frame of  the image to draw
 * @param	drawPos			2D vector of where the sprite should be drawn in the game world
 * @param	scale			how much to scale the image
 * @param	[in] center		the center point of the image to rotate around
 * @param	angle			the angle to rotate it by
 * @param	flip			whether or not to flip the image
 */
void sprite_bloom_draw(Sprite *sprite, int frame, Vect2d drawPos, Vect2d scale, SDL_Point *center, float angle, SDL_RendererFlip flip);



#endif