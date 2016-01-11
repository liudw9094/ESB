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
		typedef std::function<
			int(SREF(Utils::Thread::IThread) pthread,
			struct soap* psoap,
			const std::wstring& wsSession,
			const std::wstring& wsInputs,
			std::wstring& wsResults)> TInvokeFunc;
		typedef std::function <BOOL(const struct soap* sSoap)> TAcceptFunc;

		virtual BOOL Start(int nPort) = 0;
		virtual BOOL Stop() = 0;
		virtual BOOL IsStarted() const = 0;
		virtual int GetPort() const = 0;
		virtual std::wstring&& GetClientIP(const struct soap* pSoap) const = 0;
		// the Invoke function would be called assynchronically.
		virtual BOOL SetCallback_Invoke(const TInvokeFunc& func) = 0;
		virtual BOOL SetCallback_Accept(const TAcceptFunc& func) = 0;
	};

	ESBWEBSERVICE_API IESBWebServiceServer* CreateESBWebServiceServer();
};

#endif //_INCLUDE_ESBWEBSERVICE_ESBWEBSERVICESERVER_H_