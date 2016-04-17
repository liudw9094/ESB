#include "stdafx.h"
#include "ESBHubServiceLib.h"
#include "ESBHubServiceImp.h"

using namespace std;
using namespace std::placeholders;
using namespace ESBMidService;
using namespace Utils::Thread;
using namespace ESBHubService;
using namespace ESBCommon;
using namespace ESBDataSerialzer;
using namespace ESBXMLParser;

CESBHubServiceImp::CESBHubServiceImp() :
	m_service(CreateESBService())
{
	m_serviceThread =
	CreateThread(
		[this](IThread*) {
			::CoInitialize(NULL);
			m_timerHeartBeatMonitor = ::CreateTimer(SERVICE_SESSION_HEARTBEAT_TIME_INTERVAL * 1000, false);
			m_timerHeartBeatMonitor->AddFunc(bind(&CESBHubServiceImp::_OnTimer_HeartBeatMonitor, this, _1));
		},
		[this](IThread*) {
			m_timerHeartBeatMonitor = NULL;
			::CoUninitialize();
		});
}


CESBHubServiceImp::~CESBHubServiceImp()
{
	m_serviceThread = NULL;
}

BOOL CESBHubServiceImp::Start(int nPort, const ESBWebService::SAuthentication *pAuthentication/* = NULL*/)
{
	BOOL nRet = FALSE;
	m_serviceThread->Invoke([this, nPort, pAuthentication, &nRet]() {
		using namespace std::placeholders;
		auto func = std::bind(&CESBHubServiceImp::_PreProcessInvoke, this, _1, _2, _3, _4, _5, _6, _7);
		m_service->SetCallback_OnPreInvoke(func);
		nRet = m_service->Start(nPort, pAuthentication);
		if (nRet)
			m_timerHeartBeatMonitor->Enable(true);
	});
	return nRet;
}

BOOL CESBHubServiceImp::Stop(void)
{
	BOOL nRet = 0;
	m_serviceThread->Invoke([this, &nRet]() {
		m_timerHeartBeatMonitor->Enable(false);
		nRet = m_service->Stop();
	});
	return nRet;
}

BOOL CESBHubServiceImp::IsStarted(void) const
{
	BOOL bRet = FALSE;
	m_serviceThread->Invoke([this, &bRet]() {
		bRet = m_service->IsStarted();
	});
	return bRet;
}

BOOL CESBHubServiceImp::SetCallback_OnPreInvoke(const TOnPreInvokeFunc &func)
{
	throw logic_error("Invalid method.");
	return FALSE;
}

BOOL CESBHubServiceImp::SetCallback_OnClientSessionConfirmed(const TOnClientSessionConfirmedFunc &func)
{
	throw logic_error("Invalid method.");
	return FALSE;
}

BOOL CESBHubServiceImp::SetCallback_OnClientSessionEnd(const TOnClientSessionEndFunc &func)
{
	throw logic_error("Invalid method.");
	return FALSE;
}

BOOL CESBHubServiceImp::SetCallback_OnRegisteredOnHub(const TOnRegisteredOnHubFunc &func)
{
	throw logic_error("Invalid method.");
	return FALSE;
}

BOOL CESBHubServiceImp::SetCallback_OnUnregisteredFromHub(const TOnUnregisteredFromHubFunc &func)
{
	throw logic_error("Invalid method.");
	return FALSE;
}

BOOL CESBHubServiceImp::SetCallback_OnHubSessionLost(const TOnHubSessionLostFunc &func)
{
	throw logic_error("Invalid method.");
	return FALSE;
}

BOOL CESBHubServiceImp::SetCallback_OnClientInvoke(const TOnClientInvokeFunc &func)
{
	throw logic_error("Invalid method.");
	return FALSE;
}

BOOL CESBHubServiceImp::SetCallback_OnAccept(const TOnAcceptFunc& func)
{
	throw logic_error("Invalid method.");
	return FALSE;
}

BOOL CESBHubServiceImp::SetCallback_OnStarted(const TOnStartFunc& func)
{
	BOOL bRet = FALSE;
	m_serviceThread->Invoke([this, &bRet, &func]() {
		bRet = m_service->SetCallback_OnStarted(func);
	});
	return bRet;
}

BOOL CESBHubServiceImp::SetCallback_OnStoped(const TOnStopFunc& func)
{
	BOOL bRet = FALSE;
	m_serviceThread->Invoke([this, &bRet, &func]() {
		bRet = m_service->SetCallback_OnStoped(func);
	});
	return bRet;
}

int	CESBHubServiceImp::RegisterToHub(const std::wstring& wsHubURL,
	const std::wstring& wsServiceURL,
	const GUID guidService,
	const std::wstring& wsServiceName,
	UINT maximumSession)
{
	throw logic_error("Invalid method.");
	return -1;
	/*
	int nRet = 0;
	m_serviceThread->Invoke([this, &wsHubURL, &wsServiceURL, &guidService, &wsServiceName, maximumSession, &nRet]() {
		nRet = m_service->RegisterToHub(wsHubURL, wsServiceURL, guidService, wsServiceName, maximumSession);
	});
	return nRet;
	*/
}

int CESBHubServiceImp::GetPort(void) const
{
	int nRet = 0;
	m_serviceThread->Invoke([this, &nRet]() {
		nRet = m_service->GetPort();
	});
	return nRet;
}

std::wstring CESBHubServiceImp::GetClientIP(const struct soap* pSoap) const
{
	wstring szRet = 0;
	m_serviceThread->Invoke([this, &szRet, pSoap]() {
		szRet = m_service->GetClientIP(pSoap);
	});
	return szRet;
}

ESBMidService::IESBServiceHubConnection* CESBHubServiceImp::GetHubConnection()
{
	throw logic_error("Invalid method.");
	return NULL;
	/*
	IESBServiceHubConnection* pRet = 0;
	m_serviceThread->Invoke([this, &pRet]() {
		pRet = m_service->GetHubConnection();
	});
	return pRet;
	*/
}

BOOL CESBHubServiceImp::IsClientSessionExisted(const std::wstring& wsSession) const
{
	BOOL bRet = 0;
	m_serviceThread->Invoke([this, &bRet, &wsSession]() {
		bRet = m_service->IsClientSessionExisted(wsSession);
	});
	return bRet;
}

BOOL CESBHubServiceImp::IsClientSessionValid(const std::wstring& wsSession) const
{
	BOOL bRet = 0;
	m_serviceThread->Invoke([this, &bRet, &wsSession]() {
		bRet = m_service->IsClientSessionValid(wsSession);
	});
	return bRet;
}

BOOL CESBHubServiceImp::RemoveClientSession(const std::wstring& wsSession, EMSessionEndReason reason/* = EMSessionEndReason::SERVER_MANIPULATE*/)
{
	BOOL bRet = 0;
	m_serviceThread->Invoke([this, &bRet, &wsSession, reason]() {
		bRet = m_service->RemoveClientSession(wsSession, reason);
	});
	return bRet;
}

BOOL CESBHubServiceImp::CheckHubSession() const
{
	throw logic_error("Invalid method.");
	return FALSE;
}

UINT CESBHubServiceImp::GetMaximumSessionNum() const
{
	throw logic_error("Invalid method.");
	return FALSE;
}

UINT CESBHubServiceImp::GetCurrentSessionNum() const
{
	throw logic_error("Invalid method.");
	return FALSE;
}

void CESBHubServiceImp::Dispose()
{
	delete this;
}

void CESBHubServiceImp::_OnTimer_HeartBeatMonitor(Utils::Thread::ITimer*)
{
	for (auto iter = m_mapServices_session.begin(); iter != m_mapServices_session.end(); )
	{
		SREF(CRegisteredService) pService = iter->second;
		if (pService->IsDead())
		{
			// Remove registered service
			wstring session = pService->GetSession();
			if (iter != m_mapServices_session.begin())
			{
				auto tmpIter = iter;
				--tmpIter;
				m_mapServices_session.erase(iter->first);
				iter = tmpIter;
			}
			else
			{
				m_mapServices_session.erase(iter->first);
				iter = m_mapServices_session.begin();
			}

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
			continue;
		}
		++iter;
	}
}

int CESBHubServiceImp::_PreProcessInvoke(SREF(Utils::Thread::IThread) pthread,
										struct soap* psoap,
										std::wstring& wsSession,
										std::wstring& wsInputs,
										BOOL &bNoFuthureProcess,
										std::wstring& wsResults,
										ESBCommon::ENUM_IDTYPE &idType)
{
	idType = ENUM_IDTYPE::IDTYPE_ESBHub;
	ESBServiceRequest request;
	if (!String2Data(request, wsInputs))
		return -1;

	if (request.idType == ENUM_IDTYPE::IDTYPE_ESBService)
		return _ProcessServiceRequest(pthread, psoap, wsSession, request.contents, bNoFuthureProcess, wsResults);
	else if (request.idType == ENUM_IDTYPE::IDTYPE_ESBClient)
		return _ProcessClientRequest(pthread, psoap, wsSession, request.contents, bNoFuthureProcess, wsResults);
	else
		return 0;
}

int CESBHubServiceImp::_ProcessServiceRequest(SREF(Utils::Thread::IThread) pthread,
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
	else if (nodeName == ESBService_HubMethod_UpdateLoadState::NAMES.ROOTNAME)
	{
		ESBService_HubMethod_UpdateLoadState param;
		if (!String2Data(param, wsInputs))
			return -1;
		return _On_ESBService_HubMethod(wsSession, param, wsResults);
	}
	else if (nodeName == ESBHeartBeat::NAMES.ROOTNAME)
	{
		ESBHeartBeat param;
		if (!String2Data(param, wsInputs))
			return -1;
		return _On_ESBService_HubMethod(wsSession, param, wsResults);
	}
	/*
	// TODO: remove the codes later.
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
	*/
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

int CESBHubServiceImp::_ProcessClientRequest(SREF(Utils::Thread::IThread) pthread,
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

int CESBHubServiceImp::_On_ESBService_HubMethod(const std::wstring& session,
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
				pService->UpdateServiceInfo(param);
				pService->UpdateHeartBeat();
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
			newRegService->UpdateHeartBeat();
		}


		// Send the reply.
		ESBServiceSessionReply sessionReply;
		sessionReply.wsServiceSession = wsSession;
		Data2String(results, sessionReply);
		nRet = 0;
	});
	return nRet;
}

int CESBHubServiceImp::_On_ESBService_HubMethod(const std::wstring& session,
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

int CESBHubServiceImp::_On_ESBService_HubMethod(const std::wstring& session,
	const ESBCommon::ESBService_HubMethod_UpdateLoadState& param,
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

		// Update the loadstate information.
		SREF(CRegisteredService) pService = found->second;
		if (!pService->SetLoadState(param.maximumSession, param.currentSessionNum, param.timeStamp))
		{
			nRet = -301;
			return;
		}
		pService->UpdateHeartBeat();

		// Reorder
		wstring wsGuidService = GUID2String(pService->GetServiceGUID());
		std::vector <SREF(CRegisteredService)> &refVec = m_mapServices_guid[wsGuidService];
		_Reorder(refVec);

		// Reply
		ESBService_ReplyOK replyOK;
		if (!Data2String(results, replyOK))
		{
			nRet = -100;
			return;
		}

		nRet = 0;
	});
	return nRet;
}
int CESBHubServiceImp::_On_ESBService_HubMethod(const std::wstring& session,
	const ESBCommon::ESBHeartBeat& param,
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

		SREF(CRegisteredService) pService = found->second;
		pService->UpdateHeartBeat();

		ESBService_ReplyOK replyOK;
		if (!Data2String(results, replyOK))
		{
			nRet = -100;
			return;
		}

		nRet = 0;
	});
	return nRet;
}

int CESBHubServiceImp::_On_ESBService_HubMethod(const std::wstring& session,
	const ESBCommon::ESBService_HubMethod_ClientSessionEnd& param,
	std::wstring& results)
{
	// TODO: add implementation
	return -1;
}

int CESBHubServiceImp::_On_ESBService_HubMethod(const std::wstring& session,
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
		
		pService->UpdateHeartBeat();

		// send new token to service and retrieve the token info.
		ESBClientToken newToken;
		if (!pService->NewToken(newToken))
		{
			nRet = -101;
			return;
		}

		// Reoreder
		_Reorder(refVec);
		/*
		if (nIndex > 0)
		{
			SREF(CRegisteredService) pServicePrev = refVec[nIndex - 1];
			if (pServicePrev->GetCurrentCapacityUsageRate() < pService->GetCurrentCapacityUsageRate())
				swap(refVec[nIndex], refVec[nIndex - 1]);
			
		}*/
		// reply client with the token
		Data2String(results, newToken);
	});
	return nRet;
}

void CESBHubServiceImp::_Reorder(std::vector <SREF(CRegisteredService)> &refVec)
{
	std::sort(refVec.begin(), refVec.end(), [](SREF(CRegisteredService)& spsev1, SREF(CRegisteredService)& spsev2) {
		return spsev1->GetCurrentCapacityUsageRate() > spsev2->GetCurrentCapacityUsageRate();
	});
}

ESBHUBSERVICELIB_API IESBHubService* ESBHubService::CreateESBHubService()
{
	return new CESBHubServiceImp;
}