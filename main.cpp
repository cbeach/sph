// SPH.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"

#include <util/uVect.h>
#include <iostream>

#include <render/ogl.h>
#include <particle/sp.h>

using namespace std;

int main(int argc, char* argv[])
{
	uVect *vect = new uVect(1,0,0,SPHERICAL);
	ogl sph;

	sph.Start(argc, argv);
	

	return 0;
}

