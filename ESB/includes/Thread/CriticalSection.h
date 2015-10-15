#ifndef _INCLUDES_THREAD_CRITICAL_SECTION_H_
#define _INCLUDES_THREAD_CRITICAL_SECTION_H_
#include "ILockable.h"

#ifndef THREAD_API
#define THREAD_API
#endif //THREAD_API

namespace Thread
{
	class ICriticalSection : public ILockable
	{
	public:
		virtual bool TryLock(unsigned long reserved = 0) = 0;
	};

	THREAD_API ICriticalSection* CreateCriticalSection();

}
#endif //_INCLUDES_THREAD_CRITICAL_SECTION_H_