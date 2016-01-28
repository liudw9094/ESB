#include "stdafx.h"
#include "UtilsRuntime.h"
#include <Utils/Thread/ScopeLock.h>
#include "DispatcherImp.h"

using namespace std;
using namespace Utils::Thread;

static thread_local IDispatcher* s_pCurDispatcher = NULL;

CDispatcherImp::CDispatcherImp(const std::function<BOOL(DWORD dwThreadID)> &funcPostMessage)
	: m_funcPostMessage(funcPostMessage),
	m_dwOwnerThreadID(::GetCurrentThreadId())
{
}


CDispatcherImp::~CDispatcherImp()
{
	CleanUpAllTasks();
}

bool CDispatcherImp::CancleAWaitingTask(const CAsynTaskImp* task)
{
	SLOCK(&m_csdqTasks);
	CAsynTaskImp *pTask = NULL;
	for (auto cur = m_dqTasks.begin(); cur != m_dqTasks.end(); ++cur)
	{
		pTask = static_cast<CAsynTaskImp*>((::Utils::Thread::IAsynTask*)(*cur));
		if (pTask == task)
		{
			pTask->SetDispatcher(NULL);
			m_dqTasks.erase(cur);
			return true;
		}
	}
	return false;
}

void CDispatcherImp::Invoke(const std::function<void()> &func)
{
	if (::GetCurrentThreadId() == m_dwOwnerThreadID)
		func();
	else
	{
		auto task = AsynInvoke(func);
		task->Join();
	}
}

SREF(Utils::Thread::IAsynTask) CDispatcherImp::AsynInvoke(const std::function<void()> &func)
{
	SREF(Utils::Thread::IAsynTask) spTask = new CAsynTaskImp(func, this, false);
	if (spTask == NULL)
	{
		throw std::runtime_error("Failed to create a task.");;
	}
	else
	{
		const int nTryCount = 3;
		int nTry = 0;
		for (nTry = 0; nTry < nTryCount; ++nTry, Sleep(100))
		{
			SLOCK(&m_csdqTasks);
			m_dqTasks.push_back(spTask);
			if (!m_funcPostMessage(m_dwOwnerThreadID))
				m_dqTasks.pop_back();
			else
				break;
		}
		if (nTry >= nTryCount)
		{
			throw std::runtime_error("Failed to post the thread a message.");
		}
		return spTask;
	}
}

void CDispatcherImp::OnMessage()
{
	if (!IsAccessibleDirectly())
		throw std::runtime_error("Method OnMessage() should be called in the dispatcher's owner thread.");

	SREF(Utils::Thread::IAsynTask) spTask;
	CAsynTaskImp *pTask = NULL;
	{
		SLOCK(&m_csdqTasks);
		if (m_dqTasks.empty())
			return;
		spTask = (*m_dqTasks.begin());
		pTask = static_cast<CAsynTaskImp*>((::Utils::Thread::IAsynTask*)(spTask));
		m_dqTasks.pop_front();
	}
	if (pTask != NULL)
		pTask->Execute();
}

bool CDispatcherImp::IsAccessibleDirectly() const
{
	return GetCurrentThreadId() == m_dwOwnerThreadID;
}

void CDispatcherImp::CleanUpAllTasks()
{
	SLOCK(&m_csdqTasks);
	CAsynTaskImp *pTask = NULL;
	while (!m_dqTasks.empty())
	{
		pTask = static_cast<CAsynTaskImp*>((::Utils::Thread::IAsynTask*)(m_dqTasks.back()));
		pTask->SetDispatcher(NULL);
		pTask->Cancle();
		m_dqTasks.pop_back();
	}
}

void CDispatcherImp::Dispose()
{
	if (!IsAccessibleDirectly())
		throw runtime_error("Method IDispatcher::Dispose() cannot be called "
			"other than by the dispatcher's owner thread.");
	else
		delete this;
}

UTILSRUNTIME_API IDispatcher* Utils::Thread::CreateDispatcher(const std::function<BOOL(DWORD dwThreadID)> &funcPostMessage)
{
	return new CDispatcherImp(funcPostMessage);
}
