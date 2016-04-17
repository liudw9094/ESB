#ifndef _INCLUDE_THREAD_ILOCKABLE_H_
#define _INCLUDE_THREAD_ILOCKABLE_H_


#include <functional>
#include "../SafeCoding/IDisposable.h"
#include "../SafeCoding/Finalize.h"

namespace Utils
{
	namespace Thread
	{
		class ILockable : public Utils::SafeCoding::IDisposable
		{
		public:
			void LockOperation(std::function<void()> func)
			{
				Lock();
				SafeCoding::CFinalize fin([this] {
					Unlock();
				});
				func();
			};
		public:
			virtual void Lock() = 0;
			virtual void Unlock() = 0;
			virtual bool TryLock(unsigned long ms_timeout) = 0;
		};
	};
};

#endif // _INCLUDE_THREAD_ILOCKABLE_H_