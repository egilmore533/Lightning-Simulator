#include <math.h>

#include "vector.h"


/**
 * @brief	quick constructor for a vect2d.
 * @param	a	The float to become the x/w.
 * @param	b	The float to become the y/h.
 * @return	the new vect2d.
 */
Vect2d vect2d_new(float a, float b)
{
	Vect2d vector = {a, b};
	return vector;
}

/**
 * @brief	quick constructor for a vect3d.
 * @param	a	The float to become the x.
 * @param	b	The float to become the y.
 * @param	c	The float to become the z.
 * @return	the new vect2d.
 */
Vect3d vect3d_new(float a, float b, float c)
{
	Vect3d v = {a, b, c};
	return v;
}

/**
 * @brief	Vect2d get length via ya homie pythagoras.
 * @param	vect	The vect.
 * @return	A float.
 */
float vect2d_get_length(Vect2d vect)
{
	return sqrt(vect.x * vect.x + vect.y * vect.y);
}

/**
 * @brief	Vect3d get length via ya homie pythagoras.
 * @param	vect	The vect.
 * @return	A float.
 */
float vect3d_get_length(Vect3d vect)
{
	return sqrt(vect.x * vect.x + vect.y * vect.y + vect.z * vect.z);
}

/**
 * @brief	Vect 2D normalize.
 * @param [in,out]	vect	If non-null, the vect.
 */
void vect2d_normalize(Vect2d *vect)
{
	float length;
	if(vect == NULL)
	{
		return;
	}
	length = vect2d_get_length(*vect);
	if(length == 0.0f)
	{
		return;
	}

	length = 1/length;
	vect->x *= length;
	vect->y *= length;
}

/**
 * @brief	Vect 3D normalize.
 * @param [in,out]	vect	If non-null, the vect.
 */
void vect3d_normalize(Vect3d *vect)
{
	float length;
	if(vect == NULL)
	{
		return;
	}
	length = vect3d_get_length(*vect);
	if(length == 0.0f)
	{
		return;
	}

	length = 1/length;
	vect->x *= length;
	vect->y *= length;
	vect->z *= length;
}

/**
 * @brief	rectangle constructor.
 * @param	x	The x coordinate.
 * @param	y	The y coordinate.
 * @param	w	The width.
 * @param	h	The height.
 * @return	A SDL_Rect with given values.
 */
SDL_Rect rect(int x, int y, int w, int h)
{
	SDL_Rect r;
	r.x = x;
	r.y = y;
	r.w = w;
	r.h = h;
	return r;
}

/**
 * @brief	check if two rectangles intersect
 * @param	a	The SDL_Rect to process.
 * @param	b	The SDL_Rect to process.
 * @return  a flag true or false.
 */
int rect_intersect(SDL_Rect a, SDL_Rect b)
{
	if((a.x + a.w >= b.x) && 
	   (b.x + b.w >= a.x) && 
	   (a.y + a.h >= b.y) && 
	   (b.y + b.h >= a.y) )
	{
		return 1;
	}
	return 0;
}