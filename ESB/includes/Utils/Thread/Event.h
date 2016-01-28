#ifndef _INCLUDES_UTILS_EVENT_H_
#define _INCLUDES_UTILS_EVENT_H_

#include <functional>
#include "../SafeCoding/IDisposable.h"
#include "../SafeCoding/SmtPtr.h"
#include "IArgumentSet.h"

#ifndef UTILSRUNTIME_API
#define UTILSRUNTIME_API
#endif // UTILSRUNTIME_API

namespace Utils
{
	namespace Thread
	{
		class IEventHandler;

		class IEventSource : public Utils::SafeCoding::IDisposable
		{
		public:
			virtual void AddHandler(SREF(IEventHandler) eventHandler) = 0;
			virtual void Clear() = 0;
			virtual void InvokeAll(Utils::SafeCoding::IDisposable* psender, IArgumentSet* pargs) const = 0;
		};

		UTILSRUNTIME_API IEventSource* CreateEventSource();


		class IEventHandler : public Utils::SafeCoding::IDisposable
		{
		public:
			virtual void Invoke(Utils::SafeCoding::IDisposable* psender, IArgumentSet* pargs) const = 0;
		};

		UTILSRUNTIME_API IEventHandler* CreateEventHandler(const std::function<void(Utils::SafeCoding::IDisposable* psender, IArgumentSet* pargs)>& func);

		template <class TARGS> class TEventHandler;
		template <class TEVENTHANDLER, class TARGS = TEVENTHANDLER::ARGSTYPE> class TEvent;

		template <class TARGS>
		class TEventHandler
		{
			SREF(IEventHandler) m_evtHandler;
		public:
			typedef TARGS ARGSTYPE;
		public:
			TEventHandler(const std::function<void(Utils::SafeCoding::IDisposable* psender, TARGS* pargs)> &func)
			{
				auto func_mock = [func](Utils::SafeCoding::IDisposable* psender, IArgumentSet* pargs) {
					func(psender, static_cast<TARGS*>(pargs))
				}
				m_evtHandler = CreateEventHandler(func_mock);
			};
			void operator ()(Utils::SafeCoding::IDisposable* psender, TARGS* pargs)
			{
				m_evtHandler->Invoke(psender, pargs);
			}
			operator SREF(IEventSource*) ()
			{
				return m_evtHandler;
			}
		};

		template <class TEVENTHANDLER, class TARGS /*= TEVENTHANDLER::ARGSTYPE*/>
		class TEvent
		{
		private:
			SREF(IEventSource) m_spEventSource;
		public:
			TEvent()
				: m_spEventSource(CreateEventSource())
			{

			}
			TEvent& operator += (const TEVENTHANDLER &handler)
			{
				m_spEventSource->AddHandler(*handler);
				return *this;
			}
			void operator ()(Utils::SafeCoding::IDisposable* psender, TARGS* pargs)
			{
				m_spEventSource->InvokeAll(psender, pargs);
			}
			operator SREF(IEventSource*) ()
			{
				return m_spEventSource;
			}
			IEventSource* operator -> ()
			{
				return m_spEventSource;
			}
		};
	};
};

#endif //_INCLUDES_UTILS_EVENT_H_