#ifndef _INCLUDE_ESBWEBSERVICE_ESBWEBSERVICESERVER_H_
#define _INCLUDE_ESBWEBSERVICE_ESBWEBSERVICESERVER_H_

#include <string>
#include <functional>
#include "../Utils/Thread.h"
#include "../Utils/SafeCoding/IDisposable.h"
#include "Authenticate.h"

#ifndef ESBWEBSERVICE_API
#define ESBWEBSERVICE_API
#endif

struct soap;

namespace ESBWebService
{

	class IESBWebServiceServer : public Utils::SafeCoding::IDisposable
	{
	public:
		typedef std::function<
			int(SREF(Utils::Thread::IThread) pthread,
			struct soap* psoap,
			const std::wstring& wsSession,
			const std::wstring& wsInputs,
			std::wstring& wsResults)> TOnClientInvokeFunc;
		typedef std::function <BOOL(struct soap* sSoap)> TOnAcceptFunc;
		typedef std::function <void(IESBWebServiceServer* sender)> TOnStartFunc;
		typedef std::function <void(IESBWebServiceServer* sender)> TOnStopFunc;

		virtual BOOL Start(int nPort, const SAuthentication *pAuthentication = NULL) = 0;
		virtual BOOL Stop() = 0;
		virtual BOOL IsStarted() const = 0;
		virtual int GetPort() const = 0;
		virtual std::wstring GetClientIP(const struct soap* pSoap) const = 0;
		// the Invoke function would be called assynchronically.
		virtual BOOL SetCallback_OnClientInvoke(const TOnClientInvokeFunc& func) = 0;
		virtual BOOL SetCallback_OnAccept(const TOnAcceptFunc& func) = 0;
		virtual BOOL SetCallback_OnStarted(const TOnStartFunc& func) = 0;
		virtual BOOL SetCallback_OnStoped(const TOnStopFunc& func) = 0;
	};

	ESBWEBSERVICE_API IESBWebServiceServer* CreateESBWebServiceServer();
};

#endif //_INCLUDE_ESBWEBSERVICE_ESBWEBSERVICESERVER_H_