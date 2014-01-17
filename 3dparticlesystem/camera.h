/*
************************FPS CAMERA CLASS*****************************************
*			Adam Bysice - 2013 - do not use without my permission				*
************************* Usage: ************************************************ 
* 1. Initialize as a ptr to null at startup, i.e. Camera* myCam = NULL;			*
* 2. After starting glut use a constructor, i.e. myCam = new Camera(WindowWidth, WindowHeight, eyepos,lookpos,up);
* 3. call Onkey() in glutKeyboardFunc, i.e. myCam->OnKeyboard(key);				*
* 4. call OnKeyUp() in glutKeyboardUpFunc, i.e. myCam->OnKeyUp(key);			*
* 5. call OnMouse() in a mouse functioglutPassiveMotionFunc, i.e. myCam->OnMouse(x,y);
* 6. call OnDisplay() in glutIdleFunc, i.e. myCam->OnDisplay();					*
* 7. call CamUpdate() in glutDisplayFunc i.e. myCam->CamUpdate();				*
* NOTE: be sure to push + pop around the camUpdate();							*
*********************************************************************************
*/


#ifndef _camera_h
#define _camera_h

#include "mathlib3d.h"
#include <gl/freeglut.h>

class Camera
{
public:
    Camera(int WindowWidth, int WindowHeight);
    Camera(int WindowWidth, int WindowHeight, vec3D Pos, vec3D Target, vec3D Up);
	void CamUpdate();
	void OnKey(unsigned char key);
	void OnKeyUp(unsigned char key);
	void OnMouse(int x, int y);
	void OnDisplay();

	vec3D m_pos;
    vec3D m_target;
    vec3D m_up;

private:
	void init();
	void update();
	void OnKeyboard(bool keys[]);

	int m_windowWidth;
    int m_windowHeight;
	float m_AngleH;
    float m_AngleV;

	bool keys[4];
    bool m_OnUpperEdge;
    bool m_OnLowerEdge;
    bool m_OnLeftEdge;
    bool m_OnRightEdge;

	point2Di m_mousePos;
};

#endif