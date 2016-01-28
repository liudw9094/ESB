#ifndef _INCLUDES_THREAD_APPLICATION_H_
#define _INCLUDES_THREAD_APPLICATION_H_

#ifndef UTILSRUNTIME_API
#define UTILSRUNTIME_API
#endif // UTILSRUNTIME_API

#include "Thread.h"

namespace Utils
{
	namespace Thread
	{
		class CApplicationImp;

		class UTILSRUNTIME_API CApplication : public IThread
		{
		private:
			CApplicationImp *m_pClassImp;
		public:
			CApplication();
			virtual ~CApplication();

			virtual UINT Run();

			virtual void Invoke(const std::function<void()> &func);
			virtual SREF(IAsynTask) AsynInvoke(const std::function<void()> &func);
			virtual void DoEvents();
			virtual SREF(IDispatcher) GetDispatcher();
			virtual void Dispose();
		protected:
			virtual void OnMessage();
			virtual BOOL OnInitialization()
			{
				return TRUE;
			}
			virtual void OnFinalization()
			{
			}
		};
	};
};

#endif //_INCLUDES_THREAD_APPLICATION_H_