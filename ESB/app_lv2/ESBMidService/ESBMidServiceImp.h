#pragma once

#include <ESBMidService/ESBMidService_Service.h>
#include "ESBServiceHubConnectionImp.h"

class CESBMidServiceImp : public ESBMidService::IESBService
{
private:
	struct CLIENTINFO
	{
		ESBCommon::ESBServiceToken token;
		BOOL bConfirmed;
		CLIENTINFO()
			: bConfirmed(FALSE)
		{
		}
	};
	SREF(ESBWebService::IESBWebServiceServer)		m_webService;
	CESBServiceHubConnectionImp						m_hubConnection;
	TOnPreInvokeFunc								m_funcOnPreInvoke;
	TOnNewClientSessionFunc							m_funcOnNewClientSession;
	TOnClientSessionEndFunc							m_funcOnClientSessionEnd;
	TOnRegisteredOnHubFunc							m_funcOnRegisteredOnHub;
	TOnUnregisteredFromHubFunc						m_funcOnUnregisteredFromHub;
	TOnHubSessionLostFunc							m_funcOnHubSessionLost;
	TOnClientInvokeFunc								m_funcOnInvoke;
	mutable SREF(Utils::Thread::ICriticalSection)	m_plkMapUsers;
	std::map<std::wstring, CLIENTINFO>				m_mapUsers;
	volatile UINT									m_uMaxSessionNum;

public:
	CESBMidServiceImp();
	~CESBMidServiceImp();

	virtual BOOL Start(int nPort);
	virtual BOOL Stop(void);
	virtual BOOL IsStarted(void) const;

	virtual BOOL SetCallback_OnPreInvoke(const TOnPreInvokeFunc &func);
	virtual BOOL SetCallback_OnNewClientSession(const TOnNewClientSessionFunc &func);
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
							UINT maximumSessionn);
	virtual int GetPort(void) const;
	virtual std::wstring GetClientIP(const struct soap* pSoap) const;
	virtual ESBMidService::IESBServiceHubConnection* GetHubConnection();
	virtual BOOL IsClientSessionExisted(const std::wstring& wsSession) const;
	virtual BOOL IsClientSessionValid(const std::wstring& wsSession) const;
	virtual BOOL CheckHubSession() const;
	virtual void Dispose();
private:
	int _ProcessWebServiceInvoke(SREF(Utils::Thread::IThread) pthread,
								struct soap* psoap,
								const std::wstring& wsSession,
								const std::wstring& wsInputs,
								std::wstring& wsResults);
	int _ProcessHubRequest(SREF(Utils::Thread::IThread) pthread,
							struct soap* psoap,
							const std::wstring& wsSession,
							const std::wstring& wsInputs,
							std::wstring& wsResults);
	int _ProcessClientRequest(SREF(Utils::Thread::IThread) pthread,
								struct soap* psoap,
								const std::wstring& wsSession,
								const std::wstring& wsInputs,
								std::wstring& wsResults);

	int _On_ESBService_ServiceMethod(const std::wstring& session,
		const ESBCommon::ESBServiceToken& param,
		std::wstring& results);
};

