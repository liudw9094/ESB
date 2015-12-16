#include "stdafx.h"
#include "ESBHubServiceLib.h"
#include "ESBHubService.h"

using namespace std;
using namespace ESBMidService;
using namespace Utils::Thread;
using namespace ESBHubService;
using namespace ESBCommon;
using namespace ESBDataSerialzer;
using namespace ESBXMLParser;

CESBHubService::CESBHubService() :
	m_service(CreateESBService()),
	m_serviceThread(CreateThread())
{
}


CESBHubService::~CESBHubService()
{
}

int CESBHubService::Start(int nPort)
{
	int nRet = 0;
	m_serviceThread->Invoke([this, nPort, &nRet]() {
		m_service->SetEvent_PreInvoke([this](SREF(Utils::Thread::IThread) pthread,
											struct soap* psoap,
											std::wstring& wsSession,
											std::wstring& wsInputs,
											BOOL &bNoFuthureProcess,
											std::wstring& wsResults,
											ESBCommon::ENUM_IDTYPE& idType) -> int {
			return _PreProcessInvoke(pthread, psoap, wsSession, wsInputs, bNoFuthureProcess, wsResults, idType);
		});
		nRet = m_service->Start(nPort);
	});
	return nRet;
}

int CESBHubService::End(void)
{
	int nRet = 0;
	m_serviceThread->Invoke([this, &nRet]() {
		nRet = m_service->End();
	});
	return nRet;
}

BOOL CESBHubService::IsStarted(void) const
{
	BOOL bRet = 0;
	m_serviceThread->Invoke([this, &bRet]() {
		bRet = m_service->IsStarted();
	});
	return bRet;
}

BOOL CESBHubService::SetEvent_PreInvoke(const TPreInvokeFunc &func)
{
	return FALSE;
}

BOOL CESBHubService::SetEvent_Invoke(const TInvokeFunc &func)
{
	return FALSE;
}

BOOL CESBHubService::SetEvent_Accept(const TAcceptFunc& func)
{
	return FALSE;
}

int	CESBHubService::RegisterToHub(const std::wstring& wsHubURL,
	const std::wstring& wsServiceURL,
	const GUID guidService,
	const std::wstring& wsServiceName,
	UINT maximumSession)
{
	return -1;
	/*
	int nRet = 0;
	m_serviceThread->Invoke([this, &wsHubURL, &wsServiceURL, &guidService, &wsServiceName, maximumSession, &nRet]() {
		nRet = m_service->RegisterToHub(wsHubURL, wsServiceURL, guidService, wsServiceName, maximumSession);
	});
	return nRet;
	*/
}

int CESBHubService::GetPort(void) const
{
	int nRet = 0;
	m_serviceThread->Invoke([this, &nRet]() {
		nRet = m_service->GetPort();
	});
	return nRet;
}

ESBMidService::IESBServiceHubConnection* CESBHubService::GetHubConnection()
{
	return NULL;
	/*
	IESBServiceHubConnection* pRet = 0;
	m_serviceThread->Invoke([this, &pRet]() {
		pRet = m_service->GetHubConnection();
	});
	return pRet;
	*/
}

BOOL CESBHubService::CheckClientSession(const std::wstring& wsSession)
{
	BOOL bRet = 0;
	m_serviceThread->Invoke([this, &bRet, &wsSession]() {
		bRet = m_service->CheckClientSession(wsSession);
	});
	return bRet;
}

BOOL CESBHubService::CheckHubSession(const std::wstring& wsSession)
{
	BOOL bRet = 0;
	m_serviceThread->Invoke([this, &bRet, &wsSession]() {
		bRet = m_service->CheckHubSession(wsSession);
	});
	return bRet;
}

void CESBHubService::Dispose()
{
	delete this;
}

int CESBHubService::_PreProcessInvoke(SREF(Utils::Thread::IThread) pthread,
										struct soap* psoap,
										std::wstring& wsSession,
										std::wstring& wsInputs,
										BOOL &bNoFuthureProcess,
										std::wstring& wsResults,
										ESBCommon::ENUM_IDTYPE &idType)
{
	idType = IDTYPE_ESBHub;
	ESBServiceRequest request;
	if (!String2Data(request, wsInputs))
		return -1;

	if (request.idType == IDTYPE_ESBService)
		return _ProcessServiceRequest(pthread, psoap, wsSession, request.contents, bNoFuthureProcess, wsResults);
	else if (request.idType == IDTYPE_ESBClient)
		return _ProcessClientRequest(pthread, psoap, wsSession, request.contents, bNoFuthureProcess, wsResults);
	else
		return 0;
}

int CESBHubService::_ProcessServiceRequest(SREF(Utils::Thread::IThread) pthread,
										struct soap* psoap,
										const std::wstring& wsSession,
										const std::wstring& wsInputs,
										BOOL &bNoFuthureProcess,
										std::wstring& wsResults)
{
	SREF(IXMLDoc) xmlDoc = CreateXMLDoc();
	SREF(IXMLNode) root = xmlDoc->LoadXML(wsInputs);
	if (!root->IsValid())
		return 0;
	wstring nodeName = root->GetNodeName();
	bNoFuthureProcess = TRUE;
	if (nodeName == ESBService_HubMethod_RegisterToHub::NAMES.ROOTNAME)
	{
		ESBService_HubMethod_RegisterToHub param;
		if (!String2Data(param, wsInputs))
			return -1;
		return _On_ESBService_HubMethod(wsSession, param, wsResults);
	}
	else if (nodeName == ESBService_HubMethod_Unregister::NAMES.ROOTNAME)
	{
		ESBService_HubMethod_Unregister param;
		if (!String2Data(param, wsInputs))
			return -1;
		return _On_ESBService_HubMethod(wsSession, param, wsResults);
	}
	else if (nodeName == ESBService_HubMethod_ModifySessionLimitation::NAMES.ROOTNAME)
	{
		ESBService_HubMethod_ModifySessionLimitation param;
		if (!String2Data(param, wsInputs))
			return -1;
		return _On_ESBService_HubMethod(wsSession, param, wsResults);
	}
	else if (nodeName == ESBService_HubMethod_IncreaseSessionLoad::NAMES.ROOTNAME)
	{
		ESBService_HubMethod_IncreaseSessionLoad param;
		if (!String2Data(param, wsInputs))
			return -1;
		return _On_ESBService_HubMethod(wsSession, param, wsResults);
	}
	else if (nodeName == ESBService_HubMethod_DecreaseSessionLoad::NAMES.ROOTNAME)
	{
		ESBService_HubMethod_DecreaseSessionLoad param;
		if (!String2Data(param, wsInputs))
			return -1;
		return _On_ESBService_HubMethod(wsSession, param, wsResults);
	}
	else if (nodeName == ESBService_HubMethod_ClientSessionEnd::NAMES.ROOTNAME)
	{
		ESBService_HubMethod_ClientSessionEnd param;
		if (!String2Data(param, wsInputs))
			return -1;
		return _On_ESBService_HubMethod(wsSession, param, wsResults);
	}
	else
	{
		bNoFuthureProcess = FALSE;
		return 0;
	}
}

int CESBHubService::_ProcessClientRequest(SREF(Utils::Thread::IThread) pthread,
										struct soap* psoap,
										const std::wstring& wsSession,
										const std::wstring& wsInputs,
										BOOL &bNoFuthureProcess,
										std::wstring& wsResults)
{
	ESBService_HubMethod_StartSession request;
	if (!String2Data(request, wsInputs))
		return 0;
	bNoFuthureProcess = TRUE;
	return _On_ESBService_HubMethod(wsSession, request, wsResults);
}

int CESBHubService::_On_ESBService_HubMethod(const std::wstring& session,
	const ESBCommon::ESBService_HubMethod_RegisterToHub& param,
	std::wstring& results)
{
	int nRet = 0;
	m_serviceThread->Invoke([this, &session, &param, &results, &nRet]() {
		wstring wsGuidService = GUID2String(param.guidService);
		auto found = m_mapServices_guid.find(wsGuidService);
		if (found == m_mapServices_guid.end())
			m_mapServices_guid[wsGuidService] = std::vector <SREF(CRegisteredService)>();
		std::vector <SREF(CRegisteredService)> &refVec = m_mapServices_guid[wsGuidService];
		BOOL bExists = FALSE;
		wstring wsSession;
		for (size_t i = 0; i < refVec.size(); ++i)
		{
			SREF(CRegisteredService) pService = refVec[i];
			if (pService->GetURL() == param.wsServiceURL)
			{
				wsSession = pService->GetSession();
				bExists = TRUE;
				break;
			}
		}
		if (!bExists)
		{
			wsSession = CreateGuid();
			SREF(CRegisteredService) newRegService = new CRegisteredService(wsSession, param);
			refVec.push_back(newRegService);
			m_mapServices_session[wsSession] = newRegService;
		}
		ESBServiceSessionReply sessionReply;
		sessionReply.wsServiceSession = wsSession;
		Data2String(results, sessionReply);
	});
	return nRet;
}

int CESBHubService::_On_ESBService_HubMethod(const std::wstring& session,
	const ESBCommon::ESBService_HubMethod_Unregister& param,
	std::wstring& results)
{
	int nRet = 0;
	m_serviceThread->Invoke([this, &session, &param, &results, &nRet]() {
		auto found = m_mapServices_session.find(session);
		if (found == m_mapServices_session.end())
		{
			nRet = -101;
			return;
		}

		// Remove registered service
		SREF(CRegisteredService) pService = found->second;
		m_mapServices_session.erase(found);
		GUID serviceGUID = pService->GetServiceGUID();
		wstring wsGuidService = GUID2String(serviceGUID);
		std::vector <SREF(CRegisteredService)> &refVec = m_mapServices_guid[wsGuidService];
		for (size_t i = 0; i < refVec.size(); ++i)
		{
			SREF(CRegisteredService) pService = refVec[i];
			if (pService->GetSession() == session)
			{
				refVec.erase(refVec.begin() + i);
				break;
			}
		}
		if (refVec.empty())
			m_mapServices_guid.erase(wsGuidService);

		ESBService_ReplyOK replyOK;
		Data2String(results, replyOK);
	});
	return nRet;
}

int CESBHubService::_On_ESBService_HubMethod(const std::wstring& session,
	const ESBCommon::ESBService_HubMethod_ModifySessionLimitation& param,
	std::wstring& results)
{
	// TODO: add implementation
	return -1;
}

int CESBHubService::_On_ESBService_HubMethod(const std::wstring& session,
	const ESBCommon::ESBService_HubMethod_IncreaseSessionLoad& param,
	std::wstring& results)
{
	// TODO: add implementation
	return -1;
}

int CESBHubService::_On_ESBService_HubMethod(const std::wstring& session,
	const ESBCommon::ESBService_HubMethod_DecreaseSessionLoad& param,
	std::wstring& results)
{
	// TODO: add implementation
	return -1;
}

int CESBHubService::_On_ESBService_HubMethod(const std::wstring& session,
	const ESBCommon::ESBService_HubMethod_ClientSessionEnd& param,
	std::wstring& results)
{
	// TODO: add implementation
	return -1;
}

int CESBHubService::_On_ESBService_HubMethod(const std::wstring& session,
	const ESBCommon::ESBService_HubMethod_StartSession& param,
	std::wstring& results)
{
	int nRet = 0;
	m_serviceThread->Invoke([this, &session, &param, &results, &nRet]() {
		wstring wsGuidService = GUID2String(param.guidService);
		auto found = m_mapServices_guid.find(wsGuidService);
		if (found == m_mapServices_guid.end())
		{
			nRet = -101;
			return;
		}
		std::vector <SREF(CRegisteredService)> &refVec = m_mapServices_guid[wsGuidService];
		if (refVec.empty())
		{
			m_mapServices_guid.erase(wsGuidService);
			nRet = -101;
			return;
		}
		size_t nIndex = refVec.size() - 1;
		SREF(CRegisteredService) pService = refVec[nIndex];
		ESBClientToken newToken;
		if (!pService->NewToken(newToken))
		{
			nRet = -101;
			return;
		}

		// Reoreder
		if (nIndex > 0)
		{
			SREF(CRegisteredService) pServicePrev = refVec[nIndex - 1];
			if (pServicePrev->GetCurrentCapacityUsageRate() < pService->GetCurrentCapacityUsageRate())
				swap(refVec[nIndex], refVec[nIndex - 1]);
		}
		// reply token
		Data2String(results, newToken);
	});
	return nRet;
}