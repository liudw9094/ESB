#pragma once

#include <Thread/CriticalSection.h>

class CCriticalSectionImp : public Thread::ICriticalSection
{
private:
	CRITICAL_SECTION	cs;
public:
	CCriticalSectionImp();
	~CCriticalSectionImp();
public:
	void Lock();
	void Unlock();
	bool TryLock(unsigned long reserved = 0);
	void Dispose();
};

