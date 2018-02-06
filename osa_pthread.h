#ifndef OSA_Thread_POSIX_INCLUDED
#define OSA_Thread_POSIX_INCLUDED

#include <pthread.h>
#include <features.h>
#include <string>

#include <vector>

namespace OSA {


class ThreadImpl 
{

private:
	static void* Execute(void* param);

	ThreadImpl(const ThreadImpl&);
	ThreadImpl& operator=(const ThreadImpl&);

protected:

	/**
	 * \brief Function to know if the thread has finished computation.
	 *
	 * This function can be used in the subclasses to check if a request
	 * for termination has been made; if so, the thread is terminated.
	 */
	static void checkTermination() 
	{
		pthread_testcancel();
	}

	/**
	 * \brief This function must be reimplemented in a derived class
	 * to define the specific function run by the thread.
	 */
	virtual void run() = 0;

public:

	ThreadImpl(bool jion, int stack_size, std::string name);
	virtual ~ThreadImpl();
	bool start();
	bool stop();
	bool jion_lmpl();
	int get_stackSize();
	char* get_thrName();
	static bool blockSignal (int sig);
	static bool unblockSignal (int sig);
	bool sendSignal(int sig);
	static bool setSignalHandler(int sig, void (*handler) (int));
	bool setSchedParam(int policy, int priority);
	bool getSchedParam(int* policy, int* priority);
	
private:
	pthread_t     handle_;
	bool 		  isStarted_;
	std::size_t   stackSize;
	bool          joined;
	std::string   pth_name;
};

} 

#endif /* ABSTRACTTHREAD_HPP_ */


