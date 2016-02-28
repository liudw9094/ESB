#pragma once

#include <ESBMidService/ESBMidService_HubConnection.h>
#include <ESBMidService/ESBMidService_Service.h>
#include <Common/ESBHub.h>

class CESBMidServiceImp;

class CESBServiceHubConnectionImp : public ESBMidService::IESBServiceHubConnection
{
private:
	CESBMidServiceImp						*m_pESBMidServiceImp;
	SREF(ESBWebService::IESBWebServiceClient) m_webClient;
	mutable SREF(Utils::Thread::IThread)	m_threadClient;
	BOOL									m_bValid;
	ESBCommon::ESBServiceSessionReply		m_wsHubSession;
	SREF(Utils::Thread::ITimer)				m_timerHeartBeat;
	UINT									m_uMaximumSessionNum, m_currentSessionNum;


	ESBMidService::IESBService::TOnRegisteredOnHubFunc		m_funcOnRegisteredOnHub;
	ESBMidService::IESBService::TOnUnregisteredFromHubFunc	m_funcOnUnregisteredFromHub;
	ESBMidService::IESBService::TOnHubSessionLostFunc		m_funcOnHubSessionLost;
public:
	explicit CESBServiceHubConnectionImp(CESBMidServiceImp* pESBMidServiceImp);
	~CESBServiceHubConnectionImp();

	virtual int	RegisterToHub(const std::wstring& wsHubURL,
		const std::wstring& wsServiceURL,
		const GUID guidService,
		const std::wstring& wsServiceName,
		UINT maximumSessionNum,
		UINT currentSessionNum);
	virtual int Unregister();
	virtual BOOL IsValid() const;
	virtual UINT GetMaximumSessionNum() const;
	virtual UINT GetCurrentSessionNum() const;

	virtual int ModifySessionLimitation(UINT maximumSessionNum);
	virtual int UpdateLoadState(UINT maximumSessionNum, UINT currentSessionNum);

private:
	void _InitializeClientThread();
	void _UninitializeClientThread();
	void _OnHeartBeatTimer();
	void _ClearState();
public:
	void SetCallback_OnRegisteredOnHub(const ESBMidService::IESBService::TOnRegisteredOnHubFunc &func);
	void SetCallback_OnUnregisteredFromHub(const ESBMidService::IESBService::TOnUnregisteredFromHubFunc &func);
	void SetCallback_OnHubSessionLost(const ESBMidService::IESBService::TOnHubSessionLostFunc &func);
};

