/*************************************************************************
Name:	Casey Beach
Date:	10/13/2010
SPH: 	sph.cpp

sph is responsible for orginization of a group of smooth particles.
*************************************************************************/

#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

#include <string.h>
#include <cstdlib>
#include <stdio.h>

#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>

#include <timer.hpp>

#include <particle/sph.h>
#include <util/uVect.h>
#include <instrumentation.h>

bool compareX(SmoothedParticle* left, SmoothedParticle* right)
{
	if(left->getPosition()->at(0) < right->getPosition()->at(0))
		return true;
	return false;
}


sph::sph()
{
	dls = new vector <GLuint> (3);
	createDL(1,10);	
	frameTimer = new timer;
	timeLastFrame = frameTimer->elapsed();
}

sph::sph(int particles)
{
	particleCount = particles;

	dls = new vector <GLuint> (3);
	frameTimer = new timer;
	createDL(0,10);
	double randX, randY, randZ;
	srand(time(0));
	
	material = new vector<SmoothedParticle*>(particles);
	for(int i = 0; i < particles; i++)
	{
		randX = ((double)rand()/(double)RAND_MAX) * 5.0;
		randY = ((double)rand()/(double)RAND_MAX) * 5.0;
		randZ = ((double)rand()/(double)RAND_MAX) * 5.0;

		material->at(i) = new SmoothedParticle();
		material->at(i)->setDL(dls->at(0));
		material->at(i)->setPosition(randX, randY, randZ);
		material->at(i)->setMass(5);
	}
	timeLastFrame = frameTimer->elapsed();
}

sph::~sph()
{
	delete material;
//	delete metaMesh;
	delete dls;
	delete frameTimer;

}

void sph::applyForces(double timeDiff)
{
	double distance = 0;
	int count = 0;

	uVect *primaryTempUVect;
	uVect *secondaryTempUVect;
	vector <double> *primaryPositionVector;
	vector <double> *secondaryPositionVector;
	vector <double> vel;
	vector <double> *vel2;
	
<<<<<<< HEAD
	sort(material->begin(), material->end(), compareX);	
=======
	sort (material->begin(), material->end());

	for(int i = 0; i < particleCount; i++)
	{
		positionVector = material->at(i)->getPosition();
		cout << positionVector->at(0) << endl;
		delete positionVector;
	}
>>>>>>> d84dd994ca5e12816ab94c1590c5f5fe52451ca8

	for(int i = 0; i < particleCount; i++)
	{
		for(int j = i + 1; j < particleCount; j++)
		{
//			if(i != j)
//			{
				
			primaryPositionVector = material->at(i)->getPosition();
			secondaryPositionVector = material->at(j)->getPosition();
			
			if(primaryPositionVector && secondaryPositionVector)
			{
				distance = ((primaryPositionVector->at(0) - secondaryPositionVector->at(0))*
					    (primaryPositionVector->at(0) - secondaryPositionVector->at(0))+
					    (primaryPositionVector->at(1) - secondaryPositionVector->at(1))*
					    (primaryPositionVector->at(1) - secondaryPositionVector->at(1))+
					    (primaryPositionVector->at(2) - secondaryPositionVector->at(2))*
					    (primaryPositionVector->at(2) - secondaryPositionVector->at(2)));
			}
				
			if(distance <= 25)
			{
				primaryTempUVect = material->at(i)->getForceAtPoint(
							secondaryPositionVector->at(0),
							secondaryPositionVector->at(1),
							secondaryPositionVector->at(2));
				secondaryTempUVect = material->at(j)->getForceAtPoint(
							primaryPositionVector->at(0),
							primaryPositionVector->at(1),
							primaryPositionVector->at(2));
/*
				if(i == 0)
				{
					if(j == 1)
						cout << 0 << ": " << distance << ", " << material->at(0)->getPosition()->at(0) << endl;
				
					cout << j << ": " << distance << ", " << secondaryPositionVector->at(0) << endl;
				}
*/
				if(primaryTempUVect && secondaryTempUVect)
				{
					material->at(i)->applyForce(*secondaryTempUVect, timeDiff);
					material->at(j)->applyForce(*primaryTempUVect, timeDiff);
						
				}
<<<<<<< HEAD
				
				delete primaryTempUVect;
				delete secondaryTempUVect;
=======

				delete positionVector;
				delete tempUVect;
>>>>>>> d84dd994ca5e12816ab94c1590c5f5fe52451ca8
				/*
				if(i == 1)
				{
					tempUVect = material->at(i)->getVelocity();
					 vel = tempUVect->getCartesian();
					 
				
					cout << "i " << vel.at(0) << " " << 
						"j " << vel.at(1) << " " <<
						"k " << vel.at(2) << " " <<
						"l " << vel.at(3) << endl;

					cout << "vel[0] " << vel2->at(0) << " " <<
						"vel[1] " << vel2->at(1) << " " <<
						"vel[2] " << vel2->at(2) << endl << endl;
					
					delete positionVector;
					delete tempUVect;
				}
				*/

//			}
				count++;
			} else 
				break;
			
			delete primaryPositionVector;
			delete secondaryPositionVector;

		}
	}

<<<<<<< HEAD
	
	cout << "count: " << count << endl;

=======
	cout << endl << endl;
>>>>>>> d84dd994ca5e12816ab94c1590c5f5fe52451ca8
	for (int i = 0; i < particleCount; i++)
	{
		
//		cout << "here" << endl;
		material->at(i)->updatePosition(timeDiff);

	}
	
}


int sph::display()
{
	int index = 0;
	int success = 0;
	bool cont = true;
	
	double currentTime = frameTimer->elapsed();
	
	success = 0;
	
	if((currentTime - timeLastFrame) > 0)
	{
		applyForces(currentTime - timeLastFrame);
	}

	if ((currentTime - timeLastFrame) > 0)
	{
		while (cont == true)
		{
			try
			{
				if(index < material->capacity())
				{
					if(material->at(index))
					{
						
						#ifndef SCHOOL 
						material->at(index)->display(timeLastFrame);
						#endif
					}
					index++;
				}
			}
			catch(char *str)
			{
				if(strcmp(str, "out_of_range"))
				{
					cont = false;
					break;
				}
				else
				{
					cout << "caught exception " << str << " ending program" << endl;
					exit(1);
				}

			}
			if(index >= particleCount)
				cont = false;


			
			timeLastFrame = frameTimer->elapsed();
			success = 1;
		}
	}

	return success;

}

void sph::createDL(int index, int space)
{
#ifndef SCHOOL	
	int VertexCount = (90/space)*(360/space)*4;
	VERTICES *VERTEX = createSphere(2,0.0,0.0,0.0,10);
	dls->at(index) = glGenLists(1);
	glNewList(dls->at(index),GL_COMPILE);
		DisplaySphere(10.0,VertexCount,VERTEX);	
	glEndList();

	delete[] VERTEX;
#endif
}

void sph::DisplaySphere (double R, int VertexCount, VERTICES *VERTEX)
{

	int b;
	glScalef (0.0125 * R, 0.0125 * R, 0.0125 * R);
	glRotatef (90, 1, 0, 0);
//	glBindTexture (GL_TEXTURE_2D, *planetTex );
	glBegin (GL_TRIANGLE_STRIP);

		for(b=0;b<=VertexCount;b++)
		{
	//		glTexCoord2f (VERTEX[b].U, VERTEX[b].V);
			glVertex3f (VERTEX[b].X, VERTEX[b].Y, -VERTEX[b].Z);
		}


		for(b = 0;b<=VertexCount;b++)
		{
	//		glTexCoord2f (VERTEX[b].U, -VERTEX[b].V);
			glVertex3f (VERTEX[b].X, VERTEX[b].Y, VERTEX[b].Z);
		}
	    
	glEnd();
}


/*************************************************************************
The createSphere function was copied from 
http://www.swiftless.com/tutorials/opengl/sphere.html

Now new and Improved!
There was an error in the original code which created a plane that 
extended north and south from the sphere's meridian.  This was likely
due to floating point rounding errors which would cause the triangle
strip's ends not to meet.  With the addition of the new if blocks this
should be fixed.
*************************************************************************/
VERTICES* sph::createSphere (double radius, double H, double K, double Z, int space) 
{
	using namespace std;
	int n;
	double a;
	double b;
	
	int VertexCount = (90/space)*(360/space)*4;
	VERTICES *VERTEX = new VERTICES[VertexCount];

	n = 0;
	for( b = 0; b <= 90 - space; b+=space)
	{

		for( a = 0; a <= 360 - space; a+=space)
		{



			VERTEX[n].X = radius * sin((a) / 180 * PI) * sin((b) / 180 * PI) - H;
			VERTEX[n].Y = radius * cos((a) / 180 * PI) * sin((b) / 180 * PI) + K;
			VERTEX[n].Z = radius * cos((b) / 180 * PI) - Z;
			VERTEX[n].V = (2 * b) / 360;
			VERTEX[n].U = (a) / 360;
			n++;

			VERTEX[n].X = radius * sin((a) / 180 * PI) * sin((b + space) / 180 * PI) - H;
			VERTEX[n].Y = radius * cos((a) / 180 * PI) * sin((b + space) / 180 * PI) + K;
			VERTEX[n].Z = radius * cos((b + space) / 180 * PI) - Z;
			VERTEX[n].V = (2 * (b + space)) / 360;
			VERTEX[n].U = (a) / 360;
			n++;
			
			if (a < 360 -space)			//this is an added conditional
			{					//if this is not the end of the strip, business as usual
				VERTEX[n].X = radius * sin((a + space) / 180 * PI) * sin((b) / 180 * PI) - H;
				VERTEX[n].Y = radius * cos((a + space) / 180 * PI) * sin((b) / 180 * PI) + K;
				VERTEX[n].Z = radius * cos((b) / 180 * PI) - Z;
				VERTEX[n].V = (2 * b) / 360;
				VERTEX[n].U = (a + space) / 360;
				n++;

				VERTEX[n].X = radius * sin((a + space) / 180 * PI) * sin((b + space) /180 * PI) - H;
				VERTEX[n].Y = radius * cos((a + space) / 180 * PI) * sin((b + space) /180 * PI) + K;
				VERTEX[n].Z = radius * cos((b + space) / 180 * PI) - Z;
				VERTEX[n].V = (2 * (b + space)) / 360;
				VERTEX[n].U = (a + space) / 360;
				n++;
			}
			else if (a >= 360 - space)		//however if the end of the strip has been reached, set the two end points 
			{					//to be equal to the two begining points.
				VERTEX[n].X = VERTEX[0].X;
				VERTEX[n].Y = VERTEX[0].Y;
				VERTEX[n].Z = VERTEX[0].Z;
				VERTEX[n].V = VERTEX[0].U;
				VERTEX[n].U = VERTEX[0].V;
				n++;

				VERTEX[n].X = VERTEX[1].X;
				VERTEX[n].Y = VERTEX[1].Y;
				VERTEX[n].Z = VERTEX[1].Z;
				VERTEX[n].V = VERTEX[1].U;
				VERTEX[n].U = VERTEX[1].V;
				n++;


			}

		}
	}
	return VERTEX;
}

void sph::setTimer(timer *newTimer)
{

	frameTimer = newTimer;

}

