#pragma once

#include <ESBMidService/ESBMidService_HubConnection.h>
#include <Common/ESBHub.h>

class CESBServiceHubConnectionImp;

class CESBServiceHubConnectionImp : public ESBMidService::IESBServiceHubConnection
{
private:
	SREF(ESBWebService::IESBWebServiceClient) m_webClient;
	BOOL m_bValid;
	ESBCommon::ESBServiceHubSession m_wsHubSession;
public:
	CESBServiceHubConnectionImp();
	~CESBServiceHubConnectionImp();

	int	RegisterToHub(const std::wstring& wsHubURL,
					const std::wstring& wsServiceURL,
					const GUID guidService,
					const std::wstring& wsServiceName,
					UINT maximumSession);

	virtual BOOL IsValid();
	virtual int ModifySessionLimitation(int nLimitation);
	virtual int IncreaseSessionLoad();
	virtual int DecreaseSessionLoad();
	virtual int Unregister();
};

