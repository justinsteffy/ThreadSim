#include "thread.hpp"
#include "Ball.h"
#include "Plane.h"
#include <vector>
#include <windows.h>
#include <time.h>

using namespace std;

class PhysicsThread : public Win32Thread
{
public:
	PhysicsThread();
	~PhysicsThread();

	void Init(vector<Ball*>, vector<Plane*>, float *t, int *j, int *e, HANDLE* start, HANDLE* stop, clock_t *endtime);

private:

	void run();
	vector<Ball*> balls;
	vector<Plane*> planes;

	float *time;
	int * iters;
	int * extra;

	clock_t *end;

	HANDLE *startThreading;
	HANDLE *stopThreading;
};
