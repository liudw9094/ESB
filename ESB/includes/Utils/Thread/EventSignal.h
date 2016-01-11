#ifndef _INCLUDES_THREAD_EVENTSIGNAL_H_
#define _INCLUDES_THREAD_EVENTSIGNAL_H_

#include "ILockable.h"

#ifndef UTILSRUNTIME_API
#define UTILSRUNTIME_API
#endif // UTILSRUNTIME_API

namespace Utils
{
	namespace Thread
	{
		class IEventSignal
		{
		public:
			virtual void Signal() = 0;
			virtual void Unsignal() = 0;
			virtual BOOL Wait(unsigned long millisec) = 0;
		};

		UTILSRUNTIME_API IEvent* CreateEventSignal();
	}
};

#endif //_INCLUDES_THREAD_EVENTSIGNAL_H_