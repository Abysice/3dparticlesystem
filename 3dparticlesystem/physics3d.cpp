/*
 * Adam Bysice - 0753709
 * File: mathlib2d.cpp
 * --------------------
 * This file contains the implementation
 * for the mathlib2D classes.
 */

#include <math.h>
#include "mathlib3d.h"

//using references from Thomas Smid's implementation of collision in c++
void collision3D(point3D p1, point3D p2, vec3D& v1, vec3D& v2)     {

	float  pi,r12,m21,d,v,theta2,phi2,st,ct,sp,cp,vx1r,vy1r,vz1r,fvz1r,
			thetav,phiv,dr,alpha,beta,sbeta,cbeta,dc,sqs,t,a,dvz2,
			vx2r,vy2r,vz2r,x21,y21,z21,vx21,vy21,vz21,vx_cm,vy_cm,vz_cm;
	float R = 1;
	float m1 =1;
	float m2 =1;
	float r1 =0.05f;
	float r2 =0.05f;
	int error = 0;
	
	// initialize some variables 
       pi=acos(-1.0E0);
       r12=r1+r2;
       m21=m2/m1;
	
	   // positions
	   x21=p2.x-p1.x;
       y21=p2.x-p1.y;
       z21=p2.x-p1.z;
	// vectors
       vx21=v2.x-v1.x;
       vy21=v2.y-v1.y;
       vz21=v2.z-v1.z;
       
       vx_cm = (m1*v1.x+m2*v2.x)/(m1+m2) ;
       vy_cm = (m1*v1.y+m2*v2.y)/(m1+m2) ;
	   vz_cm = (m1*v1.z+m2*v2.z)/(m1+m2) ;  

	   
	// calculate relative distance and relative speed
       d=sqrt(x21*x21 +y21*y21 +z21*z21);
       v=sqrt(vx21*vx21 +vy21*vy21 +vz21*vz21);
  
	//shift coordinate system so that ball 1 is at the origin
       p2.x=x21;
	   p2.y=y21;
       p2.z=z21;
       
	// boost coordinate system so that ball 2 is resting
	   v1.x=-vx21;
	   v1.y=-vy21;
	   v1.z=-vz21;

	//find the polar coordinates of the location of ball 2
	   theta2=acos(p2.z/d);
	   if (p2.x==0 && p2.y==0) {phi2=0;} else {phi2=atan2(p2.y,p1.x);}
       st=sin(theta2);
       ct=cos(theta2);
       sp=sin(phi2);
       cp=cos(phi2);


	//express the velocity vector of ball 1 in a rotated coordinate
	//system where ball 2 lies on the z-axis
       vx1r=ct*cp*v1.x+ct*sp*v1.y-st*v1.z;
       vy1r=cp*v1.y-sp*v1.x;
       vz1r=st*cp*v1.x+st*sp*v1.y+ct*v1.z;
       fvz1r = vz1r/v ;
       if (fvz1r>1) {fvz1r=1;}   // fix for possible rounding errors
          else if (fvz1r<-1) {fvz1r=-1;} 
       thetav=acos(fvz1r);
       if (vx1r==0 && vy1r==0) {phiv=0;} else {phiv=atan2(vy1r,vx1r);}

        						
	// calculate the normalized impact parameter
       dr=d*sin(thetav)/r12;


	//return old positions and velocities if balls do not collide
       if (thetav>pi/2 || fabs(dr)>1) {
           p2.x=p2.x+p1.x;
           p2.y=p2.y+p1.y;
           p2.z=p2.z+p1.z;
           v1.x=v1.x+v2.x;
           v1.y=v1.y+v2.y;
           v1.z=v1.z+v2.z;
           error=1;
           return;
        }
       
	//calculate impact angles if balls do collide
       alpha=asin(-dr);
       beta=phiv;
       sbeta=sin(beta);
       cbeta=cos(beta);
        
       
	// calculate time to collision 
       t=(d*cos(thetav) -r12*sqrt(1-dr*dr))/v;
     
	// update positions and reverse the coordinate shift 
       p2.x=p2.x+v2.x*t +p1.x;
       p2.y=p2.y+v2.y*t +p1.y;
       p2.z=p2.z+v2.z*t +p1.z;
	   p1.x=(v1.x+v2.x)*t +p1.x;
	   p1.y=(v1.y+v2.y)*t +p1.y;
	   p1.z=(v1.z+v2.z)*t +p1.z;
               
	// update velocities 
	   a=tan(thetav+alpha);

       dvz2=2*(vz1r+a*(cbeta*vx1r+sbeta*vy1r))/((1+a*a)*(1+m21));
       
       vz2r=dvz2;
       vx2r=a*cbeta*dvz2;
       vy2r=a*sbeta*dvz2;
       vz1r=vz1r-m21*vz2r;
       vx1r=vx1r-m21*vx2r;
       vy1r=vy1r-m21*vy2r;

       
	// rotate the velocity vectors back and add the initial velocity
	// vector of ball 2 to retrieve the original coordinate system 
                     
	   v1.x=ct*cp*vx1r-sp*vy1r+st*cp*vz1r +v2.x;
	   v1.y=ct*sp*vx1r+cp*vy1r+st*sp*vz1r +v2.y;
	   v1.z=ct*vz1r-st*vx1r               +v2.z;
	   v2.x=ct*cp*vx2r-sp*vy2r+st*cp*vz2r +v2.x;
	   v2.y=ct*sp*vx2r+cp*vy2r+st*sp*vz2r +v2.y;
	   v2.z=ct*vz2r-st*vx2r               +v2.z;
        

	// velocity correction for inelastic collisions

       v1.x=(v1.x-vx_cm)*R + vx_cm;
       v1.y=(v1.y-vy_cm)*R + vy_cm;
	   v1.z=(v1.z-vz_cm)*R + vz_cm;
	   v2.x=(v2.x-vx_cm)*R + vx_cm;
	   v2.y=(v2.y-vy_cm)*R + vy_cm;
	   v2.z=(v2.z-vz_cm)*R + vz_cm;  

       return;

}
/*
void collision2D (point2D p1, point2D p2, vec2D *v1, vec2D *v2) {
	double  m21, dvx2, a, x21, y21, vx21, vy21, fy21, sign, vx_cm, vy_cm, R;
	//elasticity constant testing use only
	R = 1;

	m21=(1);
    x21=p2.getX()-p1.getY();
    y21=p2.getY()-p1.getY();
    vx21=v2->getX()-v1->getX();
    vy21=v2->getY()-v1->getY();
	vx_cm = (1*v1->getX()+1*v2->getX())/(1+1);
	vy_cm = (1*v1->getY()+1*v2->getY())/(1+1);   
	//return old velocities if not approaching
	if ( (vx21*x21 + vy21*y21) >= 0) return;
	//the following statements to avoid a zero divide; 
	fy21 = 1.0E-12*fabs(y21);
	if ( fabs(x21) < fy21 ) {  
		if (x21 < 0) { 
			sign = -1;
		}
		else {
			sign = 1;
			}
		x21 = fy21*sign; 
	} 
	//update velocities
	a = y21/x21;
	dvx2 = -2*(vx21 +a*vy21)/((1+a*a)*(1+m21)) ;
	v2->setX(v2->getX()+dvx2);
	v2->setY(v2->getY()+a*dvx2);
	v1->setX(v1->getX()-m21*dvx2);
	v1->setY(v1->getY()-a*m21*dvx2);	
	//velocity correction for inelastic collisions 
	//Still being tested
	v1->setX((v1->getX()-vx_cm)*R + vx_cm);
	v1->setY((v1->getY()-vy_cm)*R + vy_cm);
	v2->setX((v2->getX()-vx_cm)*R + vx_cm);
	v2->setY((v2->getY()-vy_cm)*R + vy_cm);
}
*/
//returns greater psize

float greater (float s1, float s2) {
	if (s1 > s2)
		return s1;
	else
		return s2;
}

#include <math.h>
#include "mathlib3d.h"