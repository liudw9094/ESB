#ifndef _INCLUDE_ESBMIDSERVICE_SERVICE_H_
#define _INCLUDE_ESBMIDSERVICE_SERVICE_H_

#include <string>

#ifdef _WINDOWS
//#include <Rpcdce.h>
#endif //_WINDOWS

#include "../Utils/SafeCoding/IDisposable.h"
#include "../Utils/SafeCoding/SmtPtr.h"
#include "../ESBWebService/ESBWebServiceServer.h"
#include "../Common/ESBHub.h"
#include "ESBMidService_HubConnection.h"

#include <functional>

#ifndef ESBMIDSERVICE_API
#define ESBMIDSERVICE_API
#endif // ESBMIDSERVICE_API

namespace ESBMidService
{
	class IESBService : public ESBWebService::IESBWebServiceServer
	{
	public:
		typedef std::function<int(SREF(Utils::Thread::IThread) pthread,
		struct soap* psoap,
			IN OUT std::wstring& wsSession,
			IN OUT std::wstring& wsInputs,
			OUT BOOL& bNoFurtherProcess,
			OUT std::wstring& wsResults,
			ESBCommon::ENUM_IDTYPE &idType)> TOnPreInvokeFunc;

		virtual BOOL SetCallback_OnPreInvoke(const TOnPreInvokeFunc &func) = 0;

		virtual int	RegisterToHub(const std::wstring& wsHubURL,
								const std::wstring& wsServiceURL,
								const GUID guidService,
								const std::wstring& wsServiceName,
								UINT maximumSession) = 0;
		virtual IESBServiceHubConnection* GetHubConnection() = 0;
		virtual BOOL CheckClientSession(const std::wstring& wsSession) = 0;
		virtual BOOL CheckHubSession(const std::wstring& wsSession) = 0;
	};

	ESBMIDSERVICE_API IESBService* CreateESBService();
};

#endif //_INCLUDE_ESBMIDSERVICE_SERVICE_H_