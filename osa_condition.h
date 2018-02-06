#ifndef OSA_Event_POSIX_INCLUDED
#define OSA_Event_POSIX_INCLUDED

#include <pthread.h>
#include <errno.h>

namespace OSA
{

class  ConditionImpl
{
public:
	ConditionImpl();		
	~ConditionImpl();
	void setImpl();
	void waitImpl(MutexImpl *m);
	bool waitImpl(MutexImpl *m, long milliseconds);
	
private:
	pthread_cond_t  cond_;
};

//
// inlines
//
inline void ConditionImpl::setImpl()
{
	pthread_cond_broadcast(&cond_);
}


}// namespace Poco


#endif // Foundation_Event_POSIX_INCLUDED

