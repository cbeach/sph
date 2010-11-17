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
#include <timer.hpp>

#include <GL/gl.h>
#include <GL/freeglut.h>
#include <GL/glut.h>

#include <util/uVect.h>

#define WATER 0



//class vector;
using namespace std;
using namespace boost;

struct BindingPoint
{
	vector <double> points;
//	vector <SmoothedParticle*> bound;

};


class SmoothedParticle
{
	protected:
		
		double constForceConst;
	
		//variables that are needed for basic functioning
		//physical properties
		vector <double> *position;
		vector <SmoothedParticle*>	*neighbors;
		uVect 	*velocity;
		double 	radius;
		double 	mass;
		int 	materialID;
		timer	*frameTimer;
		double 	timeLastFrame;
		double 	forceConstant;

		double threshold;	//thresHold distance between attractive and repulsive
		double stretchR;	//stretctes the attractive force curve
		double stretchA;	//stretches the repulsive force curve
		double offsetR;		//the x-axis translation for the repulsive curve
		double offsetA;		//the x-axis translation for the attractive curve
		double maxR;		//Maximum scalar value for the repulsive force (x^2)
		double maxA;		//maximum scalar value for the attractive force (x^2)

		GLuint	DL;		//this is the pointer to the SP's display list

		//The color and shape
		vector <int>	*color;	//the color of the smooth particle
		float	pressureScale;	//the rate at which the particle turns color
//		GLuint	sphereDL;

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
		virtual void setPosition(double,double,double);
		virtual void setVelocity(uVect*);
		virtual void setRadius(double);
		virtual void setMass(double);
		virtual void setMaterialID(double);
		virtual void setColor(vector<int>*);
		virtual void setPressureScale(float);
		virtual void setDL(GLuint);
		virtual void setTimer(timer *currentTime);

		//getters
		virtual vector<double>* getPosition();
		virtual uVect* getVelocity();
		virtual double getRadius();
		virtual double getMass();
		virtual double getMaterialID();
		virtual vector<int>* getColor();
		virtual float getPressurescale();
		virtual GLuint getDL();
		
		virtual void display(double);
		virtual uVect* getForceAtPoint(double,double,double);
		virtual vector <double>* applyForce(uVect &, double);
		virtual void updatePosition(double elapsedTime);
				
};

#endif
