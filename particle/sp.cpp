/*************************************************************************
Name:	Casey Beach
Date:	10/10/2010
SPH: 	sp.cpp

The class sp represents a single smooth particle.  It is responsible for
all calculations regarding that particle (eg. getting the force from this 
particle at a certain point.
*************************************************************************/

#include <stdlib.h>
#include <time.h>

#include <iostream>
#include <vector>
#include <stack>
#include <cmath>

#include <GL/gl.h>
#include <GL/freeglut.h>
#include <GL/glut.h>

#include <particle/sp.h>
#include <util/uVect.h>

#define CONST_FORCE_CONST 1
//#define WEIGHTLESS

using namespace std;

SmoothedParticle::SmoothedParticle():radius(1),mass(1),viscosity(2.034),materialID(WATER),
forceConstant(CONST_FORCE_CONST),threshold(0.5),stretchR(1),stretchA(1),
offsetR(0),offsetA(0),maxR(100),maxA(-100)
{
	position = new vector <double> (3);
	neighbors = new stack<int>;
	velocity  = new uVect(0,0,0,1);
	color = new vector<int> (3);
}


SmoothedParticle::SmoothedParticle(const SmoothedParticle& clone):radius(1),mass(1),
viscosity(2.034),materialID(WATER),forceConstant(CONST_FORCE_CONST),threshold(0.5),stretchR(1),
stretchA(1),offsetR(0),offsetA(0),maxR(100),maxA(-100)
{
	position = new vector<double> (*clone.position);
	neighbors = new stack<int> ;
	velocity = new uVect(*clone.velocity);
	radius = clone.radius;
	mass = clone.mass;
	materialID = clone.materialID;

	color = new vector<int> (*clone.color);
	pressureScale = clone.pressureScale;
}

SmoothedParticle::~SmoothedParticle()
{
	delete position;
	delete neighbors;
	delete velocity;
	delete color;
}

void SmoothedParticle::display(double oldFrameTime)
{
	timeLastFrame = oldFrameTime;

#ifndef SCHOOL
	glPushMatrix();
//	glColor4f(1.0,1.0,1.0,0.0);
	glTranslated(position->at(0), position->at(1), position->at(2));

	if(DL != 0)
	{
		glCallList(DL);
	}
	glPopMatrix();
#endif

}



//setters
void SmoothedParticle::setPosition(double x, double y, double z)
{
	position->at(0) = x;
	position->at(1) = y;
	position->at(2) = z;


}

void SmoothedParticle::setVelocity(double i, double j, double k)
{
	
	delete velocity;
	uVect *newVelocity = new uVect(i,j,k,uVect::cart);
	velocity = newVelocity;


}
void SmoothedParticle::setRadius(double newRadius)
{
	radius = newRadius;
}
void SmoothedParticle::setMass(double newMass)
{
	mass = newMass;
}
void SmoothedParticle::setMaterialID(double newID)
{
	materialID = newID;
}
void SmoothedParticle::setColor(vector<int>* newColor)
{
	if(newColor != color)
	{
		color = newColor;
	}
}
void SmoothedParticle::setPressureScale(float newScale)
{
	pressureScale = newScale;
}

void SmoothedParticle::setDL(GLuint newDL){DL = newDL;}
void SmoothedParticle::setTimer(timer *currentTime){frameTimer = currentTime;}

//getters  ***************************************************************
vector<double>* SmoothedParticle::getPosition()
{
	vector <double> *tempV = new vector <double> (*position);
	return tempV;
}

vector<int>* SmoothedParticle::getColor()
{
	vector<int>* tempV = new vector<int> (*color);
	return tempV;

}

uVect* SmoothedParticle::getVelocity()
{
	uVect *tempU = new uVect(*velocity);
	return tempU;
}
double SmoothedParticle::getRadius(){return radius;}
double SmoothedParticle::getMass(){return mass;}
double SmoothedParticle::getMaterialID(){return materialID;}
float SmoothedParticle::getPressurescale(){return pressureScale;}
GLuint SmoothedParticle::getDL(){return DL;}

/************************************************************************/
//getForceAtPoint is the heart of this program.  This function dictates the
//interaction between particles, which is what SPH is all about.
//
//
//return value: The force that is returned by this function is the force
//		that this particle enacts on its neighbor.
//
/************************************************************************/
uVect* SmoothedParticle::getForceAtPoint(SmoothedParticle *neighbor)
{

	//all variables that are prefixed with the letter n are values that
	//belong to the neighboring particle.  This convention is used to 
	//shorten expression length.
	//eg. neighbor->position->at(0) = nPosition->at(0)
	
	vector <double> *nPosition = neighbor->position;	//do NOT delete this vector
	
	double k = .0000001;

	double distance = 0.0;

	double forceX = 0.0;
	double forceY = 0.0;
	double forceZ = 0.0;
	
	double nMass = neighbor->mass;
	double nDensity = neighbor->density;
	double nViscosity = neighbor->viscosity;

	double pressure = .1; //+ k*(density - nDensity);
	double nPressure = .1; //+ k*(nDensity - density);

	vector <double> diffVector(3);

	//the vector representation of the distance between these
	//two points
	diffVector.at(0) = nPosition->at(0) - position->at(0);
	diffVector.at(1) = nPosition->at(1) - position->at(1);
	diffVector.at(2) = nPosition->at(2) - position->at(2);

	vector <double> *pressureKernelValue = pressureKernel(&diffVector);
	vector <double> *viscosityKernelValue = viscosityKernel(&diffVector);

	//this is the distance between the two particles
	distance = sqrt(diffVector.at(0)*diffVector.at(0) +
			diffVector.at(1)*diffVector.at(1) + 
			diffVector.at(2)*diffVector.at(2));

//	force = -(((distance-2)*(distance - 2)*(distance - 2)) - (distance - 2)*4);
	if(nDensity != 0)
	{
		//force due to pressure
		forceX = -1.0 * nMass * ((nPressure + pressure)/(2*nDensity)) * densityKernel(nPosition);//pressureKernelValue->at(0);
		forceY = -1.0 * nMass * ((nPressure + pressure)/(2*nDensity)) * densityKernel(nPosition);//pressureKernelValue->at(1);
		forceZ = -1.0 * nMass * ((nPressure + pressure)/(2*nDensity)) * densityKernel(nPosition);//;pressureKernelValue->at(2);

		//force due to viscosity
		forceX += viscosity * nMass * ((viscosity * nViscosity)/nDensity) * viscosityKernelValue->at(0);
		forceY += viscosity * nMass * ((viscosity * nViscosity)/nDensity) * viscosityKernelValue->at(1);
		forceZ += viscosity * nMass * ((viscosity * nViscosity)/nDensity) * viscosityKernelValue->at(2);
		
		//cout << forceX << endl;
		
	}
	else
	{
		forceX = forceY = forceZ = 0;
	}

	

	//making the return vector
	if(distance != 0)
	{
		//sanity check
		if(forceX > 1)
			forceX = 1;
		if(forceY > 1)
			forceY = 1;
		if(forceZ > 1)
			forceZ = 1;

		uVect *tempUVect = new uVect(forceX, forceY, forceZ, 1);

		delete pressureKernelValue;
		delete viscosityKernelValue;
		return tempUVect;
	}

	delete pressureKernelValue;
	delete viscosityKernelValue;
	return NULL;
}

//this function takes a force, and applies it to the velocity of the particle

void SmoothedParticle::applyForce(uVect &actingForce, double elapsedTime)
{
	vector <double> *force = actingForce.getCartesian();
	vector <double> *vel = velocity->getCartesian();

	vel->at(0) += (force->at(0) / mass) * elapsedTime;
	vel->at(1) += (force->at(1) / mass) * elapsedTime;
	vel->at(2) += (force->at(2) / mass) * elapsedTime;

	if(velocity)
		delete velocity;
	
	velocity = new uVect(vel->at(0), vel->at(1), vel->at(2), 1);
	delete force;
	delete vel;
}

//this is called after all of the paricles have interacted
//in this time step.  This moves the particles according to
//their velocity and how much time has elapsed.

void SmoothedParticle::updatePosition(double elapsedTime)
{
	vector <double> *vel = velocity->getCartesian();
	
	#ifndef WEIGHTLESS
	vel->at(2) += -9.8 * elapsedTime;
	#endif
	
	position->at(0) += vel->at(0) * elapsedTime;	
	position->at(1) += vel->at(1) * elapsedTime;	
	position->at(2) += vel->at(2) * elapsedTime;	
	
	//cout << position->at(0) << endl;

	if(position->at(2) < 0)
	{
		position->at(2) -= vel->at(2) * elapsedTime * 2;
		vel->at(2) *= -.2;

	}

	if(velocity)
		delete velocity;
	velocity = new uVect(vel->at(0), vel->at(1), vel->at(2), uVect::cart);
	
	delete vel;
}

//The following three kernel functions are used in th egetForceatPoint
//function

vector <double>* SmoothedParticle::pressureKernel(vector <double> *r)
{
	vector <double> *tempVect = new vector <double> (3);
	
	double mag = 	sqrt(r->at(0)*r->at(0)+
			r->at(1)*r->at(1)+
			r->at(2)*r->at(2));

	tempVect->at(0) = (45.0/(PI*ER*
		ER*ER*ER*
		ER*ER)) * ((ER*mag)*
		(ER*mag)*(ER*mag)) * (r->at(0)/mag);
				
	tempVect->at(1) = (45.0/(PI*ER*ER*
		ER*ER*ER*ER)) *
	 	((ER*mag)*(ER*mag)*
		(ER*mag)) * (r->at(1)/mag);

	tempVect->at(2) = (45.0/(PI*ER*ER*
		ER*ER*ER*ER)) * 
		((ER*mag)*(ER*mag)*
		(ER*mag)) * (r->at(2)/mag);


	return tempVect;
}

vector <double>* SmoothedParticle::viscosityKernel(vector <double> *r)
{
	vector <double> *tempVect = new vector <double> (3);

	double mag = 	sqrt(r->at(0)*r->at(0)+
			r->at(1)*r->at(1)+
			r->at(2)*r->at(2));

	tempVect->at(0) = (45.0/(PI*ER*ER*ER*ER*ER*ER)) * (ER*mag);
				
	tempVect->at(1) = (45.0/(PI*ER*ER*ER*ER*ER*ER)) * (ER*mag);

	tempVect->at(2) = (45.0/(PI*ER*ER*ER*ER*ER*ER)) * (ER*mag);


	return tempVect;
}

double SmoothedParticle::densityKernel(vector <double> *r)
{
	double mag = 	sqrt(abs(r->at(0)*r->at(0)+
			r->at(1)*r->at(1)+
			r->at(2)*r->at(2)));
	
	return ((315.0)/(64 * PI * ER*ER*ER*ER*ER*ER*ER*ER*ER))*
		(ER*ER - mag*mag)*(ER*ER - mag*mag)*(ER*ER - mag*mag); 

}

//this is the first thing that is done to this particle
//the density is used in several force calculations.
//Since this particle is a member of a greater system
//the density that this particle represents should be
//"smoothed" over the surrounding area by taking a 
//weighted average of its neighbors.
void SmoothedParticle::calculateDensity(SmoothedParticle *neighbor)
{
	if(neighbor)
	{
		density += neighbor->mass * densityKernel(neighbor->position);
	}
}

//gives the particle's velocity a random kick
void SmoothedParticle::perterb()
{
	srand(timer(NULL));

}





















