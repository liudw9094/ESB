#ifndef _INCLUDES_THREAD_CRITICAL_SECTION_H_
#define _INCLUDES_THREAD_CRITICAL_SECTION_H_

#include <windows.h>
#include "ILockable.h"

#ifndef THREAD_API
#define THREAD_API
#endif //THREAD_API

namespace Thread
{
	class IEvent
	{
	public:
		virtual void Signal() = 0;
		virtual void Unsignal() = 0;
		virtual BOOL Wait(unsigned long millisec) = 0;
	};

	THREAD_API IEvent* CreateEvent();
}
#endif //_INCLUDES_THREAD_CRITICAL_SECTION_H_