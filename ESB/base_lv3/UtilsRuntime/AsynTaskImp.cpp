#include "stdafx.h"
#include "UtilsRuntime.h"
#include "AsynTaskImp.h"
#include "DispatcherImp.h"



CAsynTaskImp::CAsynTaskImp(const std::function<void()> &func, CDispatcherImp *pDispatcher, bool autoDispose)
	: m_func(func),
	m_evtComplete(NULL),
	m_bComplete(FALSE),
	m_bRunning(FALSE),
	m_bCancled(FALSE),
	m_pDispatcher(pDispatcher),
	m_bAutoDispose(autoDispose)
{
	m_evtComplete = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	if (m_evtComplete == NULL)
	{
		// TODO: throw an excepetion
	}
}

CAsynTaskImp::~CAsynTaskImp()
{
	::InterlockedExchange(reinterpret_cast<volatile long*>(&m_bAutoDispose), FALSE);
	if (!Cancle())
	{
		if (IsRunning())
		{
			// TODO: throw an excepetion
		}
		try
		{
			Join();
		}
		catch (...)
		{

		}
		AbortAllWaitings();
	}
}

void CAsynTaskImp::SetDispatcher(CDispatcherImp *pthread)
{
	::InterlockedExchangePointer((volatile PVOID*)(&m_pDispatcher), pthread);
}

void CAsynTaskImp::Execute()
{
	::InterlockedExchange(reinterpret_cast<volatile long*>(&m_bRunning), TRUE);
	m_func();
	SetDispatcher(NULL);
	SetComplete();
	AutoDispose();
}


void CAsynTaskImp::SetComplete()
{
	::InterlockedExchange(reinterpret_cast<volatile long*>(&m_bComplete), TRUE);
	::InterlockedExchange(reinterpret_cast<volatile long*>(&m_bRunning), FALSE);
	::SetEvent(m_evtComplete);
}

void CAsynTaskImp::AbortAllWaitings()
{
	::CloseHandle(m_evtComplete);
	m_evtComplete = NULL;
}

void CAsynTaskImp::AutoDispose()
{
	Join();
	if (m_bAutoDispose)
		delete this;
}

void CAsynTaskImp::Join()
{
	Wait(INFINITE);
}

bool CAsynTaskImp::Wait(unsigned long millisec)
{
	DWORD state = ::WaitForSingleObject(m_evtComplete, millisec);
	if (state == WAIT_OBJECT_0)
		return true;
	else if (state == WAIT_TIMEOUT)
		return false;
	else
	{
		// TODO: throw an excepetion.
		return false;
	}
}

bool CAsynTaskImp::IsCompelete()
{
	return !!m_bComplete;
}

bool CAsynTaskImp::IsRunning()
{
	return !!m_bRunning;
}

bool CAsynTaskImp::IsCancled()
{
	return !!m_bCancled;
}

bool CAsynTaskImp::Cancle()
{
	auto _Cancle = [this]()
	{
		::InterlockedExchange(reinterpret_cast<volatile long*>(&m_bCancled), TRUE);
		AbortAllWaitings();
		AutoDispose();
		return true;
	};

	if (m_pDispatcher == NULL)
	{
		// No dispatcher is attached with.
		return _Cancle();
	}
	else
	{
		if (const_cast<CDispatcherImp*>(m_pDispatcher)->CancleAWaitingTask(this))
		{
			// Task is removed from the waiting list.
			return _Cancle();
		}
		else
		{
			// Task is not in the waiting list, which means it is running or completed.
			return false;
		}
	}
}

void CAsynTaskImp::Dispose()
{
	delete this;
}