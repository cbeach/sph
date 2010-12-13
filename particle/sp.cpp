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
#define WEIGHTLESS

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

uVect* SmoothedParticle::getForceAtPoint(SmoothedParticle *neighbor)
{
	double diffX = neighbor->position->at(0) - position->at(0);
	double diffY = neighbor->position->at(1) - position->at(1);
	double diffZ = neighbor->position->at(2) - position->at(2);	
	
	double distance = 0;
	
	double force = 0;

	double forceX = 0;
	double forceY = 0;
	double forceZ = 0;
	
	double nMass = neighbor->mass;
	double nRadius = neighbor->radius;

	distance = sqrt(diffX*diffX + diffY*diffY + diffZ*diffZ);
	
//	forceX = 




	force = -(((distance-2)*(distance - 2)*(distance - 2)) - (distance - 2)*4);
//	force =  stretchA*(-1.0/((distance - offsetA)*(distance - offsetA))) * forceConstant * .001;

	if(distance != 0)
	{
		double xComponent = ((1.0) * diffX / distance) * force;
		double yComponent = ((1.0) * diffY / distance) * force;
		double zComponent = ((1.0) * diffZ / distance) * force;
	
		uVect *tempUVect = new uVect(xComponent, yComponent, zComponent, 1);

		return tempUVect;
	}
	return NULL;
}

void SmoothedParticle::applyForce(uVect &actingForce, double elapsedTime)
{
	vector <double> *force = actingForce.getCartesian();
	vector <double> *vel = velocity->getCartesian();

//	double acceleration = force->at(3) / mass;

	vel->at(0) += (force->at(0) / mass) * elapsedTime;
	vel->at(1) += (force->at(1) / mass) * elapsedTime;
	vel->at(2) += (force->at(2) / mass) * elapsedTime;

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
	
	#ifndef WEIGHTLESS
	vel->at(2) += -9.8 * elapsedTime;
	#endif
	
	if(velocity)
		delete velocity;
	velocity = new uVect(vel->at(0), vel->at(1), vel->at(2), uVect::cart);
/*
	cout << "position: " << position->at(0) << "\t\t " <<
		 position->at(1) << "\t\t " <<
		 position->at(2) << endl;
*/
	position->at(0) += vel->at(0) * elapsedTime;	
	position->at(1) += vel->at(1) * elapsedTime;	
	position->at(2) += vel->at(2) * elapsedTime;	
/*	
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

bool SmoothedParticle::operator< (const SmoothedParticle &right)
{
	return position->at(0) < right.position->at(0);
}

bool SmoothedParticle::operator> (const SmoothedParticle &right)
{
	return position->at(0) > right.position->at(0);
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
	
	//315/(64*PI*ER^9) * (ER^2 - mag^2)^3
	return ((315.0)/(64 * PI * ER*ER*ER*ER*ER*ER*ER*ER*ER))*
		(ER*ER - mag*mag)*(ER*ER - mag*mag)*(ER*ER - mag*mag); 

}


/*
void SmoothedParticle::pushNeighbor(int n)
{
	neighbors->push(n);
}

int SmoothedParticle::popNeighbor()
{
	int n = 0;
	if(!neighbors->empty())
	{
		n = neighbors->top();
		neighbors->pop();
	}
	return n;
}

int SmoothedParticle::sizeNeighbor()
{
	return neighbors->size();

}
*/
void SmoothedParticle::smoothVelocity(SmoothedParticle *neighborParticle)
{

/*
	vector <double> *vel = velocity->getCartesian();
	vector <double> *vel2 = neighborParticle->velocity->getCartesian();
	vector <double> *nPos = neighborParticle->position;
	vector <double> diff(3);

	double nMass = neighborParticle->mass;
	double nRadius = neighborParticle->mass;

	diff.at(0) = nPos->at(0) - position->at(0);
	diff.at(1) = nPos->at(1) - position->at(1);
	diff.at(2) = nPos->at(2) - position->at(2);
	
	vector <double> *smoothed = densityKernel(&diff);

	vel->at(0) +=	nMass * (vel2->at(0)/(nMass/(nRadius*nRadius*PI))) * smoothed->at(0);
	vel->at(1) +=	nMass * (vel2->at(1)/(nMass/(nRadius*nRadius*PI))) * smoothed->at(1);
	vel->at(2) +=	nMass * (vel2->at(2)/(nMass/(nRadius*nRadius*PI))) * smoothed->at(2);

	
	if(velocity)
		delete velocity;
	velocity = new uVect(vel->at(0),vel->at(1),vel->at(2), uVect::cart);

	delete vel;
	delete vel2;
	delete smoothed;
*/
}



void SmoothedParticle::calculateDensity(SmoothedParticle *neighbor)
{
	if(neighbor)
	{
		density += neighbor->mass * densityKernel(neighbor->position);
	}
}
























