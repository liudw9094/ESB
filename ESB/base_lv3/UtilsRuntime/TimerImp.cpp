#include "stdafx.h"
#include "UtilsRuntime.h"
#include "ThreadImp.h"
#include "TimerImp.h"

using namespace Utils::Thread;

CTimerImp::CTimerImp(CThreadImp *pThread, UINT millisec_interval)
	: m_pThread(pThread),
	m_uMillisecInterval(millisec_interval),
	m_bEnabled(false)
{
	ASSERT((pThread != NULL));
	ASSERT((millisec_interval > 0));
}


CTimerImp::~CTimerImp()
{
	Enable(false);
}

void CTimerImp::ExecuteAll()
{
	for (auto func : m_lstCallFuncs)
	{
		func(this);
	}
}

void CTimerImp::AddFunc(std::function<void(ITimer*)> func)
{
	m_lstCallFuncs.push_back(func);
}

void CTimerImp::Enable(bool bEnable)
{
	IThread *pThread = Utils::Thread::GetCurrentThread();
	if (pThread != m_pThread)
		throw std::runtime_error("Invalid thread object!");

	if (bEnable)
	{
		if(!m_bEnabled)
		{
			ASSERT(m_pThread->SetTimer(this, m_uMillisecInterval));
			m_bEnabled = TRUE;
		}
	}
	else
	{
		if (m_bEnabled)
		{
			m_pThread->RemoveTimer(this);
			m_bEnabled = FALSE;
		}
	}
}

bool CTimerImp::IsEnabled() const
{
	return m_bEnabled;
}

Utils::Thread::IThread* CTimerImp::GetOwnerThread()
{
	return m_pThread;
}

void CTimerImp::Dispose()
{
	delete this;
}

UTILSRUNTIME_API ITimer* Utils::Thread::CreateTimer(unsigned long millisec_interval, bool bEnable/* = true*/)
{
	IThread *pThread = Utils::Thread::GetCurrentThread();
	if (pThread == NULL)
		throw std::runtime_error("No thread object detected.");
	return pThread->NewTimer(millisec_interval, bEnable);
}
