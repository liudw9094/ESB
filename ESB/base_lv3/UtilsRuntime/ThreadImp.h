#pragma once

#include <functional>
#include "CriticalSectionImp.h"
#include "AsynTaskImp.h"
#include <Utils/Thread/ScopeLock.h>
#include <Utils/Thread/Thread.h>

class CThreadImp : public Utils::Thread::IThread
{
protected:

protected:
	static const UINT			MSG_INVOKE;
	HANDLE						m_hThread;
	unsigned int				m_nThreadID;
	// Signal to determine if the parameter has already recieved by the thread created.
	HANDLE						m_hThreadParamSig;
	// Signal for m_hThread and m_nThreadID created.
	HANDLE						m_hhThread;

	std::deque<SREF(Utils::Thread::IAsynTask)>	m_dqTasks;
	CCriticalSectionImp							m_csdqTasks;
public:
	CThreadImp();
	virtual ~CThreadImp();
public:
	virtual bool CancleAWaitingTask(const CAsynTaskImp* task);
protected:
	// overwrites
	virtual unsigned int Run();
	virtual void DispatchInvoke(WPARAM wparam, LPARAM lparam);
protected:
	static unsigned int _stdcall _Run(CThreadImp* This);
	void CleanUpTasks();
public:
	// Thread::IThread
	virtual void Invoke(const std::function<void()> &func);
	virtual SREF(Utils::Thread::IAsynTask) AsynInvoke(const std::function<void()> &func);
	virtual void DoEvents();
	//virtual bool CancleTask(Utils::Thread::IAsynTask* task);
	virtual void Dispose();
};