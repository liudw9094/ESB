#pragma once

#include <ESBHubService/ESBHubService.h>
#include "RegisteredService.h"

class CESBHubServiceImp : public ESBHubService::IESBHubService
{
private:
	SREF(ESBMidService::IESBService)								m_service;
	mutable SREF(Utils::Thread::IThread)							m_serviceThread;
	SREF(Utils::Thread::ITimer)										m_timerHeartBeatMonitor;
	std::map<std::wstring, std::vector<SREF(CRegisteredService)>>	m_mapServices_guid;
	std::map<std::wstring, SREF(CRegisteredService)>				m_mapServices_session;
public:
	CESBHubServiceImp();
	~CESBHubServiceImp();

	// Overriding ESBMidService::IESBService
	virtual BOOL Start(int nPort);
	virtual BOOL Stop(void);
	virtual BOOL IsStarted(void) const;
	virtual BOOL SetCallback_OnPreInvoke(const TOnPreInvokeFunc &func);
	virtual BOOL SetCallback_OnClientSessionConfirmed(const TOnClientSessionConfirmedFunc &func);
	virtual BOOL SetCallback_OnClientSessionEnd(const TOnClientSessionEndFunc &func);
	virtual BOOL SetCallback_OnRegisteredOnHub(const TOnRegisteredOnHubFunc &func);
	virtual BOOL SetCallback_OnUnregisteredFromHub(const TOnUnregisteredFromHubFunc &func);
	virtual BOOL SetCallback_OnHubSessionLost(const TOnHubSessionLostFunc &func);

	virtual BOOL SetCallback_OnClientInvoke(const TOnClientInvokeFunc &func);
	virtual BOOL SetCallback_OnAccept(const TOnAcceptFunc& func);
	virtual BOOL SetCallback_OnStarted(const TOnStartFunc& func);
	virtual BOOL SetCallback_OnStoped(const TOnStopFunc& func);

	virtual int	RegisterToHub(const std::wstring& wsHubURL,
							const std::wstring& wsServiceURL,
							const GUID guidService,
							const std::wstring& wsServiceName,
							UINT maximumSession);
	virtual int GetPort(void) const;
	virtual std::wstring GetClientIP(const struct soap* pSoap) const;
	virtual BOOL IsClientSessionExisted(const std::wstring& wsSession) const;
	virtual BOOL IsClientSessionValid(const std::wstring& wsSession) const;
	virtual BOOL RemoveClientSession(const std::wstring& wsSession);
	virtual BOOL CheckHubSession() const;
	virtual ESBMidService::IESBServiceHubConnection* GetHubConnection();
	// Overriding Utils::SafeCoding::IDisposable
	virtual void Dispose();
	
private:
	void _OnTimer_HeartBeatMonitor();

	int _PreProcessInvoke(SREF(Utils::Thread::IThread) pthread,
						struct soap* psoap,
						std::wstring& wsSession,
						std::wstring& wsInputs,
						BOOL &bNoFuthureProcess,
						std::wstring& wsResults,
						ESBCommon::ENUM_IDTYPE &idType);
	int _ProcessServiceRequest(SREF(Utils::Thread::IThread) pthread,
							struct soap* psoap,
							const std::wstring& wsSession,
							const std::wstring& wsInputs,
							BOOL &bNoFuthureProcess,
							std::wstring& wsResults);
	int _ProcessClientRequest(SREF(Utils::Thread::IThread) pthread,
							struct soap* psoap,
							const std::wstring& wsSession,
							const std::wstring& wsInputs,
							BOOL &bNoFuthureProcess,
							std::wstring& wsResults);


	int _On_ESBService_HubMethod(const std::wstring& session,
		const ESBCommon::ESBService_HubMethod_RegisterToHub& param,
		std::wstring& results);
	int _On_ESBService_HubMethod(const std::wstring& session,
		const ESBCommon::ESBService_HubMethod_Unregister& param,
		std::wstring& results);
	int _On_ESBService_HubMethod(const std::wstring& session,
		const ESBCommon::ESBService_HubMethod_UpdateLoadState& param,
		std::wstring& results);
	int _On_ESBService_HubMethod(const std::wstring& session,
		const ESBCommon::ESBHeartBeat& param,
		std::wstring& results);
	int _On_ESBService_HubMethod(const std::wstring& session,
		const ESBCommon::ESBService_HubMethod_ClientSessionEnd& param,
		std::wstring& results);

	// when client request a service.
	int _On_ESBService_HubMethod(const std::wstring& session,
		const ESBCommon::ESBService_HubMethod_StartSession& param,
		std::wstring& results);

	static void _Reorder(std::vector <SREF(CRegisteredService)> &refVec);
};

