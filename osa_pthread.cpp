#include "osa_pthread.h"

#include <unistd.h>
#include <csignal>
#include <strings.h>
#include <stdexcept>
#include <stdio.h>
#include <string.h>
#include <errno.h>

namespace OSA 
{

/**
 * \brief The static function representing the code executed in the thread context.
 * 
 * This function just calls the run() function of the subclass.
 * We need this level of indirection because pthread_create() cannot accept
 * a method which is non static or virtual.
 * @param param The pointer to the concrete subclass.
 * @return Always zero.
 */
void *ThreadImpl::Execute(void* param)
{
	//printf("Execute\n");
	ThreadImpl* th = reinterpret_cast<ThreadImpl*>(param);
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	//printf("run\n");
	th->run();
	return 0;
}

/**
 * \brief Constructor. Initialize the class attributes.
 */
ThreadImpl::ThreadImpl(bool jion, int stack_size, std::string name)
{
	isStarted_ = false;
	joined = jion;
	stackSize = stack_size;
	pth_name = name;
}


/**
 * \brief Destructor.
 *
 * In case the thread is running, it stops the thread and prints an error
 * message.
 */
ThreadImpl::~ThreadImpl() 
{
	//printf("yyy\n");
	if (isStarted_)
	{
		stop();
	}
}

/**
 * \brief Starts execution of the thread by calling run().
 *
 * If the thread is
 * already started this function does nothing.
 * @return true if the thread is started or it has been previously started;
 * false if an error occurs when starting the thread.
 */
bool ThreadImpl::start()
{
	if (isStarted_)
		return true;

	pthread_attr_t attributes;
	pthread_attr_init(&attributes);

	if (stackSize != 0)
	{
		if (0 != pthread_attr_setstacksize(&attributes, stackSize))
		{
			pthread_attr_destroy(&attributes);
		}
	}

	if (!joined)
	{
		pthread_attr_setdetachstate(&attributes, PTHREAD_CREATE_DETACHED);
	}
	//printf("start..........\n");
	if (pthread_create(&handle_, &attributes, ThreadImpl::Execute, (void*)this) == 0)
	{
		isStarted_ = true;
	}
	else
	{
		printf("falid=%s\n",  strerror(errno));
	}

	//printf("yyy\n");
	pthread_attr_destroy(&attributes);
	return isStarted_;
}

int ThreadImpl::get_stackSize()
{
 	return stackSize;
}


/**
 * \brief Stops the running thread.
 *
 * @return true on success; false if an error occurs or the thread
 * is not running (i.e., it has not been started or it has been already stopped)
 */
bool ThreadImpl::stop()
{
	if (!isStarted_)
	{
		return false;
	}

	isStarted_ = false;
	if (pthread_cancel(handle_) == 0)
	{
		return true;
	}
	return false;
}

/**
 * \brief Blocks the calling thread until the thread is finished.
 *
 * This method blocks the calling thread until the thread associated with
 * the AbstractThread object has finished execution
 * @return true on success, false if an error occurs.
 */
bool ThreadImpl::jion_lmpl()
{
	if (pthread_join(handle_, NULL) == 0)
	{
		isStarted_ = false;
		return true;
	}
	return false;
}

/**
 * \brief Masks a specific signal on this thread
 *
 * This method allows to block a specific signal
 * The list of signals is available on /usr/include/bits/signum.h
 * @param sig the signal to be blocked
 * @return true on success; false if some error occurred
 */
bool ThreadImpl::blockSignal (int sig)
{
	sigset_t m;
	sigemptyset(&m);
	sigaddset(&m, sig);
	if (pthread_sigmask(SIG_BLOCK, &m, NULL) != 0) 
	{
		return false;
	}
	return true;
}

/**
 * \brief Unmasks a signal previously masked
 *
 * This method allows to unblock a specific signal previously blocked
 * through blockSignal().
 * The list of signals is available on /usr/include/bits/signum.h
 * @param sig the signal to be unblocked
 * @return true on success; false if some error occurred
 */
bool ThreadImpl::unblockSignal (int sig)
{
	sigset_t m;
	sigemptyset(&m);
	sigaddset(&m, sig);
	if (pthread_sigmask(SIG_UNBLOCK, &m, NULL) != 0)
	{
		return false;
	}
	return true;
}

/**
 * \brief Sends a signal to the thread
 *
 * This method allows to send a signal from one thread to another thread
 * The list of signals is available on /usr/include/bits/signum.h
 * @param sig the signal to be sent
 * @return true on success; false if some error occurred
 */
bool ThreadImpl::sendSignal(int sig)
{
	if (pthread_kill(handle_, sig) != 0)
	{
		return false;
	}
	return true;
}

/**
 * \brief Set a handler for a specific signal
 *
 * This method allows to manually set a handler for handling a
 * specific signal.
 * The list of signals is available on /usr/include/bits/signum.h
 * Use signals less as possible, mainly for standard situations.
 * During the execution of the handler other signals may arrive.
 * This can lead
 * to inconsistent states. The handler must be short.
 * It must just update the internal state and/or kill the application.
 * Not all library functions can be called inside the handler without having
 * strange behaviors (see man signal). In particular, it's not safe calling
 * functions of the standard library, like printf() or exit(), or other
 * functions defined inside the application itself. The access to global
 * variables is not safe either, unless they have been defined as volatile.
 * @param sig the signal to be sent
 * @return true on success; false if some error occurred
 */
bool ThreadImpl::setSignalHandler(int sig, void (*handler) (int))
{
	bool ret = true;
	sigset_t oldset, set;
	struct sigaction sa;
	/* mask all signals until the handlers are installed */
	sigfillset(&set);
	sigprocmask(SIG_SETMASK, &set, &oldset);
	bzero( &sa, sizeof(sa) );
	sa.sa_handler = handler;
	if (sigaction(sig, &sa, NULL) < 0)
	{
		ret = false;
	}
	/* remove the mask */
	sigprocmask(SIG_SETMASK, &oldset,NULL);
	return ret;
}


/**
 * \brief Set scheduling policy and priority
 *
 * @param policy: policy (SCHED_FIFO, SCHED_RR or SCHED_OTHER)
 * @param priority: scheduling priority
 * @return true in case of success; false in case of error
 */
bool ThreadImpl::setSchedParam(int policy, int priority)
{
	struct sched_param p;
	p.sched_priority = priority;
	if (pthread_setschedparam(handle_, policy, &p) == 0)
		return true;
	else
		return false;
}

/**
 * \brief Get current scheduling policy and priority
 *
 * @param policy: policy (SCHED_FIFO, SCHED_RR or SCHED_OTHER)
 * @param priority: scheduling priority; it has a meaning only for SCHED_FIFO
 * and SCHED_RR
 * @return true in case of success; false in case of error
 */
bool ThreadImpl::getSchedParam(int* policy, int* priority)
{
	struct sched_param p;
	int ret = pthread_getschedparam(handle_, policy, &p);
	*priority = p.sched_priority;
	if (ret == 0)
		return true;
	else
		return false;
}


#if defined(ONPOSIX_LINUX_SPECIFIC) && defined(__GLIBC__) && \
    ((__GLIBC__ > 2) || ((__GLIBC__ == 2) && (__GLIBC_MINOR__ > 3)))
/**
 * \brief Set CPU affinity
 *
 * Example of usage:
 * \code
 * std::vector<bool> v (2);
 * v[0] = true;
 * v[1] = false;
 * setAffinity(v);
 * \endcode
 * @param v: vector of booleans containing the affinity (true/false)
 * @exception std::runtime_error in case affinity cannot be set
 */
void ThreadImpl::setAffinity(const std::vector<bool>& v)
{
	cpu_set_t s;
	CPU_ZERO(&s);
	for (unsigned int i = 0; (i < v.size()) && (i < CPU_SETSIZE); ++i)
		if (v[i])
			CPU_SET(i, &s);

	if ((pthread_setaffinity_np(handle_, sizeof(s), &s) != 0))
		throw std::runtime_error ("Set affinity error");
}

/**
 * \brief Get CPU affinity
 *
 * Example of usage:
 * \code
 * std::vector<bool> v (2);
 * getAffinity(&v);
 * std::cout << "Affinity: " << v[0] << " " << v[1] << std::endl;
 * \endcode
 * @param v: vector of booleans containing the current affinity (true/false)
 * @exception std::runtime_error in case affinity cannot be get
 */
void ThreadImpl::getAffinity(std::vector<bool>* v)
{
	cpu_set_t s;
	CPU_ZERO(&s);

	if ((pthread_getaffinity_np(handle_, sizeof(s), &s) != 0))
		throw std::runtime_error ("Get affinity error");

        for (unsigned int j = 0; (j < CPU_SETSIZE) && (j < v->size()); ++j) {
		if (CPU_ISSET(j, &s))
			(*v)[j] = true;
		else
		(*v)[j] = false;
	}
}
#endif /* ONPOSIX_LINUX_SPECIFIC && GLIBC */



}

