#include "PhysicsThread.h"

PhysicsThread::
PhysicsThread() : Win32Thread()
{

}

PhysicsThread::
~PhysicsThread()
{


}

void 
PhysicsThread::
Init(vector<Ball*> bList, vector<Plane*> pList, float *t, int *j, int *e, HANDLE* start,HANDLE* stop, clock_t *endtime)
{
	for(int i=0; i<bList.size(); i++)
		balls.push_back(bList[i]);

	for(int i=0; i<pList.size(); i++)
		planes.push_back(pList[i]);
    
	time = t;
	iters = j;
	extra = e;
	startThreading = start;
	stopThreading = stop;
	end = endtime;
}

void
PhysicsThread::
run()
{
	while(canRun())
	{
		WaitForSingleObject((*startThreading),INFINITE);

		for(int i=0; i<balls.size(); i++)
			balls[i]->Step((*time),(*extra));

		for(int i=0; i<balls.size()-1; i++)
			for(int j=i+1; j<balls.size(); j++)
				balls[i]->Collides((*balls[j]),(*time),(*iters));

		for(int i=0; i<balls.size(); i++)
			for(int j=0; j<planes.size(); j++)
				balls[i]->Collides((*planes[j]));

		(*end) = clock();
		ResetEvent((*startThreading));
	}
}
