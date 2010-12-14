/*************************************************************************
Name:	Casey Beach
Date:	10/13/2010
SPH: 	sph.h

sph is responsible for orginization of a group of smooth particles.
*************************************************************************/

#ifndef SPH_H
#define SPH_H

#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

#include <timer.hpp>

#include <vector>

#include <particle/sp.h>

//class timer;

using namespace boost;

struct VERTICES	//I used this strct for creating sphears for my particles
{		//I have since switched to simple pixels, but might switch back again some day
	int X;
	int Y;
	int Z;
	double U;
	double V;
};


class sph
{
	protected:

		vector <SmoothedParticle*> 	*material;	//this is my vector full of particles
//		vector <double>			metaMesh;
		vector <GLuint>		 	*dls;		//this is the display list i keep for fast rendering.
								//each particle has a copy of this pointer
		int				particleCount;	
		timer				*frameTimer;	//this tracks the amount of time between frames for reasonable rendering
		double				timeLastFrame;	//the time that the last frame was rendered at

		virtual void createDL(int, int VertexCount);

		//The Following functions were taken from
		//http://www.swiftless.com/tutorials/opengl/sphere.html
		virtual void DisplaySphere(double R, int VertexCount, VERTICES*);		//depricated
		virtual VERTICES* createSphere(double radius, double x, double y, double z, int space);		//depricated
		virtual void calculateDensity();	//this runs through material finds neighboring particles and calls their calculateDensity()
		virtual void applyForces(double timeDiff);	//gets neighboring particels and calls their getForceAtPoint, applyForce...
		
	public:
		sph();
		sph(int);
		sph(int,int);
		sph(const sph&);
		~sph();
//		VERTECIES *VERTEX			
		virtual int display();
		virtual void setTimer(timer*);

};

#endif
