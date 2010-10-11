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
#include <math.h>

#include <GL/gl.h>
#include <GL/freeglut.h>
#include <GL/glut.h>

#include <particle/sp.h>
#include <util/uVect.h>



SmoothedParticle::SmoothedParticle():radius(1),mass(1),materialID(WATER)
{
	position = new vector <double> (3);
	neighbors = new vector<SmoothedParticle*> (10);
	velocity  = new uVect();
	color = new vector<int> (3);
}


SmoothedParticle::SmoothedParticle(const SmoothedParticle& clone)
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

//getters
void SmoothedParticle::setPostion(vector<double> *newPosition)
{
	if(newPosition != position)
	{
		delete position;
		position = newPosition;
	}

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

//setters  ***************************************************************
vector<double>* SmoothedParticle::getPostition()
{
	vector <double> *tempV = new vector <double> (*position);
	return tempV;
}

vector<int>* SmoothedParticle::SmoothedParticle::getColor()
{
	vector<int>* tempV = new vector<int> (*color);
	return tempV;

}

uVect* SmoothedParticle::SmoothedParticle::getVelocity()
{
	uVect *tempU = new uVect(*velocity);
	return tempU;
}
double SmoothedParticle::SmoothedParticle::getRadius(){return radius;}
double SmoothedParticle::SmoothedParticle::getMass(){return mass;}
double SmoothedParticle::SmoothedParticle::getMaterialID(){return materialID;}
float SmoothedParticle::SmoothedParticle::getPressurescale(){return pressureScale;}





