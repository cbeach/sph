/*************************************************************************
Name:	Casey Beach
Date:	10/13/2010
SPH: 	ogl.h

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

struct VERTICES
{
	int X;
	int Y;
	int Z;
	double U;
	double V;
};


class sph
{
	protected:

		vector <SmoothedParticle*> 	*material;
//		vector <double>			metaMesh;
		vector <GLuint>		 	*dls;
		int				particleCount;
		timer				*frameTimer;
		double				timeLastFrame;

		virtual void createDL(int, int VertexCount);

		//The Following functions were taken from
		//http://www.swiftless.com/tutorials/opengl/sphere.html
		virtual void DisplaySphere(double R, int VertexCount, VERTICES*);
		virtual VERTICES* createSphere(double radius, double x, double y, double z, int space);	
		virtual void applyForces(double timeDiff);

	public:
		sph();
		sph(int);
		sph(const sph&);
		~sph();
//		VERTECIES *VERTEX			
		virtual void display();
		virtual void setTimer(timer*);

};

#endif
