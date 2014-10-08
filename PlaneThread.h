#include "thread.hpp"
#include "Ball.h"
#include "Plane.h"
#include <windows.h>
#include <vector>

using namespace std;

class PlaneThread : public Win32Thread
{
public:

	PlaneThread();
	~PlaneThread();

	void Init(Ball* ball, vector<Plane*> pList, float *t, int *i, int *e, HANDLE* st);

private:

	void run();
	Ball * b;
	vector<Plane*> planes;

	float *time;
	int * iters;
	int * extra;

	HANDLE *startThreading;

};