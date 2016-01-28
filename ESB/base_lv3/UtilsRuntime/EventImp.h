#pragma once

#include <Utils/Thread/Thread.h>
#include <Utils/Thread/CriticalSection.h>
#include <Utils/Thread/Event.h>

class CEventSourceImp : public Utils::Thread::IEventSource
{
private:
	mutable SREF(Utils::Thread::ICriticalSection)	m_splkLstHandlers;
	std::list<SREF(Utils::Thread::IEventHandler)>	m_lstHandlers;
	mutable SREF(Utils::Thread::IDispatcher)		m_spDispatcher;
public:
	CEventSourceImp();
	~CEventSourceImp();

	virtual void AddHandler(SREF(Utils::Thread::IEventHandler) eventHandler);
	virtual void Clear();
	virtual void InvokeAll(Utils::SafeCoding::IDisposable* psender, Utils::Thread::IArgumentSet* pargs) const;
};

class CEventHandlerImp : public Utils::Thread::IEventHandler
{
public:
	explicit CEventHandlerImp(const std::function<void(Utils::SafeCoding::IDisposable* psender, Utils::Thread::IArgumentSet* pargs)>& func);
	~CEventHandlerImp();

	virtual void Invoke(Utils::SafeCoding::IDisposable* psender, Utils::Thread::IArgumentSet* pargs) const;
};