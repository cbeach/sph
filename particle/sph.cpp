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
#include <stack>
#include <algorithm>
#include <ctime>

#include <timer.hpp>

#include <particle/sph.h>
#include <util/uVect.h>
#include <instrumentation.h>

const int DIMENSION = 0.01;

bool compareX(SmoothedParticle* left, SmoothedParticle* right)
{
	vector <double> *leftPosition = left->getPosition(); 
	vector <double> *rightPosition = right->getPosition(); 
	
	if( leftPosition->at(2) < rightPosition->at(2))
	{
		delete leftPosition;
		delete rightPosition;
		
		return true;

	}

	delete leftPosition;
	delete rightPosition;
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
	
	dls = new vector <GLuint> (3);
	frameTimer = new timer;
	createDL(0,10);
	double randX, randY, randZ;
	double randI, randJ, randK; //velocity vector values
	
	srand(time(0));
	
	#ifdef VISIBLE_TEST
	particleCount = 5;
	material = new vector<SmoothedParticle*>(particleCount);
	for(int i = 0; i < 5; i++)
	{
		material->at(i) = new SmoothedParticle();
		material->at(i)->setDL(dls->at(0));
		material->at(i)->setPosition(0, i/2.0, 0);
		material->at(i)->setMass(1);
	}

	#endif

	#ifndef VISIBLE_TEST
	particleCount = particles;
	material = new vector<SmoothedParticle*>(particles);

	for(int i = 0; i < particles; i++)
	{
		randX = ((double)rand()/(double)RAND_MAX) * 4.0;
		randY = ((double)rand()/(double)RAND_MAX) * 4.0;
		randZ = ((double)rand()/(double)RAND_MAX) * 4.0;
		
		randI = (((double)rand()/(double)RAND_MAX) * 0.2) - 0.1;
		randJ = (((double)rand()/(double)RAND_MAX) * 0.2) - 0.1;
		randK = (((double)rand()/(double)RAND_MAX) * 0.2) - 0.1;

		material->at(i) = new SmoothedParticle();
		material->at(i)->setDL(dls->at(0));
		material->at(i)->setPosition(randX, randY, randZ);
		material->at(i)->setVelocity(randI, randJ, randK);
		material->at(i)->setMass(5);
	}


	#endif
	timeLastFrame = frameTimer->elapsed();
}

sph::sph(int particles, int cube)
{
	particleCount = DIMENSION*DIMENSION*DIMENSION;
	dls = new vector <GLuint> (3);
	frameTimer = new timer;
	createDL(0,10);
	srand(time(0));

	double randI, randJ, randK; //velocity vector values

	material = new vector<SmoothedParticle*>(DIMENSION*DIMENSION*DIMENSION);

	for(int i = 0; i < DIMENSION; i++)
	{
		for(int j = 0; j < DIMENSION; j++)
		{
			for(int k = 0; k <  DIMENSION; k++)
			{
				randI = (((double)rand()/(double)RAND_MAX) * 0.2) - 0.1;
				randJ = (((double)rand()/(double)RAND_MAX) * 0.2) - 0.1;
				randK = (((double)rand()/(double)RAND_MAX) * 0.2) - 0.1;

				material->at(DIMENSION*DIMENSION*i + DIMENSION*j + k) = new SmoothedParticle();
				material->at(DIMENSION*DIMENSION*i + DIMENSION*j + k)->setDL(dls->at(0));
				material->at(DIMENSION*DIMENSION*i + DIMENSION*j + k)->setPosition(i/(DIMENSION/5.0), j/(DIMENSION/5.0), k/(DIMENSION/5.0));
				material->at(i)->setVelocity(randI, randJ, randK);
				material->at(DIMENSION*DIMENSION*i + DIMENSION*j + k)->setMass(5);
			}
		}
	}
	for(int i = 0; i < DIMENSION*DIMENSION*DIMENSION; i++)
	{
		if(material->at(i) == 0)
			cout << i << endl;


	}

	timeLastFrame = frameTimer->elapsed();
}
sph::~sph()
{
	for(int i = 0; i < particleCount; i++)
	{
		delete material->at(i);
	}
	delete material;
//	delete metaMesh;
	delete dls;
	delete frameTimer;

}

void sph::applyForces(double timeDiff)
{
	double distance = 0;

	uVect *primaryTempUVect;
	uVect *secondaryTempUVect;
	vector <double> *primaryPositionVector;
	vector <double> *secondaryPositionVector;
	vector <double> vel;

	sort(material->begin(), material->end(), compareX);	
	calculateDensity();

	for(int i = 0; i < particleCount; i++)
	{
		
		primaryPositionVector = material->at(i)->getPosition();
		for(int j = i + 1; j < particleCount; j++)
		{
				
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
				
			if(distance <= ER)
			{
				primaryTempUVect = material->at(i)->getForceAtPoint(material->at(j));
				secondaryTempUVect = material->at(j)->getForceAtPoint(material->at(i));
			
				material->at(i)->pushNeighbor(j);
				material->at(j)->pushNeighbor(i);
				
				if(primaryTempUVect && secondaryTempUVect)
				{
					material->at(i)->applyForce(*secondaryTempUVect, timeDiff);
					material->at(j)->applyForce(*primaryTempUVect, timeDiff);
						
				}
	
				delete secondaryPositionVector;
				delete primaryTempUVect;
				delete secondaryTempUVect;
			} else 
			{
				delete secondaryPositionVector;
	
				break;
			}
		}
		
		delete primaryPositionVector;
	}

	for (int i = 0; i < particleCount; i++)
	{
		material->at(i)->updatePosition(timeDiff);
		material->at(i)->zeroDensity();
	}
}

void sph::calculateDensity()
{
	double distance = 0;
	
	uVect *primaryTempUVect;
	uVect *secondaryTempUVect;

	vector <double> *primaryPositionVector;
	vector <double> *secondaryPositionVector;

	for(int i = 0; i < particleCount; i++)
	{
		primaryPositionVector = material->at(i)->getPosition();
		for(int j = 0; j < particleCount; j++)
		{
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
				
			if(distance <= ER*ER)
			{
				if(primaryPositionVector && secondaryPositionVector)
				{
					material->at(i)->calculateDensity(material->at(j));			
				}
	
				delete secondaryPositionVector;
				delete primaryTempUVect;
				delete secondaryTempUVect;
			} else 
			{
				delete secondaryPositionVector;
	
				break;
			}
				
		}
		delete primaryPositionVector;
	}
	for(int i = 0; i<particleCount; i++)
	{
		material->at(i)->clearNAN();
//		material->at(i)->printDensity();
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
				if((unsigned int)index < material->capacity())
				{
					if(material->at(index))
					{
						
						#ifndef SCHOOL
						#ifdef VISIBLE_TEST
						switch(index)
						{
							case 0:
								glColor3f(1.0,0.0,0.0);
								break;
							case 1:
								glColor3f(0.0,1.0,0.0);
								break;
							case 2:
								glColor3f(0.0,0.0,1.0);
								break;
							case 3:
								glColor3f(1.0,1.0,1.0);
								break;
							case 4:
								glColor3f(1.0,1.0,0.0);
								break;
							default:	
								break;


						};
						#endif
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
//	int VertexCount = (90/space)*(360/space)*4;
	VERTICES *VERTEX = createSphere(2,0.0,0.0,0.0,10);
	dls->at(index) = glGenLists(1);
	glNewList(dls->at(index),GL_COMPILE);
		glBegin(GL_POINTS);
			glVertex2i(0, 0);
		glEnd();


//		DisplaySphere(10.0,VertexCount,VERTEX);	
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



















