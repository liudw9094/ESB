#ifndef _INCLUDES_THREAD_SCOPELOCK_H_
#define _INCLUDES_THREAD_SCOPELOCK_H_

#include "ILockable.h"

#ifndef THREAD_API
#define THREAD_API
#endif //THREAD_API


class CScopeLock
{
	ILockable &_lock;
public:
	inline CScopeLock(ILockable &lock)
		: _lock(lock)
	{
		_lock.Lock();
	}
	inline ~CScopeLock()
	{
		_lock.Unlock();
	}
}; 

#define __SCOPELOCK(cs,suffix) CScopeLock stLock_##suffix##(cs)
#define _SCOPELOCK(cs,suffix) __SCOPELOCK(cs,suffix)
#define SCOPELOCK(cs) _SCOPELOCK(cs,__COUNTER__)

#define SLOCK(s) SCOPELOCK(s)


#endif // _INCLUDES_THREAD_SCOPELOCK_H_