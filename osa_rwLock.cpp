#include "rwLock.h"

namespace OSA {


RWLockImpl::RWLockImpl()
{
	pthread_rwlock_init(&_rwl, NULL);
}


RWLockImpl::~RWLockImpl()
{
	pthread_rwlock_destroy(&_rwl);
}


} // namespace Poco

