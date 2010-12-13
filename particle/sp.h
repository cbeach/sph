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
#include <stack>
#include <timer.hpp>

#include <GL/gl.h>
#include <GL/freeglut.h>
#include <GL/glut.h>

#include <util/uVect.h>

#define WATER 0



//class vector;
using namespace std;
using namespace boost;


const double ER = 3;


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
		vector  <double> 	*position;
		stack	 <int>		*neighbors;

		uVect 	*velocity;
		double 	radius;
		double 	mass;
		int 	materialID;
		timer	*frameTimer;
		double 	timeLastFrame;
		double 	forceConstant;
		bool	reversed;	//this is such a kludge... :(
		double 	density;

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
		virtual void setVelocity(double,double,double);
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
		virtual uVect* getForceAtPoint(SmoothedParticle*);
		virtual void applyForce(uVect &, double);
		virtual void updatePosition(double elapsedTime);
		virtual vector <double>* pressureKernel(vector <double>*);
		virtual vector <double>* viscosityKernel(vector <double>*);
		virtual	double densityKernel(vector <double>*);
		
		virtual void calculateDensity(SmoothedParticle*);		
		

		virtual inline void zeroDensity(){density = 0;};
		virtual inline void printDensity(){cout << "density = " << density << " " << isnan(density) << endl;};

		virtual void clearNAN()
		{
			if(isnan(density))
				density = 0;
		};

		virtual inline void pushNeighbor(int n)
		{
			neighbors->push(n);
		};
		virtual inline int popNeighbor()
		{	
			int n = 0;
			if(!neighbors->empty())
			{
				n = neighbors->top();
				neighbors->pop();
			}
			return n;
		};
		virtual inline int sizeNeighbor()
		{
			return neighbors->size();
		};

};

#endif













