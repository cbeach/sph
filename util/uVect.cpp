/*************************************************************************
Name:	Casey Beach
Date:	6/27/2010
Terminus: uVect.h

uVect(unitVector) handles vector opperations and is possibly the most useful class in
this project.  It is used in almost all classes.  Any time velocity or 
force must be calculated a uVect is used.
*************************************************************************/

//#include "stdafx.h"

#include "uVect.h"
#include <iostream>
#include <math.h>
#include <vector>


using namespace std;

//most of these functions are very straight forward

uVect::uVect():coordSys(0)
{}

uVect::uVect(const uVect &old)	//copy constructor
{
	updated[0] = false;
	updated[1] = false;
	updated[2] = false;
			
	switch(old.coordSys)
	{
		case 0:
			spherical[0] = old.spherical[0];
			spherical[1] = old.spherical[1];
			spherical[2] = old.spherical[2];
			break;
		case 1:
			cartesian[0] = old.cartesian[0];
			cartesian[1] = old.cartesian[1];
			cartesian[2] = old.cartesian[2];
			cartesian[3] = old.cartesian[3];
			break;
		case 2:
			cylindrical[0] = cylindrical[0];
			cylindrical[1] = cylindrical[1];
			cylindrical[2] = cylindrical[2];
			break;
		default:
			spherical[0] = 0;
			spherical[1] = 0;
			spherical[2] = 0;
	}

	coordSys = old.coordSys;
	updated[coordSys] = true;

}
/*
uVect::uVect(double* vect)	//a vector stored as an array.
{


}
*/
uVect::uVect(double a, double b, double c, int mode)	//obvious constructor
{
	coordSys = mode;
	updated[0] = false;
	updated[1] = false;
	updated[2] = false;

	switch(mode)
	{
		case 0:
			spherical[0] = a;
			spherical[1] = b;
			spherical[2] = c;
			break;
		case 1:
			cartesian[0] = a;
			cartesian[1] = b;
			cartesian[2] = c;
			cartesian[3] = a*a + b*b + c*c;		//all scalars will be used and scalar^2 for the sake of speed.  Most scalar ops. will be simple comparisons
			break;
		case 2:
			cylindrical[0] = a;
			cylindrical[1] = b;
			cylindrical[2] = c;
			break;
		default:
			spherical[0] = 0;
			spherical[1] = 0;
			spherical[2] = 0;
	}

	coordSys = mode;
	updated[coordSys] = true;


}


uVect::~uVect()
{
/*	if(spherical)
		delete[] spherical;
	if(cartesian)
		delete[] cartesian;
	if(cylindrical)
		delete[] cylindrical;
	if(updated)
		delete[] updated;
*/
}




//Setters*****************************************************************
void uVect::setVect(double a, double b, double c, int mode)
{
	coordSys = mode;
	updated[0] = false;
	updated[1] = false;
	updated[2] = false;

	switch(mode)
	{
		case 0:
			spherical[0] = a;
			spherical[1] = b;
			spherical[2] = c;
			break;
		case 1:
			cartesian[0] = a*a + b*b + c*c;		//all scalars will be used and scalar^2 for the sake of speed.  Most scalar ops. will be simple comparisons
			cartesian[1] = a;
			cartesian[2] = b;
			cartesian[3] = c;
			break;
		case 2:
			cylindrical[0] = a;
			cylindrical[1] = b;
			cylindrical[2] = c;
			break;
		default:
			spherical[0] = 0;
			spherical[1] = 0;
			spherical[2] = 0;
	}

	coordSys = mode;
	updated[coordSys] = true;
}

void uVect::setScalar(double newMag)
{
	switch(coordSys)
	{
		case 0:
			spherical[0] = newMag;
			
			break;
		case 1:
			cartesian[3] = newMag;		//all scalars will be used and scalar^2 for the sake of speed.  Most scalar ops. will be simple comparisons
			break;
	}

}

vector <double>& uVect::getVect()	//self explanitory
{
	vector <double> *tempV = NULL;
	switch(coordSys)
	{
		case 0:
			tempV = new vector <double> (3);
			for(int i = 0; i< 3; i++)
			{
				tempV->at(i) = spherical[i];
			}
			return *tempV;
			break;
		case 1:
			tempV = new vector <double> (4);
			for(int i = 0; i < 3; i++)
			{
				tempV->at(i) = cartesian[i];
			}

			return *tempV;
			break;
		case 2:
			tempV = new vector <double> (3);
			for(int i = 0; i< 3; i++)
			{
				tempV->at(i) = cylindrical[i];
			}
			return *tempV;
			break;
//		default:
//			spherical[0] = 0;
//			spherical[1] = 0;
//			spherical[2] = 0;
	}
	
	return *tempV;
}

vector <double>* uVect::getCartesian()	//self explanitory
{
	vector <double> *tempV = new vector <double> (4);

	for(int i = 0; i < 4; i++)
		tempV->at(i) = cartesian[i];

	return tempV;
}

vector <double>* uVect::getCylindrical()	//self explanitory
{
	vector <double> *tempV = new vector <double> (4);

	for(int i = 0; i < 3; i++)
		tempV->at(i) = cylindrical[i];

	return tempV;
}

vector <double>* uVect::getSpherical()	//self explanitory
{
	vector <double> *tempV = new vector <double> (4);

	for(int i = 0; i < 3; i++)
		tempV->at(i) = spherical[i];

	return tempV;
}



//end of getters and setters**********************************************

void uVect::addVect(uVect* second)	//add two vectors
{
	if(updated[1] == false)
		updateCartesian();

	cartesian[0] = cartesian[0] + second->cartesian[0];			
	cartesian[1] = cartesian[1] + second->cartesian[1];			
	cartesian[2] = cartesian[2] + second->cartesian[2];			

	if(coordSys == 0)
		updateSpherical();
	else if(coordSys == 3)
		updateCylindrical();	
	
}

void uVect::subVect(uVect* second)	//subtract two vectors
{
	if(updated[1] == false)
		updateCartesian();

	cartesian[0] = cartesian[0] - second->cartesian[0];			
	cartesian[1] = cartesian[1] - second->cartesian[1];			
	cartesian[2] = cartesian[2] - second->cartesian[2];			

	if(coordSys == 0)
		updateSpherical();
	else if(coordSys == 3)
		updateCylindrical();	

}

void uVect::scaleVect(double factor)	//subtract two vectors
{
	switch(coordSys)
	{
		case 0:
			spherical[0] *= factor;
			break;
		case 1:
			cartesian[0] *= factor;
			cartesian[1] *= factor;
			cartesian[2] *= factor;
			break;
		case 2:
			updateCartesian();
			cartesian[0] *= factor;
			cartesian[1] *= factor;
			cartesian[2] *= factor;
			updateCylindrical();
			break;

	}
}

void uVect::reflect(uVect *normal)	// reflect the vector off a surface (like light in a mirror)
{
	/*
	double *sphericalNormal = normal->getSpherical();	//spherical rep of the normal
	double incedence;		//the at which this vector hit the surface

	scalar = sqrt(pow(i,2)+pow(j,2)+pow(k,2));	
	
	getSpherical();		//spherical representation of this vector
	
	if(phi > sphericalNormal[2])	//reflection is only suported in the x-y plane at this point
	{
		incedence = phi-sphericalNormal[2];	//get the angle between the normal and this vector, then reflect.
		i = scalar * sin(DEGTORAD(sphericalNormal[2]-incedence));
		j = scalar * cos(DEGTORAD(sphericalNormal[2]-incedence));
		k = 0;
	}

	else if(phi < sphericalNormal[2])
	{
		incedence = phi-sphericalNormal[2];
		i = scalar * sin(DEGTORAD(sphericalNormal[2]+incedence));
		j = scalar * cos(DEGTORAD(sphericalNormal[2]+incedence));
		k = 0;
	}

	else if(phi == sphericalNormal[2])	//the vector is hitting head on. just turn it arround.
		invert();		

	delete[] sphericalNormal;
	*/
}

void uVect::invert()	//turn the vector by 180 degrees
{
	cartesian[0] *= -1.0;
	cartesian[1] *= -1.0;
	cartesian[2] *= -1.0;

}


uVect* uVect::cross(uVect *second)	//get the cross product (will later be implemented with the *opperator)
{
	/*
	if(&second)
	{
		uVect *crossProd = new uVect(	j*second.k - k*second.j,
						k*second.i - i*second.k,
						i*second.j - j*second.i);
						
		return crossProd;
	}
	else 
	*/
		return NULL;
	
}
double uVect::dot(uVect *second)	//get the dot product
{
	return NULL;
}




/*
uVect::uVect(double mag, double* thetas)	//make a vector from cylindrical coordinates
{
	using namespace std;

	scalar = mag;
	try
	{
		i = cos(thetas[0]*(PI/180.0));
		j = sin(thetas[0]*(PI/180.0));
		k = sin(thetas[1]*(PI/180.0));
		throw("Invalid double* array");
	}
	catch(char *str)
	{
		cout << "Exception: " << str << endl;
	}
}
*/


void uVect::updateSpherical()
{
	if(updated[0] == true)
		return;
	
	if(updated[2] == true)
	{
		spherical[0] = sqrt(cylindrical[0]*cylindrical[0] + cylindrical[2]*cylindrical[2]);
		spherical[1] = atan(cylindrical[0]/cylindrical[1]);
		spherical[2] = cylindrical[2];
	}
	else
	{
		spherical[0] = sqrt(cartesian[0]*cartesian[0]+cartesian[1]*cartesian[1]+cartesian[2]*cartesian[2]);
		spherical[1] = acos(cartesian[2]/spherical[0]);
		spherical[2] = atan2(cartesian[1],cartesian[0]);
	}

}

void uVect::updateCylindrical()
{
	if(updated[2] == true)
		return;

	if(updated[0] == true)
	{
		cylindrical[0] = spherical[0]*sin(spherical[1]);
		cylindrical[1] = spherical[0]*cos(spherical[1]);
		cylindrical[2] = spherical[2];
	}
	else
	{
		cylindrical[0] = sqrt(cartesian[0]*cartesian[0]+cartesian[1]*cartesian[1]);
		cylindrical[2] = cartesian[2];
		if(cartesian[0] == 0 && cartesian[1] == 0)
			cylindrical[1] = 0;
		else if(cartesian[0] >= 0)
			cylindrical[1] = asin(cartesian[1]/cylindrical[0]);
		else if(cartesian[0] < 0)
			cylindrical[1] = -asin(cartesian[1]/cylindrical[0])+PI;
	}

}
void uVect::updateCartesian()
{
	if(updated[1] == true)
		return;
	
	if(updated[2] == true)
	{
		cartesian[0] = cylindrical[0]*sin(cylindrical[1]);
		cartesian[1] = cylindrical[0]*cos(cylindrical[1]);
		cartesian[2] = cylindrical[2];
	}

	else
	{
		cartesian[0] = spherical[0]*sin(spherical[1])*cos(spherical[2]);
		cartesian[1] = spherical[0]*sin(spherical[1])*sin(spherical[2]);
		cartesian[0] = spherical[0]*cos(spherical[1]);
	}
}

void uVect::normalize()
{
/*
	switch(coordSys)
	{
		case 0:
			break
		case 1:



	}
*/
}
