#include "Ball.h"
#include "Plane.h"
#include "PhysicsThread.h"
#include "StepThread.h"
#include "PlaneThread.h"
#include "BallThread.h"
#include <vector>
#include <iostream>
#include <windows.h>
#include <time.h>

using namespace std;

#ifndef J_T_SIM
#define J_T_SIM

class ThreadedSim
{
public:

	ThreadedSim();
	~ThreadedSim() {}

	void SimStep();
	void Show();

	void BallCols();
	void PlaneCols();

	void AddBall(float radius, float mass);
	void Reset(int num, float t, int j, int e, int tm);

	void setTime(float t) {time = t;}
	void setExtra(int e) {extra = e;}
	void setIters(int i) {iters = i;}
	void setThreadModel(int i) {threadModel = i;}

	void StepPThread();
	clock_t EndTime() {return end;}

	void runStepThreads();
	void runPlaneThreads();
	void runBallThreads();

private:
	
	vector<Ball*> bList;
	vector<Plane*> pList;

	float time;
	int iters;
	int extra;

	int threadModel;

	PhysicsThread pThread;

	HANDLE startThreading;
	HANDLE stopThreading;

	clock_t end;

	vector<StepThread*> sThreads;
	vector<PlaneThread*> pThreads;
	vector<BallThread*> bThreads;

	vector<HANDLE> sHandles;
	vector<HANDLE> pHandles;
	vector<HANDLE> bHandles;

	HANDLE stepDone;
	HANDLE planeDone;
	HANDLE ballDone;

};

#endif