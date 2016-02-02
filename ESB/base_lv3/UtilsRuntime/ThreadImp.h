#pragma once

#include <functional>
#include <Utils/Thread/ScopeLock.h>
#include <Utils/Thread/Thread.h>
#include <Utils/Thread/Dispatcher.h>
#include "CriticalSectionImp.h"
#include "AsynTaskImp.h"

class CThreadImp : public Utils::Thread::IThread
{
protected:
	static const UINT			MSG_INVOKE;
	HANDLE						m_hThread;
	unsigned int				m_nThreadID;
	// Signal to determine if the parameter has already recieved by the thread created.
	HANDLE						m_hThreadParamSig;
	// Signal for m_hThread and m_nThreadID created.
	HANDLE						m_hhThread;

	SREF(Utils::Thread::IDispatcher)			m_spDispatcher;

	THREAD_CALLBACK				m_callback_OnInit;
	THREAD_CALLBACK				m_callback_OnFinish;
public:
	struct Init_AttachCurrentThread
	{
	};
public:
	CThreadImp(const IThread::THREAD_CALLBACK& onInit = nullptr,
		const IThread::THREAD_CALLBACK& onFinish = nullptr);
	explicit CThreadImp(Init_AttachCurrentThread);
	virtual ~CThreadImp();
public:
	//virtual bool CancleAWaitingTask(const CAsynTaskImp* task);
protected:
	// overwrites
	virtual unsigned int Run();
	virtual void DispatchInvoke(WPARAM wparam, LPARAM lparam);
protected:
	static unsigned int _stdcall _Run(CThreadImp* This);
	//void CleanUpTasks();
public:
	// Thread::IThread
	virtual void Invoke(const std::function<void()> &func);
	virtual SREF(Utils::Thread::IAsynTask) AsynInvoke(const std::function<void()> &func);
	virtual void DoEvents();
	virtual SREF(Utils::Thread::IDispatcher) GetDispatcher();
	//virtual bool CancleTask(Utils::Thread::IAsynTask* task);
	virtual void Dispose();
};

class CExpWMQuit : public std::exception
{
public:
	virtual const char* what() const throw()
	{
		return "WM_QUIT Message.";
	}
};
