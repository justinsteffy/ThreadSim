#include "vector_inline.h"

#ifndef J_PLANE
#define J_PLANE
class Plane {

public:

	Plane();
	Plane(vector3, vector3);
	~Plane();

	vector3 getNormal() {return normal;}
	vector3 getPoint() {return point;}

private:
	vector3 point;
	vector3 normal;

};

#endif