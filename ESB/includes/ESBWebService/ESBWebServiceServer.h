#ifndef _INCLUDE_ESBWEBSERVICE_ESBWEBSERVICESERVER_H_
#define _INCLUDE_ESBWEBSERVICE_ESBWEBSERVICESERVER_H_

#include <string>
#include <functional>
#include "../Utils/Thread.h"
#include "../Utils/SafeCoding/IDisposable.h"

#ifndef ESBWEBSERVICE_API
#define ESBWEBSERVICE_API
#endif

struct soap;

namespace ESBWebService
{
	class IESBWebServiceServer : public Utils::SafeCoding::IDisposable
	{
	public:
		virtual BOOL Start(int nPort) = 0;
		virtual BOOL End() = 0;
		virtual BOOL IsStarted() const = 0;
		virtual int GetPort() const = 0;
		virtual std::wstring GetClientIP(struct soap* pSoap) = 0;
		// the Invoke function would be called assynchronically.
		virtual BOOL SetEvent_Invoke(const std::function<
													int(SREF(Utils::Thread::IThread) pthread,
													struct soap* psoap,
													const std::wstring& wsSession,
													const std::wstring& wsInputs,
													std::wstring& wsResults)>& func
									) = 0;
		virtual BOOL SetEvent_Accept(const std::function<BOOL(const struct soap* sSoap)>& func) = 0;
	};

	ESBWEBSERVICE_API IESBWebServiceServer* CreateESBWebServiceServer();
};

#endif //_INCLUDE_ESBWEBSERVICE_ESBWEBSERVICESERVER_H_