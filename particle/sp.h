/*************************************************************************
Name:	Casey Beach
Date:	10/10/2010
SPH: 	ogl.h

The class sp represents a single smooth particle.  It is responsible for
all calculations regarding that particle (eg. getting the force from this 
particle at a certain point.
*************************************************************************/

#ifndef SP_H
#define SP_H

#include <iostream>
#include <vector>

#include <GL/gl.h>
#include <GL/freeglut.h>
#include <GL/glut.h>

#include <util/uVect.h>

#define WATER 0



//class vector;
using namespace std;

struct BindingPoint
{
	vector <double> points;
//	vector <SmoothedParticle*> bound;

};


class SmoothedParticle
{
	protected:
		//variables that are needed for basic functioning
		//physical properties
		vector <double> *position;
		vector <SmoothedParticle*>	*neighbors;
		uVect 	*velocity;
		double 	radius;
		double 	mass;
		int 	materialID;
		GLuint	DL;

		//The color and shape
		vector <int>	*color;
		float	pressureScale;
		GLuint	sphereDL;

		//These variables are used for ray tracing
//		double alpha;
//		double indexOfRefraction;
//		double reflectivity;

		//Thes variables are used for Crystalization
//		vector <BindingPoints> 	boundParticles;
//		vector <double		angularVelocity;
//		uVect	orientation;
//		bool	isBound;


	public:
		SmoothedParticle();
		SmoothedParticle(const SmoothedParticle&);
		~SmoothedParticle();
		
		//setters
		virtual void setPostion(vector<double>*);
		virtual void setVelocity(uVect*);
		virtual void setRadius(double);
		virtual void setMass(double);
		virtual void setMaterialID(double);
		virtual void setColor(vector<int>*);
		virtual void setPressureScale(float);
		virtual void setDL(GLuint);

		//getters
		virtual vector<double>* getPostition();
		virtual uVect* getVelocity();
		virtual double getRadius();
		virtual double getMass();
		virtual double getMaterialID();
		virtual vector<int>* getColor();
		virtual float getPressurescale();
		virtual GLuint getDL();

		virtual void display();
};

#endif
