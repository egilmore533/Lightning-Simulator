#ifndef __LIGHTNING_H__
#define __LIGHTNING_H__

#include "sprite.h"

#define LIGHTNING_THICKNESS		8

typedef struct Line_t
{
	int inUse;			/**< flag to know if the lightning is in use */

	Vect2d start;		/**< starting point of the line segment */
	Vect2d end;			/**< end point of the line segment */

	float thickness;	/**< thickness of the line segment */
}Lightning;

void lightning_init_system(int maxLightning);

void lightning_close_system();

void lightning_free(Lightning **lightning);

Lightning *lightning_new(Vect2d start, Vect2d end, float thickness, float rotation);

void lightning_draw(Lightning *self);

#endif