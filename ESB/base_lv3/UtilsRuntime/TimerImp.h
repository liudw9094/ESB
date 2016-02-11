#pragma once

#include <Utils/Thread/Timer.h>

class CThreadImp;

class CTimerImp : public Utils::Thread::ITimer
{
private:
	CThreadImp *m_pThread;
	std::list<std::function<void()>> m_lstCallFuncs;
	bool m_bEnabled;
	UINT m_uMillisecInterval;
public:
	CTimerImp(CThreadImp *pThread, UINT millisec_interval);
	~CTimerImp();

	void ExecuteAll();
public:
	virtual void AddFunc(std::function<void()> func);
	virtual void Enable(bool bEnable);
	virtual bool IsEnabled();
	virtual void Dispose();
};

