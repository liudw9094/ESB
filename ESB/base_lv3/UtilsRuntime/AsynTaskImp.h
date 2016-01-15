#pragma once

#include <Utils/Thread/AsynTask.h>

class CDispatcherImp;

class CAsynTaskImp : public Utils::Thread::IAsynTask
{
private:
	const std::function<void()> m_func;
	HANDLE m_evtComplete;
	volatile BOOL m_bComplete;
	volatile BOOL m_bRunning;
	volatile BOOL m_bCancled;
	volatile CDispatcherImp *m_pDispatcher;
	volatile BOOL m_bAutoDispose;
public:
	CAsynTaskImp(const std::function<void()> &func, CDispatcherImp *pDispatcher, bool autoDispose);
	~CAsynTaskImp();
public:
	void SetDispatcher(CDispatcherImp *pDispatcher);
	void Execute();
protected:
	void SetComplete();
	void AbortAllWaitings();
	void AutoDispose();
public:
	virtual void Join();
	virtual bool Wait(unsigned long millisec);
	virtual bool IsCompelete();
	virtual bool IsRunning();
	virtual bool IsCancled();
	virtual bool Cancle();
	virtual void Dispose();
};

