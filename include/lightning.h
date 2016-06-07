#ifndef __LIGHTNING_H__
#define __LIGHTNING_H__

#include "sprite.h"

/**
 * @file	lightning.h
 * @brief	handling of lightning (lines). Provides memory management, creation, deletion, and drawing of the lightning to the screen. 
 */

#define LIGHTNING_THICKNESS		8			/**< the thickness of the standard image for the lightning bolt (frameSize.y) */

#define SWAY					100			/**< The amount of displacement allowed for from segment to segment in the lightning bolt */

#define JAGGEDNESS				1 / SWAY	/**< how perpendicular the segments in the bolt are allowed to be */

/**
 * @struct the Lightning (Line) structure, contains the start and end points of the lightning, thickness of the lightning, and function pointers to free and draw the lightning
 * @brief a line segment and function pointers that allow for the creation of a lightning bolt
 */
typedef struct Line_t
{
	int inUse;								/**< flag to know if the lightning is in use */

	Vect2d start;							/**< starting point of the line segment */
	Vect2d end;								/**< end point of the line segment */

	float thickness;						/**< thickness of the line segment */

	void (*free)(struct Line_t **self);		/**< function that frees the lightning from memory */
	void (*draw)(struct Line_t *self);		/**< function that will draw the lightning to screen (also blooms it) */
}Lightning;

/**
 * @struct used to make a linked list of points (float) on the line segment of the main lightning bolt
 * @brief contains a pointer to the next point on the line, and the position of this point
 */
typedef struct Position_t
{
	struct Position_t *next;				/**< pointer tothe next point in the list */
	float pos;								/**< the position on the lightning to be displaced */
}Position;

/**
 * @brief sorts the linked list given to it by the pos, smallest to largest, recursively calls itself to shorten until comparing one position to the last position in the list
 * @param head [in,out]		the first position in the list of positions to be sorted
 * @return the head of the newly sorted linkedList
 */
Position *sort_positions(Position *head);

/**
 * @brief initializes the lightning memory management system, also loads the sprites needed to draw the lightning
 * @param maxLightning		the maximum amount of lightning segments that can exist at a time
 */
void lightning_init_system(int maxLightning);

/**
 * @brief closes the lightning memory management system
 */
void lightning_close_system();

/**
 * @brief frees a lightning segment from the lightningList and destroys the pointer to it so anything with a pointer to that lightning segment is destroyed
 * @param lightning [in,out]		the lightning that is to be removed from memory
 */
void lightning_free(Lightning **lightning);

/**
 * @brief creates a new lightning in the lightningList with the given info
 * @param start		vect2d of the starting point for the lightning
 * @param end		vect2d of the ending point for the lightning
 * @param thickness	how thick the lightning will be 
 * @return pointer to the position in the lightningList where the newly created lightning exists
 */
Lightning *lightning_new(Vect2d start, Vect2d end, float thickness);

/**
 * @brief draws the lightning to the renderer using the statically held sprites, also draws the bloom for the lightning
 *			uses trig to determine how long the lightning should be, and what angle it should be drawn at
 * @param self [in]	the lightning that is to be drawn
 */
void lightning_draw(Lightning *self);

/**
 * @brief draw all lighting in the lightningList that has a draw function. Also color mods the sprites that all lightning share periodically to go throught the rainbow.
 */
void lightning_draw_all();

/**
 * @breif creates the actual bolt of lightning, generates points randomly on the line segment, based on how long it is. 
 *			Then sorts the linked list of points on the line. Finally randomly displace the points under parameters of the previous point,
 *			and predefined values for sway and jaggedness that we want the bolt to have.
 * @param main_lightning [in]	the main lightning, that defines the start and end of the bolt we are about to make
 * @param thickness				the thickness of the bolt we are creating
 */
void lightning_create_bolt(Lightning *main_lightning, float thickness);

/**
 * @brief removes all lightning in the lightningList
 */
void lightning_purge_system();

#endif