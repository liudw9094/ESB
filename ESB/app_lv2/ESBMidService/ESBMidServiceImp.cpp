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
	if (IsStarted())
		return -1;
	auto func = [this](SREF(Utils::Thread::IThread) pthread,
					struct soap* psoap,
					const std::wstring& wsSession,
					const std::wstring& wsInputs,
					std::wstring& wsResults) -> int
	{
		return _ProcessWebServiceInvoke(pthread, psoap, wsSession, wsInputs, wsResults);
	};
	if (!m_webService->SetEvent_Invoke(func))
		return -2;
	return m_webService->Start(nPort);
}

int CESBMidServiceImp::End(void)
{
	if (IsStarted())
	{
		int nRet = m_webService->End();
		m_webService->SetEvent_Invoke(std::function<
										int(SREF(Utils::Thread::IThread) pthread,
										struct soap* psoap,
										const std::wstring& wsSession,
										const std::wstring& wsInputs,
										std::wstring& wsResults)>()
									);
		return nRet;
	}
	else
		return -1;
}

BOOL CESBMidServiceImp::IsStarted(void) const
{
	return m_webService->IsStarted();
}

BOOL CESBMidServiceImp::SetInvokeFunction(const std::function<int(const std::wstring& wsSession, const std::wstring& wsInputs, std::wstring& wsResults)> &func)
{
	if (IsStarted())
		return FALSE;
	m_funcInvoke = func;
	return TRUE;
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


int CESBMidServiceImp::_ProcessWebServiceInvoke(SREF(Utils::Thread::IThread) pthread,
												struct soap* psoap,
												const std::wstring& wsSession,
												const std::wstring& wsInputs,
												std::wstring& wsResults)
{
	// TODO: add implementation here.
	ASSERT(FALSE);
	return 0;
}


ESBMIDSERVICE_API IESBService* ESBMidService::CreateESBService()
{
	return new CESBMidServiceImp;
}