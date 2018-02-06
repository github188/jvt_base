#ifndef OSA_RWLock_POSIX_INCLUDED
#define OSA_RWLock_POSIX_INCLUDED


#include <pthread.h>
#include <errno.h>


namespace OSA 
{


class  RWLockImpl
{
public:
	RWLockImpl();
	~RWLockImpl();
	void readLockImpl();
	bool tryReadLockImpl();
	void writeLockImpl();
	bool tryWriteLockImpl();
	void unlockImpl();
	
private:
	pthread_rwlock_t rwl_;
};


//
// inlines
//
inline void RWLockImpl::readLockImpl()
{
	pthread_rwlock_rdlock(&rwl_);
}


inline bool RWLockImpl::tryReadLockImpl()
{
	int rc = pthread_rwlock_tryrdlock(&rwl_);
	if (rc == 0)
		return true;
	else if(rc == EBUSY)
		return false;
}


inline void RWLockImpl::writeLockImpl()
{
	pthread_rwlock_wrlock(&rwl_);
}


inline bool RWLockImpl::tryWriteLockImpl()
{
	int rc = pthread_rwlock_trywrlock(&rwl_);
	if (rc == 0)
		return true;
	else if (rc == EBUSY)
		return false;
}


inline void RWLockImpl::unlockImpl()
{
	pthread_rwlock_unlock(&rwl_);
}


} 


#endif 

