/*************************************************************************
Name:	Casey Beach
Date:	6/27/2010
Terminus: ogl.h

Terminus is meant to be an MMOG (Massively Multiplayer Online Game), as
such it will be, roughly speaking split into two parts, ogl and
server.  The ogl will handle all user interaction and graphics.
*************************************************************************/
#ifndef CLIENT_H
#define CLIENT_H

#include <GL/glut.h>
#include <GL/gl.h>
#include <iostream>
#include <vector>
#include <math.h>

#include <uVect.h>

struct Rect
{
	float width;
	float height;
	float x;
	float y;
};

class ogl
{


	protected:

//		sph hydro;
//		vector <mesh> objects;
		vector <double> cammeraPosition;
		uVect cammeraOrientation;
		Rect viewPaneSize;
		int moustButtonState;
		vector <int> mousePostion;
		

		

		/***********************************************************************
		Because of my decision to use C++ with openGL I had to make extensive
		use of static funtions, which are all protected.  The reason for all
		of the static functions is that openGL is a C library and uses function*'s
		for all of it's callbacks. Since function* != ogl::function* I had to 
		use either use protected static functions or recompile openGL as a C++
		library.			
		***********************************************************************/

		static void init(void);					//Initializes a lot of openGL features, mostly just glEnable calls
		static void idle(void);					//OpenGL idle callback. No idle calculations are implemented... yet
		static void display(void);				//OpenGL display call back.  Handles all rendering
		static void reshape(int w, int h);			//OpenGL window reshape callback

/*
		static void keyboardDown(unsigned char key, int x, int y);	//The next six functions are OpenGl callbacks for keyboard and mouse inputs
		static void keyboardSpecialKey(int key, int x, int y);
		static void keyboardUp(unsigned char key, int x, int y);
		static void mouseButtonEvent(int button, int state, int x, int y);
		static void mouseActiveMove(int x,int y);
		static void mousePassiveMove(int x, int y);
		
		static void detectCollisions(stellarBody*,stellarBody*);	//I chose to implement a collision detection algorithm
		
*/
	public:	
		ogl();
		~ogl();
		virtual int Start(int argc, char** argv);			//This functions is called to start the program.  Mostly just a generic glut initialization function.
     	

};

#endif

