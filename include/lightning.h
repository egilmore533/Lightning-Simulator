#ifndef __LIGHTNING_H__
#define __LIGHTNING_H__

#include "sprite.h"

#define LIGHTNING_THICKNESS		8

#define SWAY					80

#define JAGGEDNESS				1 / SWAY

typedef struct Line_t
{
	int inUse;			/**< flag to know if the lightning is in use */

	Vect2d start;		/**< starting point of the line segment */
	Vect2d end;			/**< end point of the line segment */

	float thickness;	/**< thickness of the line segment */

	void (*free)(struct Line_t **self);
	void (*draw)(struct Line_t *self);
}Lightning;

typedef struct Position_t{
	struct Position_t *next;
	float pos;
}Position;

Position *sort_positions(Position *head);

void lightning_init_system(int maxLightning);

void lightning_close_system();

void lightning_free(Lightning **lightning);

Lightning *lightning_new(Vect2d start, Vect2d end, float thickness);

void lightning_draw(Lightning *self);

void lightning_create_bolt(Lightning *main_lightning, float thickness);

void lightning_draw_all();

#endif