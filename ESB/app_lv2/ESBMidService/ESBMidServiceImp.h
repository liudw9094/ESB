#pragma once

#include <ESBMidService/ESBMidService_Service.h>
#include "ESBServiceHubConnectionImp.h"

class CESBMidServiceImp : public ESBMidService::IESBService
{
private:
	SREF(ESBWebService::IESBWebServiceServer) m_webService;
	CESBServiceHubConnectionImp m_hubConnection;
	std::function<int(const std::wstring& wsSession, const std::wstring& wsInputs, std::wstring& wsResults)>
		m_funcInvoke;
public:
	CESBMidServiceImp();
	~CESBMidServiceImp();

	virtual int Start(int nPort);
	virtual int End(void);
	virtual BOOL IsStarted(void) const;
	virtual int	RegisterToHub(const std::wstring& wsHubURL,
							const std::wstring& wsServiceURL,
							const GUID guidService,
							const std::wstring& wsServiceName,
							UINT maximumSessionn);
	virtual int GetPort(void) const;
	virtual ESBMidService::IESBServiceHubConnection* GetHubConnection();
	virtual void Dispose();
private:
	int _ProcessWebServiceInvoke(const std::wstring& wsSession, const std::wstring& wsInputs, std::wstring& wsResults);
};

