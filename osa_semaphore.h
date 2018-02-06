#ifndef OSA_Semaphore_POSIX_INCLUDED
#define OSA_Semaphore_POSIX_INCLUDED

#include <pthread.h>
#include <errno.h>


namespace OSA 
{


class  SemaphoreImpl
{
protected:
	SemaphoreImpl(int n, int max);		
	~SemaphoreImpl();
	void setImpl();
	void waitImpl();
	bool waitImpl(long milliseconds);
	
private:
	volatile int    _n;
	int             _max;
	pthread_mutex_t _mutex;
	pthread_cond_t  _cond;
};

inline void SemaphoreImpl::setImpl()
{
	pthread_mutex_lock(&_mutex);
	if (_n < _max)
	{
		++_n;
	}
	else
	{
		pthread_mutex_unlock(&_mutex);
	}	
	if (pthread_cond_signal(&_cond))
	{
		pthread_mutex_unlock(&_mutex);
	}
	pthread_mutex_unlock(&_mutex);
}


} // namespace Poco


#endif 

