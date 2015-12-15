#pragma once

#include <ESBMidService/ESBMidService_HubConnection.h>
#include <Common/ESBHub.h>

class CESBServiceHubConnectionImp;

class CESBServiceHubConnectionImp : public ESBMidService::IESBServiceHubConnection
{
private:
	SREF(ESBWebService::IESBWebServiceClient) m_webClient;
	mutable SREF(Utils::Thread::IThread)	m_threadClient;
	BOOL m_bValid;
	ESBCommon::ESBServiceHubSessionReply m_wsHubSession;
public:
	CESBServiceHubConnectionImp();
	~CESBServiceHubConnectionImp();



	virtual int	RegisterToHub(const std::wstring& wsHubURL,
		const std::wstring& wsServiceURL,
		const GUID guidService,
		const std::wstring& wsServiceName,
		UINT maximumSession);
	virtual int Unregister();
	virtual BOOL IsValid() const;
	virtual int ModifySessionLimitation(int nLimitation);
	virtual int IncreaseSessionLoad();
	virtual int DecreaseSessionLoad();

public:
};

