/*************************************************************************
Name:	Casey Beach
Date:	6/27/2010
Terminus: ogl.h

ogl is used as a OpenGL controller.  ogl is responsible for managing all openGL related activities.
*************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <timer.hpp>

#include <GL/gl.h>
#include <GL/freeglut.h>
#include <GL/glut.h>

#include <./particle/sph.h>
#include <./render/ogl.h>
#include <./util/uVect.h>
#include <instrumentation.h>

#define GLUT_SCROLL_UP 		3	//Used in the mouseButtonEvent callback.  freeglut has functionality for mouse scolling, but aparently not the defines.
#define GLUT_SCROLL_DOWN	4	//so I added them to make my code more readable.

const int PARTICLE_COUNT = 5000;	//This variable dictates how many particles will be in the simulation


using namespace std;

//static functions need static variables, which are protected.

vector 	<double> *ogl::cameraPosition;
uVect	*ogl::cameraOrientation;

int 	ogl::mouseButtonState;
vector	<int> *ogl::mousePosition;

rect	*ogl::viewPaneSize;

sph 	*ogl::hydro;

timer	*ogl::timeSinceStart;
/************************************************************************/

ogl::ogl()
{
	ogl::cameraPosition = new vector <double> (3);
	ogl::mousePosition = new vector <int> (3);

	ogl::cameraOrientation = new uVect(0,0,1,0);
	ogl::viewPaneSize = new rect;
	
	ogl::timeSinceStart = new timer();

	for(int i = 0;i<3;i++)
	{
		ogl::cameraPosition->at(i) = 0;
		ogl::mousePosition->at(i) = 0;
	}
	
	ogl::mouseButtonState = 0;
	
}

ogl::~ogl(){}


void ogl::init(void)		//enable texture, lighting, shading.
{
	glClearColor(0.0,0.0,0.0,1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LEQUAL);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	GLfloat ambient[] = {1.0,1.0,1.0,1.0};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambient);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);

}

void ogl::initWorld()
{
	ogl::hydro = new sph(PARTICLE_COUNT);	//this is the object that will manage all of the particles
	ogl::hydro->setTimer(timeSinceStart);	//I'm setting a timer to bind the particles to real time regardless of the coputer that they are run on

#ifdef SCHOOL
	int success = 0;
	while(success < DISPLAY_LOOPS)
	{
		if(hydro->display())
			success++;
	}
#endif


}

void ogl::reshape(int w, int h)		//just in case some one wants to resize the window
{
	glViewport(0,0,(GLsizei)w,(GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60,(GLfloat)w/(GLfloat)h,.1,100);
	glMatrixMode(GL_MODELVIEW);

}

void ogl::idle(void)
{
	glutPostRedisplay();		//when idle is called just recall display
}

void ogl::display(void)		//this is the meat of the program.  ogl::display orchistrates all of the rendering done by the entire program
{
	using namespace std;
	int success = 0;
#ifndef SCHOOL	
	glClearColor(0.0,0.0,0.0,1.0);					//clear the background clear color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		//clear the color and depth buffers
	glLoadIdentity();						
	
	gluLookAt(20.0,0.0,0.0,
		   2.5,2.5,2.5,
		   0.0,0.0,1.0);		//set the cammera's position
#endif
	success = hydro->display();	//the success variable is used because the timer function in linux only has
					//a resolution of .001 sec.  For small numbers of particles I can get a LOT
					//better performance then this, so the frame rate sky rockets and particles
					//whiz around to fast to be seen.
#ifndef SCHOOL
	if(success)
		glutSwapBuffers();			//swap the buffer
#endif

}



void ogl::keyboardUp(unsigned char key, int x, int y)	//only implemented for the esc key.
{
	switch(key)
	{
		case 27:	//key27 = esc
			exit(0);
			break;
	}
}

int ogl::Start(int argc, char** argv)	//initialize glut and set all of the call backs
{
	//Set up the glut window
#ifndef SCHOOL
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize(640,480);
	glutInitWindowPosition(100,100);
	glutCreateWindow("Fluid Simulation");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	

	//Set the input callbacks
	glutKeyboardUpFunc(keyboardUp);			//keyboard key released
		
	initWorld();
	//Lets get started!
	glutMainLoop();
#endif

#ifdef SCHOOL
	cout << "School Mode Active, OpenGL Display functionality will not be used" << endl;
	initWorld();
#endif
	


	return 0;
}
 

