#ifndef _INCLUDE_THREAD_ILOCKABLE_H_
#define _INCLUDE_THREAD_ILOCKABLE_H_

#include "../Utils/SafeCoding/IDisposable.h"

class ILockable : public IDisposable
{
public:
	virtual void Lock() = 0;
	virtual void Unlock() = 0;
	virtual bool TryLock(unsigned long ms_timeout) = 0;
};

#endif // _INCLUDE_THREAD_ILOCKABLE_H_