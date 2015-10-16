#ifndef _INCLUDES_UTILS_ASYNTASK_H_
#define _INCLUDES_UTILS_ASYNTASK_H_

#include "../Utils/SafeCoding/IDisposable.h"

namespace Thread
{
	class IAsynTask : public IDisposable
	{
		// following methods would be unsafe if AutoDispose is enabled.
	public:
		virtual void Join() = 0;
		virtual bool Wait(unsigned long millisec) = 0;
		virtual bool IsCompelete() = 0;
		virtual bool IsRunning() = 0;
		virtual bool IsCancled() = 0;
		virtual bool Cancle() = 0;
	};
};

#endif _INCLUDES_UTILS_ASYNTASK_H_