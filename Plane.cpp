#include "Plane.h"


Plane::
Plane()
{
	point = vector3(0,0,0);
	normal = vector3(0,1.0,0);
}

Plane::
Plane(vector3 p, vector3 n)
{
	point = p;
	normal = n.normalize();
}

Plane::
~Plane()
{
}
