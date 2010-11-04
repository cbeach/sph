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



SmoothedParticle::SmoothedParticle():radius(1),mass(1),materialID(WATER),
threshold(0.5),stretchR(1),stretchA(1),offsetR(0),offsetA(0),maxR(100),
maxA(-100)
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

void SmoothedParticle::display()
{
	glPushMatrix();
	glColor4f(1.0,1.0,1.0,0.0);
	glTranslated(position->at(0), position->at(1), position->at(2));

	if(DL != 0)
	{
		glCallList(DL);
	}
	glPopMatrix();
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

//getters  ***************************************************************
vector<double>* SmoothedParticle::getPostition()
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
	double distance = 
		sqrt(abs((x-position->at(0))*(x-position->at(0))+
			(x-position->at(0))*(x-position->at(0))+
			(x-position->at(0))*(x-position->at(0))));
	double force = 0;

	if (distance > threshold)
	{
		force = stretchA(-1.0/((distance - offsetA)*(distance - offsetA)));
	}
	else if (distance < threshold)
	{
		force = stretchA(1.0/((distance - offsetA)*(distance - offsetA)));
	}
	else if (distance == threshold)
	{
		//do nothing, no attraction or repulsion.  force = 0;
	}

	//find vector

}




