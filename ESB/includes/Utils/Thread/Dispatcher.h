#ifndef _INCLUDES_THREAD_DISPATCHER_H_
#define _INCLUDES_THREAD_DISPATCHER_H_

#include <functional>
#include "../SafeCoding/IDisposable.h"
#include "../SafeCoding/SmtPtr.h"
#include "AsynTask.h"

#ifndef UTILSRUNTIME_API
#define UTILSRUNTIME_API
#endif // UTILSRUNTIME_API

namespace Utils
{
	namespace Thread
	{
		class IDispatcher : public Utils::SafeCoding::IDisposable
		{
		public:
			virtual void Invoke(const std::function<void()> &func) = 0;
			virtual SREF(IAsynTask) AsynInvoke(const std::function<void()> &func) = 0;
			virtual void SetPostMessageFunc(const std::function<void()> &func) = 0;
			virtual void OnMessage() = 0;
			virtual bool IsAccessible() = 0;
		};

		UTILSRUNTIME_API IDispatcher* CreateDispatcher();
	}
};

#endif //_INCLUDES_THREAD_DISPATCHER_H_