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
#include <cmath>

#include <GL/gl.h>
#include <GL/freeglut.h>
#include <GL/glut.h>

#include <particle/sp.h>
#include <util/uVect.h>

#define CONST_FORCE_CONST 1


SmoothedParticle::SmoothedParticle():radius(1),mass(1),materialID(WATER),
threshold(0.5),stretchR(1),stretchA(1),offsetR(0),offsetA(0),maxR(100),
maxA(-100),forceConstant(CONST_FORCE_CONST)
{
	position = new vector <double> (3);
	neighbors = new vector<SmoothedParticle*> (10);
	velocity  = new uVect(0,0,0,1);
	color = new vector<int> (3);
}


SmoothedParticle::SmoothedParticle(const SmoothedParticle& clone):radius(1),mass(1),
materialID(WATER),threshold(0.5),stretchR(1),stretchA(1),offsetR(0),offsetA(0),
maxR(100),maxA(-100),forceConstant(CONST_FORCE_CONST)
{
	position = new vector<double> (*clone.position);
	neighbors = new vector<SmoothedParticle*> (*clone.neighbors);
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
	glColor4f(1.0,1.0,1.0,0.0);
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

void SmoothedParticle::setVelocity(uVect* newVelocity)
{
	if(newVelocity != velocity)
	{	
		delete velocity;
		velocity = newVelocity;
	}

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

uVect* SmoothedParticle::getForceAtPoint(double x, double y, double z)
{
	double diffX = x - position->at(0);
	double diffY = y - position->at(1);
	double diffZ = z - position->at(2);	

	double distance = 0;
	double force = 0;
	
	distance = diffX*diffX + diffY*diffY + diffZ*diffZ;

	force = stretchA*(1.0/((distance - offsetA)*(distance - offsetA))) *
		forceConstant;
	cout << "force: " << force << endl;
	
	if(distance != 0)
	{
		double xComponent = diffX/force;
		double yComponent = diffY/force;
		double zComponent = diffZ/force;
	
		uVect *tempUVect = new uVect(xComponent, yComponent, zComponent, 1);
		tempUVect->setScalar(force);

		return tempUVect;
	}
	return NULL;
}

void SmoothedParticle::applyForce(uVect &actingForce, double elapsedTime)
{
	vector <double> *force = actingForce.getCartesian();
	vector <double> *vel = velocity->getCartesian();

	double acceleration = force->at(3) / mass;

	vel->at(0) += acceleration * force->at(0);
	vel->at(1) += acceleration * force->at(1);
	vel->at(2) += acceleration * force->at(2);
/*	cout << "force: " << force.at(0) << "\t " << 
		force.at(1) << "\t " <<
		force.at(2) << "\t " <<
		force.at(3) << "\t" << 
		acceleration << endl;*/
	
	if(velocity)
		delete velocity;
	
	velocity = new uVect(vel->at(0), vel->at(1), vel->at(2), 1);
	delete force;
	delete vel;
//	return vel;
}

void SmoothedParticle::updatePosition(double elapsedTime)
{
	vector <double> *vel = velocity->getCartesian();
/*
	cout << "position: " << position->at(0) << "\t\t " <<
		 position->at(1) << "\t\t " <<
		 position->at(2) << endl;
*/
	position->at(0) += vel->at(0) * elapsedTime;	
	position->at(1) += vel->at(1) * elapsedTime;	
	position->at(2) += vel->at(2) * elapsedTime;	
	
	delete vel;
}

bool SmoothedParticle::operator< (const SmoothedParticle &right)
{
	return position->at(0) < right.position->at(0);
}

bool SmoothedParticle::operator> (const SmoothedParticle &right)
{
	return position->at(0) > right.position->at(0);
}

double SmoothedParticle::smoothingKernel(double r, double h)
{
	double v = r/h;
	

	if(v > 2)
	{
		return 0;
	}
	else if(1 <= v && v <= 2)
	{
		return (3.0/(4.0*PI*h*h*h))*(10.0/3.0 - 7*v*v + 4*v*v*v);
	}

	else if(0 <= v && v < 1)
	{	
		return (3.0/(4.0*PI*h*h*h))*((2.0-v)*(2.0-v)*((5.0-4.0*v)/3.0));
	}
	
	else 
		return -1; //wtf just happened?

}
