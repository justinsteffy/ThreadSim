#include "PlaneThread.h"

PlaneThread::
PlaneThread() : Win32Thread()
{
}

PlaneThread::
~PlaneThread()
{

}

void
PlaneThread::
Init(Ball *ball, vector<Plane*> pList, float *t, int *i, int *e, HANDLE *st)
{
	b = ball;

	for(int j = 0; j<pList.size(); j++)
		planes.push_back(pList[j]);

	time = t;
	iters = i;
	extra = e;

	startThreading = st;
}

void
PlaneThread::
run()
{
	while(canRun())
	{

		WaitForSingleObject((*startThreading),INFINITE);

		for(int j=0; j<planes.size(); j++)
			b->Collides((*planes[j]));

		ResetEvent((*startThreading));
	}
}