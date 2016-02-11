#ifndef _INCLUDES_UTILS_TIMER_H_
#define _INCLUDES_UTILS_TIMER_H_

#include "../SafeCoding/IDisposable.h"
#include "../SafeCoding/SmtPtr.h"
#include <functional>

#ifndef UTILSRUNTIME_API
#define UTILSRUNTIME_API
#endif // UTILSRUNTIME_API

namespace Utils
{
	namespace Thread
	{
		class ITimer : public SafeCoding::IDisposable
		{
		public:
			virtual void AddFunc(std::function<void()> func) = 0;
			virtual void Enable(bool bEnable) = 0;
			virtual bool IsEnabled() = 0;
		};

		UTILSRUNTIME_API ITimer* CreateTimer(unsigned long millisec_interval, bool bEnable = true);
	};
};

#endif //_INCLUDES_UTILS_TIMER_H_