#include "stdafx.h"
#include "ESBMidService.h"
#include "ESBMidServiceImp.h"

using namespace std;
using namespace ESBMidService;
using namespace ESBWebService;

CESBMidServiceImp::CESBMidServiceImp() :
	m_webService(CreateESBWebServiceServer())
{
}


CESBMidServiceImp::~CESBMidServiceImp()
{
}

int CESBMidServiceImp::Start(int nPort)
{
	return m_webService->Start(nPort);
}

int CESBMidServiceImp::End(void)
{
	if (IsStarted())
		return m_webService->End();
	else
		return -1;
}

BOOL CESBMidServiceImp::IsStarted(void) const
{
	return m_webService->IsStarted();
}

int	CESBMidServiceImp::RegisterToHub(const std::wstring& wsHubURL,
									const std::wstring& wsServiceURL,
									const GUID guidService,
									const std::wstring& wsServiceName,
									UINT maximumSession)
{
	return m_hubConnection.RegisterToHub(wsHubURL, wsServiceURL, guidService, wsServiceName, maximumSession);
}

int CESBMidServiceImp::GetPort(void) const
{
	return m_webService->GetPort();
}

IESBServiceHubConnection* CESBMidServiceImp::GetHubConnection()
{
	return &m_hubConnection;
}

void CESBMidServiceImp::Dispose()
{
	delete this;
}


int CESBMidServiceImp::_ProcessWebServiceInvoke(const std::wstring& wsSession, const std::wstring& wsInputs, std::wstring& wsResults);


ESBMIDSERVICE_API IESBService* ESBMidService::CreateESBService()
{
	return new CESBMidServiceImp;
}