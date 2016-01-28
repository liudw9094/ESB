#pragma once

#include <Utils/Thread/Dispatcher.h>

#include "CriticalSectionImp.h"
#include "AsynTaskImp.h"

class CDispatcherImp : public Utils::Thread::IDispatcher
{
private:
	std::function<BOOL (DWORD dwThreadID)>		m_funcPostMessage;
	std::deque<SREF(Utils::Thread::IAsynTask)>	m_dqTasks;
	CCriticalSectionImp							m_csdqTasks;
	DWORD										m_dwOwnerThreadID;
public:
	explicit CDispatcherImp(const std::function<BOOL(DWORD dwThreadID)> &funcPostMessage);
	~CDispatcherImp();
public:
	virtual bool CancleAWaitingTask(const CAsynTaskImp* task);
public:
	virtual void Invoke(const std::function<void()> &func);
	virtual SREF(Utils::Thread::IAsynTask) AsynInvoke(const std::function<void()> &func);
	virtual void OnMessage();
	virtual bool IsAccessibleDirectly() const;
	virtual void CleanUpAllTasks();
	virtual void Dispose();
};

