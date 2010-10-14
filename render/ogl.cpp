/*************************************************************************
Name:	Casey Beach
Date:	6/27/2010
Terminus: ogl.h

ogl is used as a OpenGL controller.  ogl is responsible for managing all openGL related activities.
*************************************************************************/
#include <stdlib.h>
#include <iostream>
#include <vector>

#include <GL/gl.h>
#include <GL/freeglut.h>
#include <GL/glut.h>

#include <particle/sph.h>
#include <./render/ogl.h>
#include <./util/uVect.h>

#define GLUT_SCROLL_UP 		3	//Used in the mouseButtonEvent callback.  freeglut has functionality for mouse scolling, but aparently not the defines.
#define GLUT_SCROLL_DOWN	4	//so I added them to make my code more readable.

using namespace std;

//static functions need static variables, which are protected.

vector 	<double> *ogl::cameraPosition;
uVect	*ogl::cameraOrientation;

int 	ogl::mouseButtonState;
vector	<int> *ogl::mousePosition;

rect	*ogl::viewPaneSize;

sph 	*ogl::hydro;

/************************************************************************/

ogl::ogl()
{
	ogl::cameraPosition = new vector <double> (3);
	ogl::mousePosition = new vector <int> (3);

	ogl::cameraOrientation = new uVect(0,0,1,0);
	ogl::viewPaneSize = new rect;
	
	ogl::hydro = new sph(2);

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
	GLfloat ambient[] = {.5,.5,.5,1.0};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambient);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);

}

void ogl::initWorld()
{





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


	
	glClearColor(0.0,0.0,0.0,1.0);					//clear the background clear color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		//clear the color and depth buffers
	glLoadIdentity();						
	
	gluLookAt(0.0,0.0,5.0,0.0,0.0,0.0,0.0,1.0,0.0);		//set the cammera's position
	hydro->display();

	glutSwapBuffers();			//swap the buffer
}


/*
void ogl::detectCollisions(stellarBody *one, stellarBody *two)
{
	double *position1 = one->getPosition();		//very simple algorithm, all of the objects that I deal with are just spheres, so if there is a collision
	double *position2 = two->getPosition();		//I'll know because the distance between two center points will be less than the sum of their radii
							//I just need to get the positions and I'll be ready to go

	double *sphericalCollisionVect;			//The vecter calculations are a LOT easier to do in a spherical coordinate system.  Even though at this point I only use 
	double *sphericalVelocityVect;			//positions in the x-y plane

	uVect *resultantVector;				//If a collision is detected I do a rigidbody calculation

	double difference[3] = {position2[0]-position1[0],	//the difference vector between the two bodies
				position2[1]-position1[1],
				position2[2]-position1[2]};

	double radius1 = one->getRadiusActual();		//get the radius of both bodies
	double radius2 = two->getRadiusActual();
	
	double dist = sqrt(	pow(difference[0],2.0)+		
				pow(difference[1],2.0)+
				pow(difference[2],2.0));	//find the distance between them
	

	if(dist <= radius1+radius2)				//They have collided if the distance is less than the sum of the two radii
	{
		uVect collisionNormal(difference[0], difference[1], difference[2]);	//a collision has been detected, so find the collision's Normal vector
		sphericalCollisionVect = collisionNormal.getSpherical();		//get the normal's spherical representation
		sphericalVelocityVect = two->getVelocity()->getSpherical();		//get the velocity of the bodies
		two->getVelocity()->reflect(&collisionNormal);				//reflect the velocity vector about the collision normal
//		cout << sphericalCollisionVect[1] << " " << sphericalCollisionVect[2] << endl;

	}

}
*/





/*
void ogl::keyboardDown(unsigned char key, int x, int y){}
void ogl::keyboardSpecialKey(int key, int x, int y)		//handles the FXX keys as well as the arrow keys
{


	switch(key)
	{
		case GLUT_KEY_RIGHT:
			ogl::masterTranslation[0] += 0.5;
			break;
		case GLUT_KEY_LEFT:
			ogl::masterTranslation[0] -= 0.5;
			break;
		case GLUT_KEY_UP:
			ogl::masterTranslation[1] += 0.5;
			break;
		case GLUT_KEY_DOWN:
			ogl::masterTranslation[1] -= 0.5;
			break;
	}
	glutPostRedisplay();
}
*/
void ogl::keyboardUp(unsigned char key, int x, int y)	//only implemented for the esc key.
{
	switch(key)
	{
		case 27:	//key27 = esc
			exit(0);
			break;
	}
}
/*
void ogl::mouseButtonEvent(int button, int state, int x, int y)	//mouse clicks, releases and scrolling
{
	switch(button)
	{
		
		case GLUT_LEFT_BUTTON:
			if(!state)
				ogl::mouseState[0] = true;
			else
				ogl::mouseState[0] = false;
			break;
			
		case GLUT_MIDDLE_BUTTON:
			if(!state)
				ogl::mouseState[1] = true;
			else
				ogl::mouseState[1] = false;

			break;
			
		case GLUT_RIGHT_BUTTON:
			if(!state)
				ogl::mouseState[2] = true;
			else
				ogl::mouseState[2] = false;

			break;

		case GLUT_SCROLL_UP:
			ogl::zOffset += 0.5;
			break;
		case GLUT_SCROLL_DOWN:
			ogl::zOffset -= 0.5;
			break;
	}
//	glutPostRedisplay();

}
void ogl::mouseActiveMove(int x,int y)	//moving the mouse with a button depressed
{
	
	int xDiff = x - mousePosition[0];
	int yDiff = y - mousePosition[1];

	if(mouseState[1])
	{
		ogl::masterRotation[0] += (double)xDiff * 0.5;
		ogl::masterRotation[1] += (double)yDiff * 0.5;

		if(ogl::masterRotation[0] >= 360)
			ogl::masterRotation[0] -= 360;
		
		if(ogl::masterRotation[1] >= 360)
			ogl::masterRotation[1] -= 360;

	}
	
	mousePosition[0] = x;
	mousePosition[1] = y;
	glutPostRedisplay();

}
void ogl::mousePassiveMove(int x, int y)	//just moving the mouse activates this callback
{
	ogl::mousePosition[0] = x;
	ogl::mousePosition[1] = y;
}

*/

int ogl::Start(int argc, char** argv)	//initialize glut and set all of the call backs
{
	//Set up the glut window

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
//	glutKeyboardFunc(keyboardDown);			//keyboard key pressed
	glutKeyboardUpFunc(keyboardUp);			//keyboard key released
//	glutSpecialFunc(keyboardSpecialKey);		//one of the F keys is pressed
//	glutMouseFunc(mouseButtonEvent);		//mouse button was pressed or realeased
//	glutMotionFunc(mouseActiveMove);		//the mouse moved while a button was pressed
//	glutPassiveMotionFunc(mousePassiveMove);	//regular mouse movement

	cout << "should start running now" << endl;
	//Lets get started!
	glutMainLoop();


}
 

