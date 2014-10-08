#include "vector_inline.h"
#include <math.h>
#include "Plane.h"

#ifndef J_BALL
#define J_BALL

class Ball {

public:

	Ball();
	Ball(vector3, float, float, vector3, vector3);
	~Ball();

	void Show();
    void Step(double, int);
	void Undo();
	void Collides(Ball, float, int);
	void Collides(Plane);

	void Respond(Plane, float, float);
	void Respond(Ball, float);

	void setCenter(vector3 c) {center = c;}
	void setVelocity(vector3 v) {velocity = v;}
	void setAccel(vector3 a) {accel = a;}


private:
	
	vector3 center;
	vector3 velocity;
	vector3 accel;
	float mass;
	float radius;

	vector3 oldCenter;
	vector3 oldVel;
	vector3 oldAccel;
};

#endif