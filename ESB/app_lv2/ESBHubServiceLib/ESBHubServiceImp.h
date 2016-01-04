#pragma once

#include <ESBHubService/ESBHubService.h>
#include "RegisteredService.h"

class CESBHubServiceImp : public ESBHubService::IESBHubService
{
private:
	SREF(ESBMidService::IESBService)								m_service;
	mutable SREF(Utils::Thread::IThread)							m_serviceThread;
	std::map<std::wstring, std::vector<SREF(CRegisteredService)>>	m_mapServices_guid;
	std::map<std::wstring, SREF(CRegisteredService)>				m_mapServices_session;
public:
	CESBHubServiceImp();
	~CESBHubServiceImp();

	// Overriding ESBMidService::IESBService
	virtual int Start(int nPort);
	virtual int End(void);
	virtual BOOL IsStarted(void) const;
	virtual BOOL SetEvent_PreInvoke(const TPreInvokeFunc &func);
	virtual BOOL SetEvent_Invoke(const TInvokeFunc &func);
	virtual BOOL SetEvent_Accept(const TAcceptFunc& func);
	virtual int	RegisterToHub(const std::wstring& wsHubURL,
							const std::wstring& wsServiceURL,
							const GUID guidService,
							const std::wstring& wsServiceName,
							UINT maximumSession);
	virtual int GetPort(void) const;
	virtual BOOL CheckClientSession(const std::wstring& wsSession);
	virtual BOOL CheckHubSession(const std::wstring& wsSession);
	virtual ESBMidService::IESBServiceHubConnection* GetHubConnection();
	// Overriding Utils::SafeCoding::IDisposable
	virtual void Dispose();
	
private:
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
		const ESBCommon::ESBService_HubMethod_ModifySessionLimitation& param,
		std::wstring& results);
	int _On_ESBService_HubMethod(const std::wstring& session,
		const ESBCommon::ESBService_HubMethod_IncreaseSessionLoad& param,
		std::wstring& results);
	int _On_ESBService_HubMethod(const std::wstring& session,
		const ESBCommon::ESBService_HubMethod_DecreaseSessionLoad& param,
		std::wstring& results);
	int _On_ESBService_HubMethod(const std::wstring& session,
		const ESBCommon::ESBService_HubMethod_ClientSessionEnd& param,
		std::wstring& results);

	// when client request a service.
	int _On_ESBService_HubMethod(const std::wstring& session,
		const ESBCommon::ESBService_HubMethod_StartSession& param,
		std::wstring& results);
};

