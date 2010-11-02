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
#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include <vector>
#include <ctime>

#include <timer.hpp>

#include <particle/sph.h>
#include <util/uVect.h>

sph::sph()
{
	dls = new vector <GLuint> (3);
	frameTimer = new timer;
	timeLastFrame = frameTimer->elapsed();
	createDL(1,10);	
}

sph::sph(int particles)
{
	particleCount = particles;
	dls = new vector <GLuint> (3);
//	dls->at(0) = glGenLists(1);
	createDL(0,10);
	
	material = new vector<SmoothedParticle*>(particles);
	for(int i = 0; i < particles; i++)
	{
		cout << i << endl;
		material->at(i) = new SmoothedParticle();
		material->at(i)->setDL(dls->at(0));
		material->at(i)->setPosition(0.0,(double)i,-5.0);
	}
	frameTimer = new timer;
	timeLastFrame = frameTimer->elapsed();
}

sph::~sph()
{
	delete material;
//	delete metaMesh;
	delete dls;

}

void sph::display(clock_t currentTime)
{
	int index = 0;
	bool cont = true;
	if (frameTimer->elapsed() - timeLastFrame != 0)
	{
		while (cont == true)
		{
			try
			{
				if(index < material->capacity())
				{
					if(material->at(index))
					{
						material->at(index)->display();
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


		}
	}

}

void sph::createDL(int index, int space)
{
	
	int VertexCount = (90/space)*(360/space)*4;
	VERTICES *VERTEX = createSphere(2,0.0,0.0,0.0,10);
	dls->at(index) = glGenLists(1);
	glNewList(dls->at(index),GL_COMPILE);
		DisplaySphere(10.0,VertexCount,VERTEX);	
	glEndList();

	delete[] VERTEX;

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
