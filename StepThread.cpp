#include "StepThread.h"

StepThread::
StepThread() : Win32Thread()
{

}

StepThread::
~StepThread()
{
}


void
StepThread::
Init(Ball* ball, float *t, int *i, int *e, HANDLE *st)
{
	b = ball;
	time = t;
	iters = i;
	extra = e;
	startThreading = st;
}

void
StepThread::
run()
{
	while(canRun())
	{
		WaitForSingleObject((*startThreading), INFINITE);

		b->Step((*time), (*extra));

		ResetEvent((*startThreading));
	}
}
