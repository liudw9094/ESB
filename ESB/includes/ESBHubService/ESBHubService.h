#ifndef _INCLUDE_ESBMIDSERVICE_HUBSERVICE_H_
#define _INCLUDE_ESBMIDSERVICE_HUBSERVICE_H_

#include "../Utils/SafeCoding/IDisposable.h"
#include "../Utils/SafeCoding/SmtPtr.h"
#include "../ESBMidService/ESBMidService_Service.h"

#ifndef ESBHUBSERVICELIB_API
#define ESBHUBSERVICELIB_API
#endif

namespace ESBHubService
{
	class IESBServiceInfo : public Utils::SafeCoding::IDisposable
	{

	};

	class IESBHubService : public ESBMidService::IESBService
	{
	public:
		enum HUBMESSAGE
		{

		};
	public:
		//virtual BOOL SetEvent(const std::function<int(HUBMESSAGE, WPARAM, LPARAM)> &func) = 0;
	};

	ESBHUBSERVICELIB_API IESBHubService* CreateESBHubService();
};

#endif //_INCLUDE_ESBMIDSERVICE_HUBSERVICE_H_