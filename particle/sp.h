/*************************************************************************
Name:	Casey Beach
Date:	10/10/2010
SPH: 	sp.h

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


const double ER = .005; 


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
		vector  <double> 	*position;
		stack	 <int>		*neighbors;

		uVect 	*velocity;
		double 	radius;
		double 	mass;
		double 	viscosity;
		int 	materialID;	//this will later be used to tell different fluids apart (eg. oil and water)
		timer	*frameTimer;
		double 	timeLastFrame;
		double 	forceConstant;
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
		virtual uVect* getForceAtPoint(SmoothedParticle*);	//this is the biggest deal in this program
		virtual void applyForce(uVect &, double);		//apply the forces to the velocity
		virtual void updatePosition(double elapsedTime);	//apply the velocity to the position
		
		virtual vector <double>* pressureKernel(vector <double>*);	//smoothing kernel functions used in the getForceAtPoint function
		virtual vector <double>* viscosityKernel(vector <double>*);
		virtual	double densityKernel(vector <double>*);
		
		virtual void calculateDensity(SmoothedParticle*);		//used to calculate the pressure force
		

		virtual inline void zeroDensity(){density = mass/(radius*radius*PI);};	//this is used after the frame is over and the current density is no longer needed
		virtual inline void printDensity(){cout << "density = " << density << " " << isnan(density) << endl;};

		virtual void clearNAN()	//a very kludgey solution to a nan problem I was having in the density calculation.
		{			//finding the root cause of this is on my list of things to do.
			if(isnan(density))
				density = 0;
		};
};

#endif













