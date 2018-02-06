#include "osa_task.h"
#include <unistd.h>

using namespace OSA;
//#define MM
#if 1
FastMutexImpl mtx = new FastMutexImpl;

int main()
{
	
	#ifdef MM
	
	TaskImpl *task = new TaskImpl("c++app");
	task->start();
	sleep(10);
	task->stop();
	delete task;
	#else
	TaskImpl task("c++app");
	task.start();
	sleep(10);
	task.stop();
	#endif

	
	
	return 0;
}
#endif

