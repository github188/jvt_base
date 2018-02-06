#include <time.h>
#include <sys/time.h>
#include <assert.h>

#include "semaphore.h"

//#define poco_assert(cond)               do { if (!(cond)) std::abort(); } while (0)

namespace OSA 
{

SemaphoreImpl::SemaphoreImpl(int n, int max): _n(n), _max(max)
{
	assert (n >= 0 && max > 0 && n <= max);

	pthread_mutex_init(&_mutex, NULL);
	
	pthread_condattr_t attr;
	if (pthread_condattr_init(&attr))
	{
		pthread_mutex_destroy(&_mutex);
	}
	if (pthread_condattr_setclock(&attr, CLOCK_MONOTONIC))
    {
		pthread_condattr_destroy(&attr);
		pthread_mutex_destroy(&_mutex);
    }
	if (pthread_cond_init(&_cond, &attr))
	{
		pthread_condattr_destroy(&attr);
		pthread_mutex_destroy(&_mutex);
	}
	pthread_condattr_destroy(&attr);

}


SemaphoreImpl::~SemaphoreImpl()
{
	pthread_cond_destroy(&_cond);
	pthread_mutex_destroy(&_mutex);
}


void SemaphoreImpl::waitImpl()
{
	pthread_mutex_lock(&_mutex);
	while (_n < 1)
	{
		if (pthread_cond_wait(&_cond, &_mutex))
		{
			pthread_mutex_unlock(&_mutex);
		}
	}
	--_n;
	pthread_mutex_unlock(&_mutex);
}


bool SemaphoreImpl::waitImpl(long milliseconds)
{
	int rc = 0;
	struct timespec abstime;

	clock_gettime(CLOCK_MONOTONIC, &abstime);
	abstime.tv_sec  += milliseconds / 1000;
	abstime.tv_nsec += (milliseconds % 1000)*1000000;
	if (abstime.tv_nsec >= 1000000000)
	{
		abstime.tv_nsec -= 1000000000;
		abstime.tv_sec++;
	}

	pthread_mutex_lock(&_mutex);
	while (_n < 1)
	{
		if ((rc = pthread_cond_timedwait(&_cond, &_mutex, &abstime)))
		{
			if (rc == ETIMEDOUT) break;
			pthread_mutex_unlock(&_mutex);
		}
	}
	if (rc == 0) --_n;
	pthread_mutex_unlock(&_mutex);
	return rc == 0;
}

}

