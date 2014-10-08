#include "ThreadedSim.h"


ThreadedSim::
ThreadedSim()
{

	pList.push_back(new Plane(vector3(0.0,-10.0,0.0),vector3(0.0,1.0,0.0)));
	pList.push_back(new Plane(vector3(0.0,10.0,0.0),vector3(0.0,-1.0,0.0)));
	pList.push_back(new Plane(vector3(-10.0,0.,0.0),vector3(1.0,0.0,0.0)));
	pList.push_back(new Plane(vector3(10.0,0.0,0.0),vector3(-1.0,0.0,0.0)));
	pList.push_back(new Plane(vector3(0.0,0.0,-10.0),vector3(0.0,0.0,1.0)));
	pList.push_back(new Plane(vector3(0.0,0.0,10.0),vector3(0.0,0.0,-1.0)));

	for(int i=0; i<10; i++)
	{
		vector3 randstart(20.0*rand()/RAND_MAX-10.0,20.0*rand()/RAND_MAX-10.0,20.0*rand()/RAND_MAX-10.0);
		vector3 randvel((float)rand()/RAND_MAX,(float)rand()/RAND_MAX,(float)rand()/RAND_MAX);
		bList.push_back(new Ball(randstart,1.0,1.0,randvel,vector3(0,0,0)));
	}

	time = 0.2;
	iters = 10;
	extra = 5;

	threadModel = 0;

	startThreading = CreateEvent(NULL, false, false, NULL);
	stopThreading = CreateEvent(NULL, false, false, NULL);

	int numB = bList.size();

	for(int i=0; i<numB; i++)
	{
		sHandles.push_back(CreateEvent(NULL, false, false, NULL));
		pHandles.push_back(CreateEvent(NULL, false, false, NULL));
		bHandles.push_back(CreateEvent(NULL, false, false, NULL));

		sThreads.push_back(new StepThread());
		pThreads.push_back(new PlaneThread());
		bThreads.push_back(new BallThread());
	}

	for(int i=0; i<numB; i++)
	{
		sThreads[i]->Init(bList[i], &time, &iters, &extra, &sHandles[i]);
		pThreads[i]->Init(bList[i], pList, &time, &iters, &extra, &pHandles[i]);
		bThreads[i]->Init(bList[i], bList,  &time, &iters, &extra, &bHandles[i]);
	}


	for(int i=0; i<numB; i++)
	{
		sThreads[i]->create();
		sThreads[i]->start();

		bThreads[i]->create();
		bThreads[i]->start();

		pThreads[i]->create();
		pThreads[i]->start();
	}

	pThread.Init(bList,pList, &time, &iters, &extra, &startThreading, &stopThreading, &end);

	pThread.create();
	pThread.start();

	stepDone = CreateEvent(NULL, false, false, NULL);
	planeDone = CreateEvent(NULL, false, false, NULL);
	ballDone = CreateEvent(NULL, false, false, NULL);
	
}

void
ThreadedSim::
SimStep()
{
	for(int i=0; i<bList.size(); i++)
		bList[i]->Step(time,extra);

}

void
ThreadedSim::
Show()
{
	for(int i=0; i<bList.size(); i++)
		bList[i]->Show();
}

void
ThreadedSim::
BallCols()
{
	for(int i=0; i<bList.size()-1; i++)
		for(int j=i+1; j<bList.size(); j++)
			bList[i]->Collides((*bList[j]),time,iters);
}

void
ThreadedSim::
PlaneCols()
{
	for(int i=0; i<bList.size(); i++)
		for(int j=0; j<pList.size(); j++)
			bList[i]->Collides((*pList[j]));
}

void
ThreadedSim::
AddBall(float radius, float mass)
{
	vector3 randstart(20.0*rand()/RAND_MAX-10.0,20.0*rand()/RAND_MAX-10.0,20.0*rand()/RAND_MAX-10.0);
	vector3 randvel((float)rand()/RAND_MAX,(float)rand()/RAND_MAX,(float)rand()/RAND_MAX);
	bList.push_back(new Ball(randstart,radius,mass,randvel,vector3(0,0,0)));
}

void
ThreadedSim::
Reset(int num, float t, int j, int e, int tm)
{
	bList.clear();
	for(int i=0; i<num; i++)
	{
		vector3 randstart(20.0*rand()/RAND_MAX-10.0,20.0*rand()/RAND_MAX-10.0,20.0*rand()/RAND_MAX-10.0);
		vector3 randvel((float)rand()/RAND_MAX,(float)rand()/RAND_MAX,(float)rand()/RAND_MAX);
		bList.push_back(new Ball(randstart,1.0,1.0,randvel,vector3(0,0,0)));
	}

	time = t;
	extra = e;
	iters = j;

	threadModel = tm;

	pThread.Init(bList,pList, &time, &iters, &extra, &startThreading, &stopThreading, &end);

}

void
ThreadedSim::
StepPThread()
{
	SetEvent(startThreading);
}

void
ThreadedSim::
runStepThreads()
{
	for(int i=0; i<sThreads.size(); i++)
		SetEvent(sHandles[i]);
}

void
ThreadedSim::
runPlaneThreads()
{
	for(int i=0; i<pThreads.size(); i++)
		SetEvent(pHandles[i]);
}

void
ThreadedSim::
runBallThreads()
{
	for(int i=0; i<bThreads.size(); i++)
		SetEvent(bHandles[i]);
}