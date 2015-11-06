#ifndef _INCLUDES_THREAD_CRITICAL_SECTION_H_
#define _INCLUDES_THREAD_CRITICAL_SECTION_H_
#include "ILockable.h"

#ifndef UTILSRUNTIME_API
#define UTILSRUNTIME_API
#endif // UTILSRUNTIME_API

namespace Utils
{
	namespace Thread
	{
		class ICriticalSection : public ILockable
		{
		public:
			virtual bool TryLock(unsigned long reserved = 0) = 0;
		};

		UTILSRUNTIME_API ICriticalSection* CreateCriticalSection();

	};
};

#endif //_INCLUDES_THREAD_CRITICAL_SECTION_H_