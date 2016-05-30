#ifndef __VECTOR_H__
#define __VECTOR_H__

#include "SDL.h"

/**
 * @file	vector.h
 * @brief	2D vector, its functions/macros
 */

#define PI					3.141589	/**< Pi for the our purposes */

#define WINDOW_HEIGHT		768
#define WINDOW_WIDTH		1366

#define MAX(a,b) (a>b?a:b)				/**< return the largest of two values. can be read as: if a greater than b then a, else b */

#define MIN(a,b) (a<b?a:b)				/**< return the smallest of two values. can be read as: if a less than b then a, else b */


/**
 * @brief	A macro that subtracts the second vect2d from the second, and puts it into the third vect2d.
 * @param	a   	The whole.
 * @param	b   	the part being removed from the whole.
 * @param	diff	The difference.
 */
#define vect2d_subtract(a,b,diff)(diff.x=a.x-b.x,diff.y=a.y-b.y)

/**
 * @brief	A macro that adds two vect2d together.
 * @param	a  	The first vect2d to add.
 * @param	b  	The second vect2d to add.
 * @param	sum	The resulting sum of the two vect2ds in a vect2d.
 */
#define vect2d_add(a,b,sum) (sum.x=a.x+b.x, sum.y=a.y+b.y)

/**
 * @brief	A macro that mutiplies two vect2ds together and puts the product inside a vect2d.
 * @param	a   	The first vect2d to mutiply.
 * @param	b   	The second vect2d to mutiply.
 * @param	prod	The resulting product of the two vect2ds in a vect2d.
 */
#define vect2d_mutiply(a,b,prod) (prod.x=a.x*b.x, prod.y=a.y*b.y)

/**
 * @brief	A macro that sets a vect2d's constraints to given floats.
 * @param	vect	The vect to set.
 * @param	a   	The x coordinate of the vect2d.
 * @param	b   	The y coordinate of the vect2d.
 */
#define vect2d_set(vect, a,b) (vect.x=(a), vect.y=(b))

/**
 * @brief	A macro that takes a vect2d and negates it.
 * @param	neg 	The resulting negative vect2d.
 * @param	vect	The vect2d to be negated.
 */
#define vect2d_negate(neg,vect) (neg.x=(-vect.x), neg.y=-(vect.y))

/**
 * @brief	A macro that takes a vect2d and a factor to mutiply the vect2d by
 * @param	vect  	The resulting vect2d.
 * @param	vectIn	The vect2d in.
 * @param	factor	The factor that the vector is being scaled by.
 */
#define vect2d_scale(vect,vectIn,factor) (vect.x=vectIn.x*factor, vect.y=vectIn.y*factor)


/**
 * @struct the vector 2d structure, it contains 2 floats
 * @brief a simple 2d vector structure containing two floats
 */
typedef struct
{
	union {
		float x;	/**< the x component of the vector */
		float w;	/**< the width component of the vector */
	};
	union {
		float y;	/**< the y component of the vector */
		float h;	/**< the height component of the vector */
	};
}Vect2d;

/**
 * @brief	quick constructor for a vect2d.
 * @param	a	The float to become the x/w.
 * @param	b	The float to become the y/h.
 * @return	the new vect2d.
 */
Vect2d vect2d_new(float a, float b);

/**
 * @brief	Vect2d get length via ya homie pythagoras.
 * @param	vect	The vect.
 * @return	A float.
 */
float vect2d_get_length(Vect2d vect);

/**
 * @brief	Vect 2D normalize.
 * @param [in,out]	vect	If non-null, the vect.
 */
void vect2d_normalize(Vect2d *vect);

/**
 * @brief	rectangle constructor.
 * @param	x	The x coordinate.
 * @param	y	The y coordinate.
 * @param	w	The width.
 * @param	h	The height.
 * @return	A SDL_Rect with given values.
 */
SDL_Rect rect(int x, int y, int w, int h);

/**
 * @brief	check if two rectangles intersect
 * @param	a	The SDL_Rect to process.
 * @param	b	The SDL_Rect to process.
 * @return  a flag true or false.
 */
int rect_intersect(SDL_Rect a, SDL_Rect b);

#endif
