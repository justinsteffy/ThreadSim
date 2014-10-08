#include "thread.hpp"
#include "Ball.h"
#include "vector_inline.h"

class ballThread : public Win32Thread
{
public:
	ballThread() : Win32Thread() {b.setCenter(vector3(0,0,0)); b.setVelocity(vector3(0,-1.0,0));}
	~ballThread() {}

	void Show() {b.Show();}

private:
	void run();
	 Ball b;
};
