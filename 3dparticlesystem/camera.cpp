/*
 * Adam Bysice - 0753709
 * File: camera.cpp
 * --------------------
 * This file contains the implementation
 * for the camera class.
 */

#include "mathlib3d.h"
#include "camera.h"
#include <gl/freeglut.h>
#include <Windows.h>
#include <math.h>
#define _USE_MATH_DEFINES 
#define ToRadian(x) (float)(((x) * M_PI / 180.0f))
#define ToDegree(x) (float)(((x) * 180.0f / M_PI))

//step distance for movement
const static float step = 0.1f;
//lower sensitivity = faster turn time
const static float SENSITIVITY = 18.0f;

const static int MARGIN = 100;

// "default" constructor
Camera::Camera(int WindowWidth, int WindowHeight)
{
	m_windowWidth  = WindowWidth;
    m_windowHeight = WindowHeight;

	m_pos = vec3D(0, 0, 0);
    m_target = vec3D(0, 0, 1);
	m_target.normalize();
    m_up = vec3D(0, 1, 0);

	init();
}

// constructor with arguments
Camera::Camera(int WindowWidth, int WindowHeight,vec3D Pos, vec3D Target, vec3D Up)
{
	m_windowWidth  = WindowWidth;
    m_windowHeight = WindowHeight;
    m_pos    = Pos;
   
	m_target = Target;
	m_target.normalize();

    m_up     = Up;
    m_up.normalize();
	
	init();
}

// initializes internal class data
void Camera::init()
{
	// create horizontal target vector + normalize it
	vec3D HTarget(m_target.x, 0.0, m_target.z);
    HTarget.normalize();
    
	// check to see if z is positive or negative, then calculate horizontal vectors based on target vec
    if (HTarget.z >= 0.0f)
    {
        if (HTarget.x >= 0.0f)
            m_AngleH = 360.0f - ToDegree(asin(HTarget.z));
        else
            m_AngleH = 180.0f + ToDegree(asin(HTarget.z));
    }
    else
    {
        if (HTarget.x >= 0.0f)
            m_AngleH = ToDegree(asin(-HTarget.z));
        else
            m_AngleH = 90.0f + ToDegree(asin(-HTarget.z));
    }
    // do the same with vertical angle
    m_AngleV = -ToDegree(asin(m_target.y));

	// init constants
	m_OnUpperEdge = false;
    m_OnLowerEdge = false;
    m_OnLeftEdge  = false;
    m_OnRightEdge = false;
	keys[0] = false;
	keys[1] = false;
	keys[2] = false;
	keys[3] = false;
	m_mousePos.x  = m_windowWidth / 2;
    m_mousePos.y  = m_windowHeight / 2;

	//starts mouse in centre of the screen
    //glutWarpPointer(m_mousePos.x, m_mousePos.y);
	SetCursorPos(m_mousePos.x,m_mousePos.y);
	glutSetCursor(GLUT_CURSOR_NONE); 
	
}

// called every frame, needed for animations to display correctly when not inputting
void Camera::OnDisplay()
{
	update();
	
}

// mouse input function
void Camera::OnMouse(int x, int y)
{
	//change in mouse position
    const int DeltaX = -(x - m_mousePos.x);
    const int DeltaY = y - m_mousePos.y;

    m_mousePos.x = x;
    m_mousePos.y = y;
	
	// adjusts horizontal and vertical angle based on change in mouse position
    m_AngleH += (float)DeltaX / SENSITIVITY;
    m_AngleV += (float)DeltaY / SENSITIVITY;

	// overly complicated cases dealing with borders and Deltax/y, clean up later
	if (DeltaX == 0) {
        if (x <= MARGIN) {
            m_OnLeftEdge = true;
		    m_mousePos.x  = m_windowWidth / 2;
			m_mousePos.y  = m_windowHeight / 2;
			//glutWarpPointer(m_mousePos.x, m_mousePos.y);
			SetCursorPos(m_mousePos.x,m_mousePos.y);
        }
        else if (x >= (m_windowWidth - MARGIN)) {
            m_OnRightEdge = true;
			m_mousePos.x  = m_windowWidth / 2;
			m_mousePos.y  = m_windowHeight / 2;
			//glutWarpPointer(m_mousePos.x, m_mousePos.y);
			SetCursorPos(m_mousePos.x,m_mousePos.y);
        }
    }
    else {
        m_OnLeftEdge = false;
        m_OnRightEdge = false;
		m_mousePos.x  = m_windowWidth / 2;
		m_mousePos.y  = m_windowHeight / 2;
		//glutWarpPointer(m_mousePos.x, m_mousePos.y);
		SetCursorPos(m_mousePos.x,m_mousePos.y);
    }

    if (DeltaY == 0) {
        if (y <= MARGIN) {
            m_OnUpperEdge = true;
			m_mousePos.x  = m_windowWidth / 2;
			m_mousePos.y  = m_windowHeight / 2;
			//glutWarpPointer(m_mousePos.x, m_mousePos.y);
			SetCursorPos(m_mousePos.x,m_mousePos.y);
        }
        else if (y >= (m_windowHeight - MARGIN)) {
            m_OnLowerEdge = true;
			m_mousePos.x  = m_windowWidth / 2;
			m_mousePos.y  = m_windowHeight / 2;
			//glutWarpPointer(m_mousePos.x, m_mousePos.y);
			SetCursorPos(m_mousePos.x,m_mousePos.y);
        }
    }
    else {
        m_OnUpperEdge = false;
        m_OnLowerEdge = false;
		m_mousePos.x  = m_windowWidth / 2;
		m_mousePos.y  = m_windowHeight / 2;
		//glutWarpPointer(m_mousePos.x, m_mousePos.y);
		SetCursorPos(m_mousePos.x,m_mousePos.y);
    }

	update();
}

// onkey functions, set "pressed" to true when a key is pressed
void Camera::OnKey(unsigned char key){
	if (key == 'w') {
		keys[0] = true;
	}
	else if (key == 's') {
		keys[1] = true;
	}
	else if (key == 'd') {
		keys[2] = true;
	}
	else if (key == 'a') {
		keys[3] = true;
	}
}

//key up func, sets "pressed" to false when the key is unpressed
void Camera::OnKeyUp(unsigned char key){
	if (key == 'w') {
		keys[0] = false;
	}
	else if (key == 's') {
		keys[1] = false;
	}
	else if (key == 'd') {
		keys[2] = false;
	}
	else if (key == 'a') {
		keys[3] = false;
	}

}


// camera movement function, uses key funcs
void Camera::OnKeyboard(bool keys[])
{
	//forward
	if (keys[0] == true)
    {
		m_pos += (m_target * step);
	}
	//back
	if (keys[1] == true)
    {
        m_pos -= (m_target * step);
	}
	//right
	if(keys[2] == true)
    {
        vec3D left = m_target.cross(m_up);
        left.normalize();
        left *= step;
        m_pos += left;
     }
	//left
	if (keys[3] == true)
    {
        vec3D right = m_up.cross(m_target);
        right.normalize();
        right *= step;
        m_pos += right;
    }
}

// update the camera using gluLookAt
void Camera::CamUpdate() {
	gluLookAt(
			m_pos.x, m_pos.y,m_pos.z,
			m_target.x +m_pos.x, m_target.y + m_pos.y, m_target.z + m_pos.z,
			m_up.x, m_up.y, m_up.z
		);
	OnKeyboard(keys);
}

//updates angles and rotates camera 
void Camera::update()
{
    const vec3D Vaxis(0.0f, 1.0f, 0.0f);

    // Rotate the view vector by the horizontal angle around the vertical axis
    vec3D View(1.0f, 0.0f, 0.0f);
    View.Rotate(m_AngleH, Vaxis);
    View.normalize();

    // Rotate the view vector by the vertical angle around the horizontal axis
    vec3D Haxis = Vaxis.cross(View);
    Haxis.normalize();
    View.Rotate(m_AngleV, Haxis);
       
    m_target = View;
    m_target.normalize();

    m_up = m_target.cross(Haxis);
    m_up.normalize();
}