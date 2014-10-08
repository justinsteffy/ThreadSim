#include "thread.hpp"
#include "Ball.h"
#include <windows.h>

class StepThread : public Win32Thread
{
public:

	StepThread();
	~StepThread();

	void Init(Ball* ball, float *t, int *i, int *e, HANDLE * st);

private:

	void run();
	Ball * b;

	float *time;
	int * iters;
	int * extra;

	HANDLE *startThreading;

};