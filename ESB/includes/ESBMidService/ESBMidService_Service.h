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
	enum class EMSessionEndReason
	{
		CLIENT_REQUEST = 0,
		SESSION_EXPIRED,
		SERVER_MANIPULATE,

		REASON_USER_DEFINED = 0x800,
	};

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


		typedef std::function <void(IESBService* sender, const std::wstring& session)> TOnClientSessionConfirmedFunc;
		typedef std::function <void(IESBService* sender, const std::wstring& session, EMSessionEndReason reason)> TOnClientSessionEndFunc;
		typedef std::function <void(IESBService* sender)> TOnRegisteredOnHubFunc;
		typedef std::function <void(IESBService* sender)> TOnUnregisteredFromHubFunc;
		typedef std::function <void(IESBService* sender)> TOnHubSessionLostFunc;

		virtual BOOL SetCallback_OnPreInvoke(const TOnPreInvokeFunc &func) = 0;
		virtual BOOL SetCallback_OnClientSessionConfirmed(const TOnClientSessionConfirmedFunc &func) = 0;
		virtual BOOL SetCallback_OnClientSessionEnd(const TOnClientSessionEndFunc &func) = 0;
		virtual BOOL SetCallback_OnRegisteredOnHub(const TOnRegisteredOnHubFunc &func) = 0;
		virtual BOOL SetCallback_OnUnregisteredFromHub(const TOnUnregisteredFromHubFunc &func) = 0;
		virtual BOOL SetCallback_OnHubSessionLost(const TOnHubSessionLostFunc &func) = 0;

		virtual int	RegisterToHub(const std::wstring& wsHubURL,
								const std::wstring& wsServiceURL,
								const GUID guidService,
								const std::wstring& wsServiceName,
								UINT maximumSession) = 0;
		virtual IESBServiceHubConnection* GetHubConnection() = 0;
		virtual BOOL IsClientSessionExisted(const std::wstring& wsSession) const = 0;
		virtual BOOL IsClientSessionValid(const std::wstring& wsSession) const = 0;
		virtual BOOL RemoveClientSession(const std::wstring& wsSession, EMSessionEndReason reason = EMSessionEndReason::SERVER_MANIPULATE) = 0;
		virtual BOOL CheckHubSession() const = 0;
		virtual UINT GetMaximumSessionNum() const = 0;
		virtual UINT GetCurrentSessionNum() const = 0;
	};

	ESBMIDSERVICE_API IESBService* CreateESBService(UINT uSessionTimeoutSecs = 600);
};

#endif //_INCLUDE_ESBMIDSERVICE_SERVICE_H_