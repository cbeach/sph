/*************************************************************************
Name:	Casey Beach
Date:	10/10/2010
SPH: 	sp.cpp

The class sp represents a single smooth particle.  It is responsible for
all calculations regarding that particle (eg. getting the force from this 
particle at a certain point.
*************************************************************************/


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

SmoothedParticle::SmoothedParticle():radius(1),mass(1),materialID(WATER),
forceConstant(CONST_FORCE_CONST),threshold(0.5),stretchR(1),stretchA(1),
offsetR(0),offsetA(0),maxR(100),maxA(-100),reversed(false)
{
	position = new vector <double> (3);
	neighbors = new stack<int>;
	velocity  = new uVect(0,0,0,1);
	color = new vector<int> (3);
}


SmoothedParticle::SmoothedParticle(const SmoothedParticle& clone):radius(1),mass(1),
materialID(WATER),forceConstant(CONST_FORCE_CONST),threshold(0.5),stretchR(1),
stretchA(1),offsetR(0),offsetA(0),maxR(100),maxA(-100),reversed(false)
{
	position = new vector<double> (*clone.position);
	neighbors = new stack<int> ;
	velocity = new uVect(*clone.velocity);
	radius = clone.radius;
	mass = clone.mass;
	materialID = clone.materialID;

	color = new vector<int> (*clone.color);
	pressureScale = clone.pressureScale;
//	sphereDL = have to do extra things to this.



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

uVect* SmoothedParticle::getForceAtPoint(SmoothedParticle *neighbor)
{

	//all variables that are prefixed with the letter n are values that
	//belong to the neighboring particle.  This convention is used to 
	//shorten expression length.
	//eg. neighbor->position->at(0) = nPosition->at(0)
	
	vector <double> *nPosition = neighbor->position;	//do NOT delete this vector
	
	double k = 5.0;

	double distance = 0.0;
//	double force = 0;

	double forceX = 0.0;
	double forceY = 0.0;
	double forceZ = 0.0;
	
	double nMass = neighbor->mass;
	double nRadius = neighbor->radius;
	double nDensity = neighbor->density;

	double pressure = 5.0 + k*(density - nDensity);
	double nPressure = 5.0 + k*(nDensity - density);

	vector <double> diffVector(3);

	diffVector.at(0) = nPosition->at(0) - position->at(0);
	diffVector.at(1) = nPosition->at(1) - position->at(1);
	diffVector.at(2) = nPosition->at(2) - position->at(2);

	vector <double> *pressureKernelValue = pressureKernel(&diffVector);

	distance = sqrt(diffVector.at(0)*diffVector.at(0) +
			diffVector.at(1)*diffVector.at(1) + 
			diffVector.at(2)*diffVector.at(2));

//	force = -(((distance-2)*(distance - 2)*(distance - 2)) - (distance - 2)*4);
	if(nDensity != 0)
	{
		forceX = -1.0 * nMass * ((nPressure + pressure)/(2*nDensity)) * pressureKernelValue->at(0);
		forceY = -1.0 * nMass * ((nPressure + pressure)/(2*nDensity)) * pressureKernelValue->at(1);
		forceZ = -1.0 * nMass * ((nPressure + pressure)/(2*nDensity)) * pressureKernelValue->at(2);
	}
	else
	{
		forceX = forceY = forceZ = 0;
	}

	if(distance != 0)
	{
		double xComponent = ((1.0) * diffVector.at(0) / distance) * forceX * 5.0;
		double yComponent = ((1.0) * diffVector.at(1) / distance) * forceY * 5.0;
		double zComponent = ((1.0) * diffVector.at(2) / distance) * forceZ * 5.0;
	
		uVect *tempUVect = new uVect(xComponent, yComponent, zComponent, 1);

		delete pressureKernelValue;

		return tempUVect;
	}

	delete pressureKernelValue;
	return NULL;
}

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

void SmoothedParticle::updatePosition(double elapsedTime)
{
	vector <double> *vel = velocity->getCartesian();
	
	#ifndef WEIGHTLESS
	vel->at(2) += -9.8 * elapsedTime;
	#endif
	
	position->at(0) += vel->at(0) * elapsedTime;	
	position->at(1) += vel->at(1) * elapsedTime;	
	position->at(2) += vel->at(2) * elapsedTime;	
	
	if(position->at(2) < 0)
	{
		position->at(2) -= vel->at(2) * elapsedTime * 2;
		vel->at(2) *= -.25;

	}

	if(velocity)
		delete velocity;
	velocity = new uVect(vel->at(0), vel->at(1), vel->at(2), uVect::cart);

/*this is supposed to be a box, but for some reason particles keep escaping	
	if(position->at(0) > 20 || position->at(0) < 0 || 
	   position->at(1) > 20 || position->at(1) < 0 || 
	   position->at(2) > 20 || position->at(2) < 0)
	{
		if(!reversed)
		{
			velocity->invert();
			reversed = true;
		}
		
	}else{

		reversed = false;

	}
*/
	


	
	delete vel;
}

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




void SmoothedParticle::calculateDensity(SmoothedParticle *neighbor)
{
	if(neighbor)
	{
		density += neighbor->mass * densityKernel(neighbor->position);
	}
}























