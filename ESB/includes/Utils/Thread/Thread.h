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
#include "AsynTask.h"

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
			virtual void Invoke(const std::function<void()> &func) = 0;
			virtual IAsynTask* AsynInvoke(const std::function<void()> &func, bool autoDispose = true) = 0;
			// A safe method to cancle a task.
			virtual bool CancleTask(IAsynTask* task) = 0;
		};

		UTILSRUNTIME_API IThread* CreateThread();
		UTILSRUNTIME_API IThread* GetCurrentThread();
	};
};

#endif // _INCLUDES_UTILS_THREAD_H_