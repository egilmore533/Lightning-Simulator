#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "simple_logger.h"

#include "SDL_image.h"
#include "sprite.h"
#include "graphics.h"


static Sprite *spriteList = NULL;
static int spriteNum = 0;
static int spriteMax = 0;



/**
 * @brief	removes one reference from spriteList, if the
 * 			refCount is 0 frees the sprite from spriteList and frees the sprite pointer.
 * @param [in,out]	sprite	double pointer to the sprite.
 */
void sprite_free(Sprite **sprite)
{
	Sprite *target;
	if(!sprite)
	{
		return;
	}
	else if(!*sprite)
	{
		return;
	}
	target = *sprite;

	target->refCount--;
	if(target->refCount == 0)
	{
		if(target->image != NULL)
		{
			SDL_DestroyTexture(target->image); 
		}
		spriteNum--;
	}
	*sprite = NULL;
}

/** @brief	closes the sprite system by freeing the entire sprite list and setting the number of sprites in the list to 0*/
void sprite_close_system()
{
	int i;
	if(!spriteList)
	{
		slog("spriteList not initialized");
		return;
	}
	for(i = 0; i < spriteMax; ++i)
	{
		if(spriteList[i].image != 0)
		{
			SDL_DestroyTexture(spriteList[i].image);
		}
	}

	free(spriteList);
	spriteList = NULL;
	spriteNum = 0;
	spriteMax = 0;
}

/**
 *  @brief opens the sprite system by allocating and memsetting the spriteList to have room for the provided number of sprites 
 *  @param	maxSprites	the maximum number of different sprites the spriteList will be able to support
 */
void sprite_init_system(int maxSprites)
{
	int i;
	if(maxSprites == 0)
	{
		slog("Max sprite == 0");
		return;
	}
	spriteList = (Sprite *)malloc(sizeof(Sprite) * maxSprites);
	if(!spriteList)
	{
		slog("spriteList failed to initialize");
		return;
	}
	memset(spriteList, 0, sizeof(Sprite) * maxSprites);
	spriteNum = 0;
	for(i = 0; i < maxSprites; ++i)
	{
		spriteList[i].image = NULL;
	}
	spriteMax = maxSprites;
	atexit(sprite_close_system);
}

/** 
 * @brief loads a sprite into the spriteList using the given info
 * @param	[in] filename	the filepath for the image
 * @param	frameSize		2d vector defining how large a frame of the image will be
 * @param	fpl				the frames per line on the image
 * @param	frames			the total number of frames that the image has, used to know when the sprite has gone through the animation
 * @return A pointer to the sprite with the info provided
 */
Sprite *sprite_load(char *filename, Vect2d frameSize, int fpl, int frames)
{
	int i;
	SDL_Surface *tempSurface;
	SDL_Texture *tempTexture;
	Sprite *sprite = NULL;
	SDL_Renderer *renderer = graphics_get_renderer();

	if(!spriteList)
	{
		slog("spriteList uninitialized");
		return NULL;
	}
	/*first search to see if the requested sprite image is alreday loaded*/
	for(i = 0; i < spriteMax; i++)
	{
		if(spriteList[i].refCount == 0)
		{
			//this makes it so that the next sprite available in the list will be used if no sprite is found to match this one
			if(sprite == NULL)
				sprite = &spriteList[i];
			continue;
		}
		if(strncmp(filename, spriteList[i].filename, 128) ==0)
		{
			spriteList[i].refCount++;
			return &spriteList[i];
		}
	}
	/*makesure we have the room for a new sprite*/
	if(spriteNum + 1 > spriteMax)
	{
		slog("Maximum Sprites Reached.");
		exit(1);
	}

	memset(sprite,0,sizeof(Sprite));

	/*if its not already in memory, then load it.*/
	spriteNum++;
	tempSurface = IMG_Load(filename);

	if(tempSurface == NULL)
	{
		slog("unable to load sprite as a surface");
		exit(0);
	}
	else
	{
		/*sets a transparent color for blitting.*/
		SDL_SetColorKey(tempSurface, SDL_TRUE , SDL_MapRGB(tempSurface->format, 255,255,255));
	
		tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
		if(tempTexture == NULL)
		{
			slog("unable to load sprite as a Texture");
			exit(0);
		}
	}
	
	/*then copy the given information to the sprite*/
	sprite->image = tempTexture;	
	sprite->fpl = fpl;
	sprite->imageSize = vect2d_new(tempSurface->w, tempSurface->w); 
	sprite->frameSize.x = frameSize.x;
	sprite->frameSize.y = frameSize.y;
	sprite->filename = filename;
	sprite->frames = frames;
	sprite->refCount++;
	SDL_FreeSurface(tempSurface);
	return sprite;
}

/**
 * @brief draws the sprite frame to the screen at the position relative to the camera
 * @param	[in] sprite		the image reference to be drawn from
 * @param	frame			the frame of  the image to draw
 * @param	drawPos			2D vector of where the sprite should be drawn in the game world
 * @param	scale			how much to scale the image
 * @param	[in] center		the center point of the image to rotate around
 * @param	angle			the angle to rotate it by
 * @param	flip			whether or not to flip the image
 */
void sprite_draw(Sprite *sprite, int frame, Vect2d drawPos, Vect2d scale, SDL_Point *center, float angle, SDL_RendererFlip flip)
{
	SDL_Rect source, destination;
	SDL_Renderer *renderer = graphics_get_renderer();
	if(!sprite)
	{
		slog("sprite doesn't point to anything");
		return;
	}
	source.x = frame % sprite->fpl * sprite->frameSize.x;
	source.y = frame / sprite->fpl * sprite->frameSize.y;
	source.w = sprite->frameSize.x;
	source.h = sprite->frameSize.y;
	
	destination.x = drawPos.x;
	destination.y = drawPos.y;
	destination.w = sprite->frameSize.x * scale.x;
	destination.h = sprite->frameSize.y * scale.y;
	SDL_RenderCopy(renderer, sprite->image, &source, &destination);
}
