#include "stdafx.h"
#include "UtilsRuntime.h"
#include "EventImp.h"

using namespace std;
using namespace Utils::Thread;
using namespace Utils::SafeCoding;

// CEventSourceImp

CEventSourceImp::CEventSourceImp()
	: m_splkLstHandlers(::CreateCriticalSection())
{
	IThread *pThread = Utils::Thread::GetCurrentThread();
	if (pThread == NULL)
	{
		throw runtime_error("No thread object in current thread.");
	}
	m_spDispatcher = pThread->GetDispatcher();
}

CEventSourceImp::~CEventSourceImp()
{
	Clear();
}

void CEventSourceImp::AddHandler(SREF(IEventHandler) eventHandler)
{
	SLOCK(m_splkLstHandlers);
	m_lstHandlers.push_back(eventHandler);
}

void CEventSourceImp::Clear()
{
	SLOCK(m_splkLstHandlers);
	m_lstHandlers.clear();
}

void CEventSourceImp::InvokeAll(IDisposable* psender, IArgumentSet* pargs) const
{
	m_spDispatcher->Invoke([this, psender, pargs]() {
		SLOCK(m_splkLstHandlers);
		for (auto i = m_lstHandlers.begin(); i != m_lstHandlers.end(); ++i)
			(*i)->Invoke(psender, pargs);
	});
}


// CEventHandlerImp

CEventHandlerImp::CEventHandlerImp(const std::function<void(IDisposable* psender, IArgumentSet* pargs)>& func)
{

}

CEventHandlerImp::~CEventHandlerImp()
{

}

void CEventHandlerImp::Invoke(IDisposable* psender, IArgumentSet* pargs) const
{

}
