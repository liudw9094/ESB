#ifndef _INCLUDE_ESBMIDSERVICE_SERVICE_H_
#define _INCLUDE_ESBMIDSERVICE_SERVICE_H_

#include <string>

#ifdef _WINDOWS
//#include <Rpcdce.h>
#endif //_WINDOWS

#include "../Utils/SafeCoding/IDisposable.h"
#include "../Utils/SafeCoding/SmtPtr.h"
#include "ESBMidService_HubConnection.h"

#ifndef ESBMIDSERVICE_API
#define ESBMIDSERVICE_API
#endif // ESBMIDSERVICE_API

namespace ESBMidService
{
	class IESBService : Utils::SafeCoding::IDisposable
	{
	public:
		virtual int Start(int nPort) = 0;
		virtual int End(void) = 0;
		virtual BOOL IsStarted(void) const = 0;
		virtual int	RegisterToHub(const std::wstring& wsHubURL,
								const std::wstring& wsServiceURL,
								const GUID guidService,
								const std::wstring& wsServiceName,
								UINT maximumSession) = 0;
		virtual int GetPort(void) const = 0;
		virtual IESBServiceHubConnection* GetHubConnection() = 0;
	};

	ESBMIDSERVICE_API IESBService* CreateESBService();
};

#endif //_INCLUDE_ESBMIDSERVICE_SERVICE_H_