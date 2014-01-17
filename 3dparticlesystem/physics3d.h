/* 
 * Adam Bysice - 0753709
 * File: Mathlib2D.h
 * ----------------------
 * Interface for 3D math library
 */

#ifndef _physics3d_h
#define _physics3d_h

/*
void collision2D(point2D p1, point2D p2, vec2D *v1, vec2D *v2);
*/

void collision3D(point3D p1, point3D p2, vec3D& v1, vec3D& v2);

float greater(float s1, float s2);


#endif