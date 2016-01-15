#include "stdafx.h"
#include "ESBMidService.h"
#include "ESBMidServiceImp.h"

using namespace std;
using namespace ESBMidService;
using namespace ESBWebService;
using namespace ESBDataSerialzer;
using namespace ESBCommon;
using namespace Utils::SafeCoding;
using namespace ESBXMLParser;

CESBMidServiceImp::CESBMidServiceImp() :
	m_webService(CreateESBWebServiceServer()),
	m_uMaxSessionNum(0)
{
}


CESBMidServiceImp::~CESBMidServiceImp()
{
}

int CESBMidServiceImp::Start(int nPort)
{
	if (IsStarted())
		return -1;
	using namespace std::placeholders;
	auto func = std::bind(&CESBMidServiceImp::_ProcessWebServiceInvoke, this, _1, _2, _3, _4, _5);
	if (!m_webService->SetCallback_Invoke(func))
		return -2;
	return m_webService->Start(nPort);
}

int CESBMidServiceImp::Stop(void)
{
	if (IsStarted())
	{
		int nRet = m_webService->Stop();
		m_webService->SetCallback_Invoke([](
			SREF(Utils::Thread::IThread) pthread,
			struct soap* psoap,
			const std::wstring& wsSession,
			const std::wstring& wsInputs,
			std::wstring& wsResults) -> int
		{
			return 0;
		});
		return nRet;
	}
	else
		return -1;
}

BOOL CESBMidServiceImp::IsStarted(void) const
{
	return m_webService->IsStarted();
}

BOOL CESBMidServiceImp::SetCallback_PreInvoke(const IESBService::TPreInvokeFunc &func)
{
	if (IsStarted())
		return FALSE;
	m_funcPreInvoke = func;
	return TRUE;
}

BOOL CESBMidServiceImp::SetCallback_Invoke(const IESBService::TInvokeFunc &func)
{
	if (IsStarted())
		return FALSE;
	m_funcInvoke = func;
	return TRUE;
}

BOOL CESBMidServiceImp::SetCallback_Accept(const IESBService::TAcceptFunc& func)
{
	return m_webService->SetCallback_Accept(func);
}

int	CESBMidServiceImp::RegisterToHub(const std::wstring& wsHubURL,
									const std::wstring& wsServiceURL,
									const GUID guidService,
									const std::wstring& wsServiceName,
									UINT maximumSession)
{
	int nRet = 0;
	SLOCK(m_plkMapUsers);
	if(nRet = m_hubConnection.RegisterToHub(wsHubURL, wsServiceURL, guidService, wsServiceName, maximumSession, m_mapUsers.size()))
		m_uMaxSessionNum = maximumSession;
	return nRet;
}

int CESBMidServiceImp::GetPort(void) const
{
	return m_webService->GetPort();
}

std::wstring&& CESBMidServiceImp::GetClientIP(const struct soap* pSoap) const
{
	return m_webService->GetClientIP(pSoap);
}

IESBServiceHubConnection* CESBMidServiceImp::GetHubConnection()
{
	return &m_hubConnection;
}

BOOL CESBMidServiceImp::CheckClientSession(const std::wstring& wsSession)
{
	SLOCK(m_plkMapUsers);
	return m_mapUsers.find(wsSession) != m_mapUsers.end();
}

BOOL CESBMidServiceImp::CheckHubSession(const std::wstring& wsSession)
{
	return m_hubConnection.IsHubSessionValid(wsSession);
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
	wstring _wsSession = wsSession;
	wstring _wsInputs = wsInputs;
	wstring _wsResults;
	ENUM_IDTYPE idType = IDTYPE_ESBService;
	CFinalize final1([&wsResults, &_wsResults, &idType]() {
		ESBServiceReply rp;
		rp.idType = idType;
		rp.contents = _wsResults;
		Data2String(wsResults, rp);
	});

	BOOL bNoFurtherProcess = FALSE;
	if(m_funcPreInvoke)
	{
		int nRet = m_funcPreInvoke(pthread, psoap, _wsSession, _wsInputs, bNoFurtherProcess, _wsResults, idType);
		if (nRet < 0 || bNoFurtherProcess)
		{
			return nRet;
		}
	}
	
	ESBServiceRequest request;
	if (!String2Data(request, _wsInputs))
		return -1;
	_wsSession = request.contents;

	if (request.idType == IDTYPE_ESBHub)
		return _ProcessHubRequest(pthread, psoap, _wsSession, _wsInputs, _wsResults);
	else if (request.idType == IDTYPE_ESBClient)
	{
		ESBService_ServiceMethod_ClientRequest userRequest;
		if (!String2Data(userRequest, _wsInputs))
			return _ProcessClientRequest(pthread, psoap, _wsSession, _wsInputs, _wsResults);
		else
		{
			if (!CheckClientSession(_wsSession))
				return -102;
			_wsResults = L"";
			if (m_funcInvoke)
				return m_funcInvoke(pthread, psoap, _wsSession, _wsInputs, _wsResults);
			else
				return 0;
		}
	}
	else
		return -101;

	
}

int CESBMidServiceImp::_ProcessHubRequest(SREF(Utils::Thread::IThread) pthread,
										struct soap* psoap, 
										const std::wstring& wsSession,
										const std::wstring& wsInputs,
										std::wstring& wsResults)
{
	SREF(IXMLDoc) xmlDoc = CreateXMLDoc();
	SREF(IXMLNode) root = xmlDoc->LoadXML(wsInputs);
	if (!root->IsValid())
		return 0;
	wstring nodeName = root->GetNodeName();
	if (nodeName == ESBServiceToken::NAMES.ROOTNAME)
	{
		ESBServiceToken param;
		if (!String2Data(param, wsInputs))
			return -1;
		return _On_ESBService_ServiceMethod(wsSession, param, wsResults);
	}
	return -101;
}

int CESBMidServiceImp::_ProcessClientRequest(SREF(Utils::Thread::IThread) pthread,
											struct soap* psoap,
											const std::wstring& wsSession,
											const std::wstring& wsInputs,
											std::wstring& wsResults)
{

	return -101;
}

int  CESBMidServiceImp::_On_ESBService_ServiceMethod(const std::wstring& session,
	const ESBCommon::ESBServiceToken& param,
	std::wstring& results)
{
	SLOCK(m_plkMapUsers);
	if (m_mapUsers.size() >= m_uMaxSessionNum)
		return -101;

	ESBService_ServiceReply_LoadStateUpdate reply;
	CLIENTINFO clientInfo;
	clientInfo.token = param;
	m_mapUsers[clientInfo.token.wsClientSession] = clientInfo;
	reply.currentSessionNum = m_mapUsers.size();
	reply.maximumSession = m_uMaxSessionNum;

	Data2String(results, reply);
	return 0;
}

ESBMIDSERVICE_API IESBService* ESBMidService::CreateESBService()
{
	return new CESBMidServiceImp;
}