#include "Ball.h"
#include <gl/glut.h>
#include <iostream>

using namespace std;

Ball::
Ball()
{
	radius = 1.0;
	center = vector3(0.0,0.0,0.0);
	mass = 1.0;
	velocity = vector3(0.0,0.0,0.0);
	accel = vector3(0.0,0.0,0.0);

	oldCenter = vector3(0,0,0);
	oldVel = vector3(0.0,0.0,0.0);
	oldAccel = vector3(0.0,0.0,0.0);
}

Ball::
Ball(vector3 cent, float r, float m, vector3 vel, vector3 a)
{
	center = cent;
	radius  = r;
	mass = m;
	velocity = vel;
	accel = a;

	oldCenter = cent;
	oldVel = vel;
	oldAccel = a;
}

Ball::
~Ball()
{
}

void
Ball::
Show()
{
	glPushMatrix();
		glTranslatef(center[0], center[1], center[2]);
		glutSolidSphere(radius, 20,20);
	glPopMatrix();

}

void
Ball::
Step(double time, int max)
{
	for(int i=0; i<max*1000; i++)
	{
		double temp = tan(atan(exp(log(sqrt((double)i*i)))));
	}

	oldVel = velocity;
	oldCenter = center;
	oldAccel = accel;

	velocity += time*accel;
	center += time*velocity;
}

void
Ball::
Undo()
{
	center = oldCenter;
	velocity = oldVel;
	accel = oldAccel;
}

void
Ball::
Collides(Plane p)
{
	float dotprod = velocity.dot(p.getNormal());
	float distance = (center-p.getPoint()).dot(p.getNormal())/p.getNormal().length();
	distance -= radius;

	if(dotprod >= 0)
		return;
	else if(distance < 0)
		Respond(p, dotprod, distance);

}

void
Ball::
Collides(Ball b, float t, int iters)
{
	float time;
	vector3 dist = (center-b.center);
	float d = dist.length() - (radius+b.radius);

	if(d<0.1)
	{
		for(int i=0; i<iters; i++)
		{
			time = (float)i*(t/iters);
		
			Undo();
			b.Undo();

			Step(time,1);
			b.Step(time,1);

			vector3 dist = (center-b.center);
			float d = dist.length() - (radius+b.radius);

			if(d<0) {
				Respond(b, d);
				return;
			}
		}
	}
}

void
Ball::
Respond(Plane p, float dotprod, float distance)
{
	vector3 start(oldCenter);
	vector3 end(center);
	vector3 ray = end-start;

	float newd = (oldCenter-p.getPoint()).dot(p.getNormal())/p.getNormal().length();
	newd -= radius;

	float traveled = newd/(ray.length()-radius);

    vector3 colPoint = vector3(ray);
	colPoint *= traveled;

	colPoint += start;

	vector3 reflect(ray);
	float mag = reflect.length();
	reflect.normalize();

	float dot = (-1*reflect).dot(p.getNormal());
	vector3 finally = reflect + 2*(dot)*p.getNormal();

    center = colPoint+finally*mag;
	velocity = finally.normalize()*oldVel.length();
}

void
Ball::
Respond(Ball b, float distance)
{
	vector3 xAxis = center-b.center;
	xAxis.normalize();

	vector3 u1x = xAxis*(xAxis.dot(velocity));
	vector3 u1y = velocity - u1x;
	vector3 u2x = (-1*xAxis)*((-1*xAxis).dot(b.velocity));
	vector3 u2y = b.velocity - u2x;

	vector3 v1x=((u1x*mass)+(u2x*b.mass)-(u1x-u2x)*b.mass)/(mass+b.mass);
	vector3 v2x=((u1x*mass)+(u2x*b.mass)-(u2x-u1x)*mass)/(mass+b.mass);

	vector3 v1y=u1y;
	vector3 v2y=u2y;

	velocity = v1x+v1y;
	b.velocity = v2x+v2y;

}


