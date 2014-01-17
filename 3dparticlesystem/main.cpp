/* 
 * Adam Bysice - 0753709
 * File: main.cpp
 * ---------------------------
 * main program file, contains
 * most of the implementation
 */

#include <gl/freeglut.h>
#include <gl/gl.h>
#include "mathlib3d.h"
#include "physics3d.h"
#include "camera.h"
#include <time.h>
#include <random>
//#include "sixense.h"
//#include <png.h>

//function prototypes
void drawAxis();
void initializeCallbacks();
void pMove(int unused);

//constants
const int WindowWidth = 600;
const int WindowHeight = 400;
const int MAX_PARTICLES = 3000;
const int BOUNDRY = 10;

//global variables
Camera* myCam = NULL;
particle3D* pList[MAX_PARTICLES];
int currentSize = 0;
int maxSize = MAX_PARTICLES;
float friction = 1;
float gravity = 0.05;
bool pause = false;
bool collision = false;
int rotate = 0;
float radius = 0.05;

// display funtion
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glMatrixMode(GL_PROJECTION);
	
	glPushMatrix();
		
		// call to update camera class look at
		myCam->CamUpdate();
		glRotatef(rotate,0,1,0);
		//draw plane
		glPushMatrix();
			
			drawAxis();
		glPopMatrix();

		// draw particles if there are any
		if (currentSize > 0) {
			for(int i = 0; i < currentSize; i++) {
				glPushMatrix();
					glTranslatef(pList[i]->position.x, pList[i]->position.y, pList[i]->position.z);
					glRotatef(pList[i]->netSpin.x,1.0,0.0,0.0);
					glRotatef(pList[i]->netSpin.y,0.0,1.0,0.0);
					glRotatef(pList[i]->netSpin.z,0.0,0.0,1.0);
					glColor3d(pList[i]->colour.r, pList[i]->colour.g,pList[i]->colour.b);
					glutSolidSphere(pList[i]->psize,6,6);
				glPopMatrix();
			}
		}
	glPopMatrix();
	
	
	glutSwapBuffers();
	glutPostRedisplay();
}

//keyboard input function
void kbd(unsigned char key, int x, int y)
{
	// pass key commands to camera class
	myCam->OnKey(key);

	//quit if q is pressed
	if (key == 'q') {
		exit(0);
	}
	//pause program
	else if (key == 32) {
		pause = !pause;
	}
	//resets particles
	else if (key == 'r') {
		for(int i = 0; i < currentSize; i++) {
			pList[i] = NULL;
				
		}	
		currentSize = 0;
		friction = 1;
		gravity = 0.05;
		//collision = false;
	}
	else if (key == 'f') {
		//increase friction
		if (friction > 0)
			friction -= 0.1;
	}
	else if (key == 'g') {
		//increase gravity
		gravity += 0.01;
	}
	else if (key == 'c') {
		//enable collision EXPERIMENTAL
		if (collision == false) {
			maxSize = 200;
			for(int i = 0; i < currentSize; i++) {
				pList[i] = NULL;
			}	
			currentSize = 0;
			friction = 1;
			gravity = 0.05;
			radius *= 5;
		}
		else {
			maxSize = MAX_PARTICLES;
			for(int i = 0; i < currentSize; i++) {
				pList[i] = NULL;
			}
			currentSize = 0;
			friction = 1;
			gravity = 0.05;
			radius = 0.05;
		}

		collision = !collision;
	}

}

void speckbd(int key, int x, int y){
	// rotate scene
	if (key == GLUT_KEY_LEFT) {
		rotate += 5;
	}
	else if (key == GLUT_KEY_RIGHT) {
		rotate -= 5;
	}
}

// keyboard "upkey" function, used in camera class for smooth movement
void ukbd(unsigned char key, int x, int y)
{
	//pass key up to camera class
	myCam->OnKeyUp(key);
}

// mouse func, tracks mouse position when no button is pressed
void pmouse(int x, int y)
{
	// pass mouse position to camera class
	myCam->OnMouse(x,y);
}
// runs when doing nothing else
void idle() {
	//do camera calculations
	myCam->OnDisplay();
	// randomly generate particles when nothing else is happening
	if (!pause && (currentSize < maxSize)) {
		currentSize = currentSize + 1;
		point3D position(0,0,0);
		colour3D colour(0+(1-0)*rand()/((float)RAND_MAX),0+(1-0)*rand()/((float)RAND_MAX), 0+(1-0)*rand()/((float)RAND_MAX), 1);
		vec3D vector(((double)rand())/((double)RAND_MAX)/5.0-0.1,((double)rand())/((double)RAND_MAX)/2.0+0.5,((double)rand())/((double)RAND_MAX)/5.0-0.1);
		vector.normalize(); 
		vec3D rotation(rand()%(3), rand()%(3),rand()%(3));
		pList[currentSize-1] = new particle3D(position, vector, rotation, colour, rand()%(2),radius,rand()%(21-10)+10);
	}

}

//particle position updater function, runs on a timer
void pMove(int unused) {
	if (currentSize > 0) {
		for(int i = 0; i < currentSize; i++) {
			// temp = where the particle is moving next
			point3D temp = movePoint(pList[i]->position, pList[i]->vector);
			// if position of temp is below plane and vec is negative, and within bounds, bounce the ball
			if (temp.y <= 0 && pList[i]->vector.y <= 0 && abs(pList[i]->position.x) <= BOUNDRY && abs(pList[i]->position.z) <= BOUNDRY) {
				pList[i]->vector.y *= -friction;
			}
			//if position of temp is below the plane and within bounds and very small vec, set vec to zero
			if (temp.y <= 0 && abs(pList[i]->vector.y) <= 0.1 && abs(pList[i]->position.x) <= BOUNDRY && abs(pList[i]->position.z) <= BOUNDRY) {
				//ball has stopped moving within square
				pList[i]->vector.y = 0;
				temp.y = 0;
			
			}
			//if position is above the plane or outside the bounds, apply gravity 
			if (temp.y > 0 || abs(temp.x) >= BOUNDRY || abs(temp.z) >= BOUNDRY){
				pList[i]->vector.y -= gravity;
			}
			
			if (collision && (pList[i]->vector.magnitude > 0) && (currentSize > 0)){
				for (int j = 0; j < currentSize; j++) {
					// check if temp is already occupied by another box
					if (gdistance(pList[j]->position, temp) <= (radius*2) ) {
						//preform a collision calculation with their two vectors
						collision3D(temp, pList[j]->position, pList[i]->vector, pList[j]->vector);
					}
				}
			}
			
			//reduce particle age and update position
			pList[i]->age -= 0.01f;
			pList[i]->position.x = temp.x;
			pList[i]->position.y = temp.y;
			pList[i]->position.z = temp.z;
			pList[i]->RotateParticle();
		}
		//remove particles with age less than 0, 
		for(int i = 0; i < currentSize; i++) {
			if (pList[i]->age <= 0 || pList[i]->position.y <= -10) {
				for(int j = i; i < currentSize; i++) {
					pList[i] = pList[i+1];
				
				}
				currentSize = currentSize-1;
			}	
		}
	}
		glutTimerFunc(40, pMove, 0);
}

// main, program entry point
int main(int argc, char** argv)
{
	// set the window size, display mode, and create the window
	glutInit(&argc, argv);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(WindowWidth, WindowHeight);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GL_DOUBLEBUFFER | GL_DEPTH_BUFFER_BIT);
	glutCreateWindow("Brokeneye");


	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,2,1,200);
	glClearColor(0.5, 0.5, 0.5, 0);
	glMatrixMode(GL_MODELVIEW);
	//culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	//register glut callbacks for keyboard and display function
	initializeCallbacks();

	//start the program!
	glutMainLoop();

	return 0;
}

// initializes the glut callbacks + camera stuff
void initializeCallbacks(){
	
	glutKeyboardFunc(kbd);
	glutSpecialFunc(speckbd);
	glutPassiveMotionFunc(pmouse);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardUpFunc(ukbd);
	glutIgnoreKeyRepeat(TRUE);
	glutTimerFunc(100, pMove, 0);

	printf("Key w/s:		move camera forward/back \n");
	printf("Key a/d:		move camera left/right \n");
	printf("Mouse:			FPS-style camera control \n");
	printf("Spacebar:		pause program \n");
	printf("Key 'r':		reset program (resets gravity/friction) \n");
	printf("Key 'q':		quits program \n");
	printf("Key 'f':		increases friction by set ammount \n");
	printf("Key 'g':		increases gravity by set ammount \n");
	printf("Key 'c':		enable 3D elastic collision 2: electric boogaloo, may or may not be working\n");
	
	
	//initialize camera
	vec3D eyepos(-7,2,-7);
	vec3D lookpos(1,0,1);
	vec3D up(0,1,0);
	myCam = new Camera(WindowWidth, WindowHeight, eyepos,lookpos,up);

	// initialize particles
	srand(time(NULL)); // Seed the time

	//enable Razer Hydra
	//sixenseInit();

}
// draw planes
void drawAxis()
{
	glBegin(GL_QUADS);
		glColor3f(0.1,0.1,0.1);
		glVertex3f(-BOUNDRY,-0.05,-BOUNDRY);
		glVertex3f(-BOUNDRY,-0.05,BOUNDRY);
		glVertex3f(BOUNDRY,-0.05,BOUNDRY);
		glVertex3f(BOUNDRY,-0.05,-BOUNDRY);
	glEnd();
}
