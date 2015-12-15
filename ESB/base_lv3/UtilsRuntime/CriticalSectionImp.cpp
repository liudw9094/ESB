#include "stdafx.h"
#include "UtilsRuntime.h"
#include "CriticalSectionImp.h"


CCriticalSectionImp::CCriticalSectionImp()
{
	::InitializeCriticalSection(&cs);
}

CCriticalSectionImp::~CCriticalSectionImp()
{
	::DeleteCriticalSection(&cs);
}

void CCriticalSectionImp::Lock()
{
	EnterCriticalSection(&cs);
}

void CCriticalSectionImp::Unlock()
{
	LeaveCriticalSection(&cs);
}

bool CCriticalSectionImp::TryLock(unsigned long ms_timeout)
{
	return !!TryEnterCriticalSection(&cs);
}

void CCriticalSectionImp::Dispose()
{
	delete this;
}

namespace Utils
{
	namespace Thread
	{
		UTILSRUNTIME_API ICriticalSection* CreateCriticalSection()
		{
			return new CCriticalSectionImp;
		}
	};
}