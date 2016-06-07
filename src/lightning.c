#include <math.h>
#include <stdlib.h>

#include "simple_logger.h"

#include "graphics.h"
#include "lightning.h"

static Sprite *middleChunk = NULL;
static Sprite *rightCap = NULL;
static Sprite *leftCap = NULL;
static Vect3d color = {255, 255, 0};

static Lightning *lightningList = NULL;
static int lightningNum = 0;
static int lightningMax = 0;

/**
 * @brief sorts the linked list given to it by the pos, smallest to largest, recursively calls itself to shorten until comparing one position to the last position in the list
 * @param head [in,out]		the first position in the list of positions to be sorted
 * @return the head of the newly sorted linkedList
 */
Position *sort_positions(Position *head)
{
	Position *current, *prev, *smallest, *smallestPrev, *largest;
	Position *temp;

	if(!head || !head->next)
	{
		slog("list is already sorted/doesn't exist");
		return;
	}
	current = head;
	smallest = head;
	prev = head;
	smallestPrev = head;
	largest = head;

	while(current->next != NULL)
	{
		if(current->pos < smallest->pos)
		{
			smallestPrev = prev;
			smallest = current;
		}
		if(current->pos > largest->pos)
		{
			largest = current;
		}
		prev = current;
		current = current->next;
	}

	if(smallest != head)
	{
		smallestPrev->next = head;
		temp = head->next;
		head->next = smallest->next;
		smallest->next = temp;
	}

	smallest->next = sort_positions(smallest->next);
	if(smallest->next == largest)
	{
		smallest->next->next = NULL;
	}
	return smallest;
}

/**
 * @brief initializes the lightning memory management system, also loads the sprites needed to draw the lightning
 * @param maxLightning		the maximum amount of lightning segments that can exist at a time
 */
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

/**
 * @brief closes the lightning memory management system
 */
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

/**
 * @brief frees a lightning segment from the lightningList and destroys the pointer to it so anything with a pointer to that lightning segment is destroyed
 * @param lightning [in,out]		the lightning that is to be removed from memory
 */
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

/**
 * @brief creates a new lightning in the lightningList with the given info
 * @param start		vect2d of the starting point for the lightning
 * @param end		vect2d of the ending point for the lightning
 * @param thickness	how thick the lightning will be 
 * @return pointer to the position in the lightningList where the newly created lightning exists
 */
Lightning *lightning_new(Vect2d start, Vect2d end, float thickness)
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
	lightning->free = &lightning_free;
	lightning->draw = &lightning_draw;
	return lightning;

}

/**
 * @brief draws the lightning to the renderer using the statically held sprites, also draws the bloom for the lightning
 *			uses trig to determine how long the lightning should be, and what angle it should be drawn at
 * @param self [in]	the lightning that is to be drawn
 */
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

	SDL_SetTextureBlendMode(leftCap->image, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(rightCap->image, SDL_BLENDMODE_BLEND);
	sprite_bloom_draw(middleChunk, 1, self->start, vect2d_new(vect2d_get_length(tangent) + 1, thick), center, rot, SDL_FLIP_NONE);
	sprite_bloom_draw(leftCap, 1, vect2d_new(self->start.x - thick, self->start.y - thick), vect2d_new(thick, thick), center, rot, SDL_FLIP_NONE);
	sprite_bloom_draw(rightCap, 1, self->end, vect2d_new(thick, thick), center, rot, SDL_FLIP_NONE);

	sprite_draw(middleChunk, 1, self->start, vect2d_new(vect2d_get_length(tangent) + 1, thick), center, rot, SDL_FLIP_NONE);
	sprite_draw(leftCap, 1, vect2d_new(self->start.x - thick, self->start.y - thick), vect2d_new(thick, thick), center, rot, SDL_FLIP_NONE);
	sprite_draw(rightCap, 1, self->end, vect2d_new(thick, thick), center, rot, SDL_FLIP_NONE);

}

/**
 * @brief draw all lighting in the lightningList that has a draw function. Also color mods the sprites that all lightning share periodically to go throught the rainbow.
 */
void lightning_draw_all()
{
	int i;
	static int alpha = 255;
	static int red = 0;
	static int green = 1;
	static int blue = 0;
	static int up = 0;

	SDL_SetTextureAlphaMod(middleChunk->image, alpha);
	SDL_SetTextureAlphaMod(rightCap->image, alpha);
	SDL_SetTextureAlphaMod(leftCap->image, alpha);

	SDL_SetTextureColorMod(leftCap->image, color.r, color.g, color.b);
	SDL_SetTextureColorMod(middleChunk->image, color.r, color.g, color.b);
	SDL_SetTextureColorMod(rightCap->image, color.r, color.g, color.b);

	if(red)
	{
		if(up)
		{
			color.r += 5;
			if(color.r >= 255)
			{
				color.r = 255;
				up = 0;
				red = 0;
				green = 1;
			}
		}
		else
		{
			color.r -= 5;
			if(color.r <= 0)
			{
				color.r = 0;
				red = 0;
				green = 1;
				up = 1;
			}
		}
	}
	if(green)
	{
		if(up)
		{
			color.g += 5;
			if(color.g >= 255)
			{
				color.g = 255;
				up = 0;
				green = 0;
				blue = 1;
			}
		}
		else
		{
			color.g -= 5;
			if(color.g <= 0)
			{
				color.g = 0;
				green = 0;
				blue = 1;
				up = 1;
			}
		}
	}
	if(blue)
	{
		if(up)
		{
			color.b += 5;
			if(color.b >= 255)
			{
				color.b = 255;
				up = 0;
				blue = 0;
				red = 1;
			}
		}
		else
		{
			color.b -= 5;
			if(color.b <= 0)
			{
				color.b = 0;
				blue = 0;
				red = 1;
				up = 1;
			}
		}
	}

	//alpha = 100 * (1 + sin(get_time() * 2 * 3.14 / 2000));

	for(i = 0; i < lightningMax; i++)
	{
		if(lightningList[i].inUse && lightningList[i].draw)
		{
			lightningList[i].draw(&lightningList[i]);
		}
	}
}

/**
 * @breif creates the actual bolt of lightning, generates points randomly on the line segment, based on how long it is. 
 *			Then sorts the linked list of points on the line. Finally randomly displace the points under parameters of the previous point,
 *			and predefined values for sway and jaggedness that we want the bolt to have.
 * @param main_lightning [in]	the main lightning, that defines the start and end of the bolt we are about to make
 * @param thickness				the thickness of the bolt we are creating
 */
void lightning_create_bolt(Lightning *main_lightning, float thickness)
{
	int i;
	Vect2d tangent;
	Vect2d normal;
	float length;
	Position *headPosition = NULL;
	Position *currentPosition;

	Vect2d prevPoint = main_lightning->start;
	float prevDisplacement = 0;
	float pos, prevPos;
	float scale;
	float envelope;
	float displacement;
	Vect2d point;
	Vect2d temp, temp2;

	vect2d_subtract(main_lightning->end, main_lightning->start, tangent);
	normal = vect2d_new(tangent.y, -tangent.x);
	vect2d_normalize(&normal);
	length = vect2d_get_length(tangent);

	headPosition = (Position *)malloc(sizeof(Position));
	currentPosition = headPosition;
	currentPosition->pos = 0;
	currentPosition->next = (Position *)malloc(sizeof(Position));
	currentPosition = currentPosition->next;
	currentPosition->next = NULL;

	for(i = 0; i < length / (thickness * 4); i++)
	{
		currentPosition->pos = ((float)rand() / (float)RAND_MAX/1); //random float between 1 and 0
		currentPosition->next = (Position *)malloc(sizeof(Position));
		currentPosition = currentPosition->next;
		currentPosition->next = NULL;
	}

	headPosition = sort_positions(headPosition);

	currentPosition = headPosition->next;
	prevPos = headPosition->pos;
	while(currentPosition->next != NULL)
	{
		pos = currentPosition->pos;
		scale = (length * JAGGEDNESS) * (pos - prevPos);

		if(pos > 0.95f)
		{
			envelope = 20 * (1 - pos);
		}
		else
		{
			envelope = 1;
		}

		displacement = (rand() % (2 * SWAY)) - SWAY;
		displacement -= (displacement - prevDisplacement) * (1 - scale);
		displacement *= envelope;

		vect2d_scale(temp, tangent, pos);
		vect2d_scale(temp2, normal, displacement);
		vect2d_add(temp, temp2, point); 
		vect2d_add(point, main_lightning->start, point);

		lightning_new(prevPoint, point, thickness);


		prevPoint = point;
		prevDisplacement = displacement;
		prevPos = pos;
		currentPosition = currentPosition->next;
	}

	lightning_new(prevPoint, main_lightning->end, thickness);
	
	return;
}

/**
 * @brief removes all lightning in the lightningList
 */
void lightning_purge_system()
{
	int i;
	Lightning *lightning = NULL;
	for(i = 0; i < lightningMax; i++)
	{
		if(lightningList[i].inUse && lightningList[i].free)
		{
			lightning = &lightningList[i];
			lightningList[i].free(&lightning);
		}
	}
}