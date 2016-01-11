#pragma once

#include <ESBMidService/ESBMidService_Service.h>
#include "ESBServiceHubConnectionImp.h"

class CESBMidServiceImp : public ESBMidService::IESBService
{
private:
	struct CLIENTINFO
	{
		ESBCommon::ESBServiceToken token;
	};
	SREF(ESBWebService::IESBWebServiceServer) m_webService;
	CESBServiceHubConnectionImp m_hubConnection;
	TPreInvokeFunc m_funcPreInvoke;
	TInvokeFunc m_funcInvoke;
	SREF(Utils::Thread::ICriticalSection) m_plkMapUsers;
	std::map<std::wstring, CLIENTINFO>	m_mapUsers;
	UINT								m_uMaxSessionNum;
public:
	CESBMidServiceImp();
	~CESBMidServiceImp();

	virtual int Start(int nPort);
	virtual int Stop(void);
	virtual BOOL IsStarted(void) const;
	virtual BOOL SetCallback_PreInvoke(const TPreInvokeFunc &func);
	virtual BOOL SetCallback_Invoke(const TInvokeFunc &func);
	virtual BOOL SetCallback_Accept(const TAcceptFunc& func);
	virtual int	RegisterToHub(const std::wstring& wsHubURL,
							const std::wstring& wsServiceURL,
							const GUID guidService,
							const std::wstring& wsServiceName,
							UINT maximumSessionn);
	virtual int GetPort(void) const;
	virtual std::wstring&& GetClientIP(const struct soap* pSoap) const;
	virtual ESBMidService::IESBServiceHubConnection* GetHubConnection();
	virtual BOOL CheckClientSession(const std::wstring& wsSession);
	virtual BOOL CheckHubSession(const std::wstring& wsSession);
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

