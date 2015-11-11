#ifndef _INCLUDES_UTILS_ASYNTASK_H_
#define _INCLUDES_UTILS_ASYNTASK_H_

#include "../SafeCoding/IDisposable.h"

namespace Utils
{
	namespace Thread
	{
		class IAsynTask : public Utils::SafeCoding::IDisposable
		{
		public:
			virtual void Join() = 0;
			virtual bool Wait(unsigned long millisec) = 0;
			virtual bool IsCompelete() = 0;
			virtual bool IsRunning() = 0;
			virtual bool IsCancled() = 0;
			virtual bool Cancle() = 0;
		};
	};
};

#endif _INCLUDES_UTILS_ASYNTASK_H_