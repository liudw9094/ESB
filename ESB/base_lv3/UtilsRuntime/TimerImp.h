#pragma once

#include <Utils/Thread/Timer.h>

class CThreadImp;

class CTimerImp : public Utils::Thread::ITimer
{
private:
	CThreadImp *m_pThread;
	std::list<std::function<void(ITimer*)>> m_lstCallFuncs;
	bool m_bEnabled;
	UINT m_uMillisecInterval;
public:
	CTimerImp(CThreadImp *pThread, UINT millisec_interval);
	~CTimerImp();

	void ExecuteAll();
public:
	virtual void AddFunc(std::function<void(ITimer*)> func);
	virtual void Enable(bool bEnable);
	virtual bool IsEnabled() const;
	virtual Utils::Thread::IThread* GetOwnerThread();
	virtual void Dispose();
};

