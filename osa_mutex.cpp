#include "mutex.h"

#include <sys/select.h>
#include <cstring>
#include <sys/time.h>


namespace OSA
{

MutexImpl::MutexImpl()
{
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
#if defined(PTHREAD_MUTEX_RECURSIVE_NP)
	pthread_mutexattr_settype_np(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
#endif
	if (pthread_mutex_init(&_mutex, &attr))
	{
		pthread_mutexattr_destroy(&attr);
	}
	pthread_mutexattr_destroy(&attr);
}


MutexImpl::MutexImpl(bool fast)
{
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
#if defined(PTHREAD_MUTEX_RECURSIVE_NP)
	pthread_mutexattr_settype_np(&attr, fast ? PTHREAD_MUTEX_NORMAL_NP : PTHREAD_MUTEX_RECURSIVE_NP);
#endif
	if (pthread_mutex_init(&_mutex, &attr))
	{
		pthread_mutexattr_destroy(&attr);
	}
	pthread_mutexattr_destroy(&attr);
}


MutexImpl::~MutexImpl()
{
	pthread_mutex_destroy(&mutex_);
}


} // name
