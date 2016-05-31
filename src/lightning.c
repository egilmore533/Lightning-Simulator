#include <math.h>


#include "lightning.h"
#include "simple_logger.h"


static Sprite *middleChunk = NULL;
static Sprite *rightCap = NULL;
static Sprite *leftCap = NULL;

static Lightning *lightningList = NULL;
static int lightningNum = 0;
static int lightningMax = 0;

void lightning_init_system(int maxLightning)
{
	int i;
	if(maxLightning == 0)
	{
		slog("Max Lightning == 0");
		return;
	}
	lightningList = (Lightning *)malloc(sizeof(Lightning) * maxLightning);
	if(!lightningList)
	{
		slog("lightningList failed to initialize");
		return;
	}
	memset(lightningList, 0, sizeof(Lightning) * maxLightning);
	
	middleChunk = sprite_load("images/middle_chunk.png", vect2d_new(1, 8), 1, 1);
	leftCap = sprite_load("images/left_cap.png", vect2d_new(4, 8), 1, 1);
	rightCap = sprite_load("images/right_cap.png", vect2d_new(4, 8), 1, 1);

	lightningNum = 0;
	lightningMax = maxLightning;
	atexit(lightning_close_system);
}

void lightning_close_system()
{
	int i;
	Lightning *lightning = NULL;
	if(!lightningList)
	{
		slog("lightningList not initialized");
		return;
	}
	for(i = 0; i < lightningMax; ++i)
	{
		lightning = &lightningList[i];
		lightning_free(&lightning);
	}

	free(lightningList);
	lightningList = NULL;
	lightningNum = 0;
	lightningMax = 0;
}

void lightning_free(Lightning **lightning)
{
	Lightning *target;
	if(!lightning)
	{
		return;
	}
	else if(!*lightning)
	{
		return;
	}
	target = *lightning;

	target->inUse = 0;
	lightningNum--;
	*lightning = NULL;
}

Lightning *lightning_new(Vect2d start, Vect2d end, float thickness, float rotation)
{
	int i;
	Lightning *lightning = NULL;

	if(!lightningList)
	{
		slog("lightningList uninitialized");
		return NULL;
	}

	if(lightningNum + 1 > lightningMax)
	{
		slog("Maximum Lightning Reached.");
		exit(1);
	}

	for(i = 0; i < lightningMax; i++)
	{
		if(!lightningList[i].inUse)
		{
			lightning = &lightningList[i];
			break;
		}
	}

	memset(lightning,0,sizeof(Lightning));

	lightningNum++;
	lightning->inUse = 1;
	lightning->start = start;
	lightning->end = end;
	lightning->thickness = thickness;
	return lightning;

}

void lightning_draw(Lightning *self)
{
	Vect2d tangent; 
	Vect2d leftCapPosition, rightCapPosition;
	Vect2d middleOrigin, middleScale;
	float rot, thick;
	SDL_Point *center = NULL;

	center = (SDL_Point *) malloc(sizeof(SDL_Point));
	memset(center, 0, sizeof(SDL_Point));

	vect2d_subtract(self->end, self->start, tangent);
	rot = atan2(tangent.y, tangent.x) * 57.2957795;
	thick = self->thickness / LIGHTNING_THICKNESS;

	center->x = 0;
	center->y = 0;
	sprite_draw(middleChunk, 1, self->start, vect2d_new(vect2d_get_length(tangent) + 1, thick), center, rot, SDL_FLIP_NONE);
	sprite_draw(rightCap, 1, vect2d_new(self->start.x - 4 * thick, self->start.y), vect2d_new(thick, thick), center, rot, SDL_FLIP_HORIZONTAL);
	sprite_draw(rightCap, 1, self->end, vect2d_new(thick, thick), center, rot, SDL_FLIP_NONE);

}
