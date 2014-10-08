#include "thread.hpp"
#include "Ball.h"
#include <windows.h>
#include <vector>

using namespace std;

class BallThread : public Win32Thread
{
public:

	BallThread();
	~BallThread();

	void Init(Ball *ball, vector<Ball*> bList, float *t, int *i, int *e, HANDLE *st);

private:

	void run();

	Ball * b;

	vector<Ball*> balls;

	float *time;
	int * iters;
	int * extra;

	HANDLE *startThreading;

};