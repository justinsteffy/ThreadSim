#include "BallThread.h"

BallThread::
BallThread() : Win32Thread()
{

}

BallThread::
~BallThread()
{

}

void
BallThread::
Init(Ball *ball, vector<Ball*> bList, float *t, int *i, int *e, HANDLE *st)
{
	b = ball;

	for(int j=0; j<bList.size(); j++)
		balls.push_back(bList[j]);

	time = t;
	iters = i;
	extra = e;

	startThreading = st;
}

void
BallThread::
run()
{
	while(canRun())
	{
		WaitForSingleObject((*startThreading),INFINITE);

		for(int j=0; j<balls.size(); j++)
			if(b != balls[j])
				b->Collides((*balls[j]), (*time), (*extra));

		ResetEvent((*startThreading));
	}
}