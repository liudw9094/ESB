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
		std::chrono::steady_clock::time_point tpSession;
		CLIENTINFO()
			: bConfirmed(FALSE),
			tpSession(std::chrono::steady_clock::now())
		{
		}
	};
	UINT											m_uSessionTimeoutSecs;
	mutable SREF(Utils::Thread::IThread)			m_threadSessionMgr;
	SREF(Utils::Thread::ITimer)						m_timerSessionMgr;

	SREF(ESBWebService::IESBWebServiceServer)		m_webService;
	CESBServiceHubConnectionImp						m_hubConnection;
	TOnPreInvokeFunc								m_funcOnPreInvoke;
	TOnClientSessionConfirmedFunc					m_funcOnClientSessionConfirmed;
	TOnClientSessionEndFunc							m_funcOnClientSessionEnd;
	TOnRegisteredOnHubFunc							m_funcOnRegisteredOnHub;
	TOnClientInvokeFunc								m_funcOnInvoke;
	mutable SREF(Utils::Thread::ICriticalSection)	m_plkMapUsers;
	std::map<std::wstring, CLIENTINFO>				m_mapUsers;
	volatile UINT									m_uMaxSessionNum;
	bool											m_bAuthentication;
	ESBWebService::SAuthentication					m_authentication;
	ESBWebService::SAuthentication					m_authenticationToHub;
public:
	explicit CESBMidServiceImp(UINT uSessionTimeoutSecs);
	~CESBMidServiceImp();

	virtual BOOL Start(int nPort, const ESBWebService::SAuthentication *pAuthentication = NULL);
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
							UINT maximumSessionn);
	virtual int GetPort(void) const;
	virtual std::wstring GetClientIP(const struct soap* pSoap) const;
	virtual ESBMidService::IESBServiceHubConnection* GetHubConnection();
	virtual BOOL IsClientSessionExisted(const std::wstring& wsSession) const;
	virtual BOOL IsClientSessionValid(const std::wstring& wsSession) const;
	virtual BOOL RemoveClientSession(const std::wstring& wsSession, ESBMidService::EMSessionEndReason reason = ESBMidService::EMSessionEndReason::SERVER_MANIPULATE);
	virtual BOOL CheckHubSession() const;
	virtual UINT GetMaximumSessionNum() const;
	virtual UINT GetCurrentSessionNum() const;
	virtual void Dispose();
private:
	void _UpdateClientSessionTimePoint(const std::wstring& wsSession);
	void _OnTimer_SessionMgr(Utils::Thread::ITimer*);

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

