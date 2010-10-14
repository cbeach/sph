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

#include <vector>


class sph
{
	protected:

		vector <SmoothedParticle*> 	*material;
//		vector <double>			metaMesh;
		vector <GLuint> 		*dls;
		
		struct VERTICES
		{
			int X;
			int Y;
			int Z;
			double U;
			double V;
		};

		virtual void createDL(int, VERTECES, int VertexCount);

		//The Following functions were taken from
		//http://www.swiftless.com/tutorials/opengl/sphere.html
		virtual void DisplaySphere(double R);
		virtual VERTICES* createSphere(double radius, double x, double y, double z, int space);	

	public:
		sph();
		sph(int);
		sph(const sph&);
		~sph();
			
		virtual void display();

};
