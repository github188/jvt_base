#ifndef OSA_MUTEX_POSIX_INCLUDED
#define OSA_MUTEX_POSIX_INCLUDED


#include <pthread.h>
#include <errno.h>

namespace OSA 
{
class PosixMutex {

	PosixMutex(const PosixMutex&);
	PosixMutex& operator=(const PosixMutex&);

	pthread_mutex_t mutex_;

	friend class PosixCondition;

public:
	PosixMutex();
	~PosixMutex();

	/**
	 * \brief Acquires the lock.
	 *
	 * If the mutex is busy the calling thread is blocked.
	 */
	void lock() {
		pthread_mutex_lock(&mutex_);
	}

	/**
	 * \brief Releases the lock.
	 */
	void unlock() {
		pthread_mutex_unlock(&mutex_);
	}

	bool tryLock();
};

/**
 * \brief Class to simplify locking and unlocking of PosixMutex
 *
 * This is a convenience class that simplifies locking and unlocking
 * PosixMutex(es) making use of the RAII idiom: the lock is acquired in the
 * constructor and released in the destructor.
 */
class MutexLocker {

	PosixMutex& mutex_;

public:

	MutexLocker(PosixMutex& mutex): mutex_(mutex) {
		mutex_.lock();
	}

	~MutexLocker() {
		mutex_.unlock();
	}

};

/**
 * \brief Class to simplify locking and unlocking of pthread mutex.
 *
 * This class has the same purpose of the MutexLocker class but works
 * with pthread mutex type instead of PosixMutex class.
 */
class PthreadMutexLocker {

	pthread_mutex_t& mutex_;

public:

	PthreadMutexLocker(pthread_mutex_t& mutex): mutex_(mutex) {
		pthread_mutex_lock(&mutex_);
	}

	~PthreadMutexLocker() {
		pthread_mutex_unlock(&mutex_);
	}

};


} // namespace Poco


#endif

