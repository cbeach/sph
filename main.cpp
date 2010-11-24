#include <util/uVect.h>
#include <iostream>
#include <vector>
#include <algorithm>

#include <render/ogl.h>
#include <particle/sp.h>

using namespace std;
bool compareX(SmoothedParticle left,SmoothedParticle right)
{
	return left.getPosition()->at(0) < right.getPosition()->at(0);
}
int main(int argc, char* argv[])
{
/*
	vector <SmoothedParticle> *vect = new vector <SmoothedParticle> (2);
	vector <double> *vect2;
//	vect->at(0) = new SmoothedParticle;
//	vect->at(1) = new SmoothedParticle;

	vect->at(0).setPosition(2,2,2);
	vect->at(1).setPosition(1,1,1);
	
	vect2 = vect->at(0).getPosition();
	cout << vect2->at(0) << endl;

	sort(vect->begin(), vect->end(), compareX);
	
	vect2 = vect->at(0).getPosition();
	cout << vect2->at(0) << endl;
*/	
	


	ogl sph;

	sph.Start(argc, argv);

	return 0;
}

