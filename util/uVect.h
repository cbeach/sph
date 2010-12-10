/*************************************************************************
Name:	Casey Beach
Date:	6/27/2010
Terminus: uVect.h

uVect(unitVector) handles vector opperations and is possibly the most useful class in
this project.  It is used in almost all classes.  Any time velocity or 
force must be calculated a uVect is used.
*************************************************************************/
#ifndef UVECT_H
#define UVECT_H

#include <math.h>
#include <vector>

using namespace std;

#define PI 3.1415926535898
#define G 9.80665  //m/s^2

#define DEGTORAD(x) x*PI/180.0
#define RADTODEG(x) x*180.0/PI

#define	SPHERICAL	0
#define	CARTESIAN	1
#define	CYLINDRICAL	2

/*************************************************************************
Notes:
1)  Vector magnitudes are stored as squared values, not the true magnitude.
since most opperations simply need to compare distances this is all right
if you need a true magnitude just sqrt it


*************************************************************************/

class uVect
{
	protected:
		double spherical[3];	//
		double cartesian[4];	//(x,y,z,r)
		double cylindrical[3];	//

		//which coordinate system vector is up to date
		bool updated[3];	//[0] = true: spherical is current [1] = true: cartesian is current [2] = ...
		//what coordinate system am I using
		int coordSys;		//0 = spherical, 1 = cartesian, 2 = cylindrical

	public:
		uVect();
		uVect(const uVect&);
		//uVect(double*, int);
		uVect(double a, double b, double c, int mode);	
		
		~uVect();

		enum {sphere, cart, cylind};
		//setter
		virtual void setVect(double a, double b, double c, int mode);		
		virtual void setScalar(double);								


		//getters
		virtual vector <double>& getVect();		//returns a vector according to what coordinate system you're using
		virtual vector <double>* getCartesian();
		virtual vector <double>* getCylindrical();
		virtual vector <double>* getSpherical();
				
		
		virtual void addVect(uVect*);	//add another vector to this one.  If vectors don't use the same coordinate system the resultant will have the same system as "this".
		virtual void subVect(uVect*);	//subtract another vector from this one
		virtual void scaleVect(double);
				
		virtual void reflect(uVect* normal);	//reflect this vector about a normal. (like a collision, or light bouncing off a mirror)
		virtual void invert();			//do a 180.
		
		virtual uVect* cross(uVect*);		//cross product
		virtual double dot(uVect*);			//dot product

		virtual void updateSpherical();
		virtual void updateCylindrical();
		virtual void updateCartesian();

		virtual void normalize();
};

#endif
