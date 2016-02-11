/*
 *  File Name: includes/Thread/Thread.h
 *  Author: Dawei Liu
 *  Date: 10/14/2015
 *  Description:
 *  License:
 *
 */

#ifndef _INCLUDES_UTILS_THREAD_H_
#define _INCLUDES_UTILS_THREAD_H_

#include <functional>
#include "../SafeCoding/IDisposable.h"
#include "../SafeCoding/SmtPtr.h"
#include "AsynTask.h"
#include "Dispatcher.h"
#include "Timer.h"

#ifndef UTILSRUNTIME_API
#define UTILSRUNTIME_API
#endif //UTILSRUNTIME_API

namespace Utils
{
	namespace Thread
	{
		class IThread : public Utils::SafeCoding::IDisposable
		{
		public:
			typedef std::function<void(IThread* pThread)> THREAD_CALLBACK;
		public:
			virtual void Invoke(const std::function<void()> &func) = 0;
			virtual SREF(IAsynTask) AsynInvoke(const std::function<void()> &func) = 0;
			virtual void DoEvents() = 0;
			virtual SREF(IDispatcher) GetDispatcher() = 0;
			virtual ITimer* NewTimer(unsigned long millisec_interval, bool bEnable = true) = 0;
		};

		UTILSRUNTIME_API IThread* CreateThread(const IThread::THREAD_CALLBACK& onInit = nullptr,
			const IThread::THREAD_CALLBACK& onFinish = nullptr);
		UTILSRUNTIME_API IThread* GetCurrentThread();
	};
};

#endif // _INCLUDES_UTILS_THREAD_H_