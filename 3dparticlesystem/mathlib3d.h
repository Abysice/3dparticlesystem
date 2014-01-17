/* 
 * Adam Bysice - 0753709
 * File: Mathlib2D.h
 * ----------------------
 * Interface for 3D math library
 */

#ifndef _mathlib3d_h
#define _mathlib3d_h

#include <math.h>
#define _USE_MATH_DEFINES 
#define ToRadian(x) (float)(((x) * M_PI / 180.0f))
#define ToDegree(x) (float)(((x) * 180.0f / M_PI))


//represents a 3D point with floats/floats for x, y and z components
class point3D {
public:
	point3D(float x, float y, float z);
	point3D();
	float x;
	float y;
	float z;
};

//point with 2 integers, used for mouse position implementation
class point2Di {
public:
	point2Di(int x, int y);
	point2Di();
    int x;
    int y;
};

//represents a 3D vector with floats/floats for x, y, z and magnitude (i.e.,the vector’s length)
class vec3D {
public:
	vec3D(float x, float y, float z);
	vec3D();
	float length();
	float magnitude;
	vec3D cross(const vec3D& v) const;
    vec3D& normalize();
	void Rotate(float Angle, const vec3D& Axis);
	
	float x;
	float y;
	float z;
	
	//operator methods
	vec3D& operator+=(const vec3D& r) {
        x += r.x;
        y += r.y;
        z += r.z;
        return *this;
	}
	vec3D& operator-=(const vec3D& r) {
        x -= r.x;
        y -= r.y;
        z -= r.z;
		return *this;
    }
	vec3D& operator*=(float f) {
        x *= f;
        y *= f;
        z *= f;
        return *this;
    }
};

// more operators
inline vec3D operator+(const vec3D& l, const vec3D& r){
	vec3D vec(l.x + r.x, l.y + r.y, l.z + r.z);
	return vec;
};

inline vec3D operator-(const vec3D& l, const vec3D& r){
	vec3D vec(l.x - r.x, l.y - r.y, l.z - r.z);
	return vec;
};

inline vec3D operator*(const vec3D& l, float f){
	vec3D vec(l.x * f, l.y * f, l.z * f);
	return vec;
};


//represents a colour using floats/floats for r, g, b components
class colour3D{
public:
	colour3D(float r, float g, float b, float alpha);
	colour3D();
	float r;
	float g;
	float b;
	float alpha;
};

// returns the distance between two points

float gdistance(point3D p1, point3D p2);

//as distance, except without the square root, usefull for relative distance

float fdistance(point3D p1, point3D p2);

//takes two points, p1 = (x1, y1) and p2 = (x2, y2), and returns the vector

vec3D createVec(point3D p1, point3D p2);

//takes a point, p, and a vector, v, and returns the point (p + v)

point3D movePoint(point3D p1, vec3D v1);

// Quaternion class for camera implementation

class Quaternion {
public:
	Quaternion(float x, float y, float z, float w);
	void Normalize();
	Quaternion Conjugate();  

	float x, y, z, w;
 };

// operators for quaternion
Quaternion operator*(Quaternion& l,Quaternion& r);

Quaternion operator*(Quaternion& q, vec3D& v);

//particle superclass, uses other classes to represent particles
class particle3D {
public:
	//constructors
	particle3D();
	particle3D(float x, float y, float z);
	particle3D(point3D pos, vec3D vec, vec3D rotation, colour3D colour, float speed, float psize,float age);
	void RotateParticle();

	// internal variables
	point3D position;
	vec3D vector;
	float speed;
	vec3D rotationV;
	vec3D netSpin;
	float psize;
	colour3D colour;
	float age;

};



#endif