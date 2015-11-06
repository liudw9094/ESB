#ifndef _INCLUDES_THREAD_CRITICAL_SECTION_H_
#define _INCLUDES_THREAD_CRITICAL_SECTION_H_

#include <windows.h>
#include "ILockable.h"

#ifndef UTILSRUNTIME_API
#define UTILSRUNTIME_API
#endif // UTILSRUNTIME_API

namespace Utils
{
	namespace Thread
	{
		class IEvent
		{
		public:
			virtual void Signal() = 0;
			virtual void Unsignal() = 0;
			virtual BOOL Wait(unsigned long millisec) = 0;
		};

		UTILSRUNTIME_API IEvent* CreateEvent();
	}
};

#endif //_INCLUDES_THREAD_CRITICAL_SECTION_H_