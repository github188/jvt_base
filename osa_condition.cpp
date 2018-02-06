
#include <time.h>
#include <sys/time.h>
#include "osa_condition.h"

namespace OSA 
{

ConditionImpl::ConditionImpl()
{
	pthread_condattr_t attr;
	pthread_condattr_init(&attr))
	
	if (pthread_condattr_setclock(&attr, CLOCK_MONOTONIC))
    {
		pthread_condattr_destroy(&attr);
    }
	
	if (pthread_cond_init(&cond_, &attr))
	{
		pthread_condattr_destroy(&attr);
	}
	pthread_condattr_destroy(&attr);
}


ConditionImpl::~ConditionImpl()
{
	pthread_cond_destroy(&cond_);
}


void ConditionImpl::waitImpl(MutexImpl *m)
{
	pthread_cond_wait(&cond_, &(m->mutex_));
}


bool ConditionImpl::waitImpl(MutexImpl *m, long milliseconds)
{
	int rc = 0;
	struct timespec abstime;

	abstime.tv_sec  += milliseconds / 1000;
	abstime.tv_nsec += (milliseconds % 1000)*1000000;
	if (abstime.tv_nsec >= 1000000000)
	{
		abstime.tv_nsec -= 1000000000;
		abstime.tv_sec++;
	}

	rc = pthread_cond_timedwait(&cond_, &(m->mutex_), &abstime)))
	return rc == 0;
}


} // namespace Poco

