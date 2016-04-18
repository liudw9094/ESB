#include "stdafx.h"
#include "ESBMidService.h"
#include "ESBMidServiceImp.h"

using namespace std;
using namespace std::placeholders;
using namespace ESBMidService;
using namespace ESBWebService;
using namespace ESBDataSerialzer;
using namespace ESBCommon;
using namespace Utils::SafeCoding;
using namespace Utils::Thread;
using namespace ESBXMLParser;

CESBMidServiceImp::CESBMidServiceImp(UINT uSessionTimeoutSecs) :
	m_uSessionTimeoutSecs(uSessionTimeoutSecs),
	m_webService(CreateESBWebServiceServer()),
	m_hubConnection(this),
	m_uMaxSessionNum(0),
	m_plkMapUsers(CreateCriticalSection()),
	m_bAuthentication(false)
{
	auto initSessionMgr = [this]() {
		if (m_uSessionTimeoutSecs != 0)
		{
			m_timerSessionMgr = CreateTimer(m_uSessionTimeoutSecs * 1000 / 2 + 1, false);
			m_timerSessionMgr->AddFunc(bind(&CESBMidServiceImp::_OnTimer_SessionMgr, this, _1));
		}
	};
	auto endSessionMgr = [this]() {
		m_timerSessionMgr = NULL;
	};
	m_threadSessionMgr = CreateThread([initSessionMgr](IThread*) {initSessionMgr();}, [endSessionMgr](IThread*) {endSessionMgr();});
}


CESBMidServiceImp::~CESBMidServiceImp()
{
	Stop();
	m_threadSessionMgr = NULL;
}

BOOL CESBMidServiceImp::Start(int nPort, const ESBWebService::SAuthentication *pAuthentication/* = NULL*/)
{
	if (IsStarted())
		return FALSE;
	using namespace std::placeholders;
	auto func = std::bind(&CESBMidServiceImp::_ProcessWebServiceInvoke, this, _1, _2, _3, _4, _5);
	if (!m_webService->SetCallback_OnClientInvoke(func))
		return FALSE;
	if (m_webService->Start(nPort, pAuthentication))
	{
		m_timerSessionMgr->GetOwnerThread()->Invoke([this]() {
			m_timerSessionMgr->Enable(true);
		});
		if (pAuthentication != NULL)
		{
			m_bAuthentication = true;
			m_authentication = *pAuthentication;
			m_authenticationToHub = ESBWebService::SAuthentication();
			m_authenticationToHub.cafile = m_authentication.cafile;
			m_authenticationToHub.capath = m_authentication.capath;
		}
		else
		{
			m_bAuthentication = false;
			m_authentication = ESBWebService::SAuthentication();
			m_authenticationToHub = ESBWebService::SAuthentication();
		}
		return TRUE;
	}
	return FALSE;
}

BOOL CESBMidServiceImp::Stop(void)
{
	if (IsStarted())
	{
		m_timerSessionMgr->GetOwnerThread()->Invoke([this]() {
			m_timerSessionMgr->Enable(false);
		});

		if (m_hubConnection.IsValid())
			m_hubConnection.Unregister();

		int nRet = m_webService->Stop();
		m_webService->SetCallback_OnClientInvoke([](
			SREF(Utils::Thread::IThread) pthread,
			struct soap* psoap,
			const std::wstring& wsSession,
			const std::wstring& wsInputs,
			std::wstring& wsResults) -> int
		{
			return 0;
		});
		
		m_bAuthentication = false;
		m_authentication = ESBWebService::SAuthentication();
		m_authenticationToHub = ESBWebService::SAuthentication();

		return nRet;
	}
	else
		return FALSE;
}

BOOL CESBMidServiceImp::IsStarted(void) const
{
	return m_webService->IsStarted();
}

BOOL CESBMidServiceImp::SetCallback_OnPreInvoke(const IESBService::TOnPreInvokeFunc &func)
{
	if (IsStarted())
		return FALSE;
	m_funcOnPreInvoke = func;
	return TRUE;
}

BOOL CESBMidServiceImp::SetCallback_OnClientSessionConfirmed(const TOnClientSessionConfirmedFunc &func)
{
	if (IsStarted())
		return FALSE;
	m_funcOnClientSessionConfirmed = func;
	return TRUE;
}

BOOL CESBMidServiceImp::SetCallback_OnClientSessionEnd(const TOnClientSessionEndFunc &func)
{
	if (IsStarted())
		return FALSE;
	m_funcOnClientSessionEnd = func;
	return TRUE;
}

BOOL CESBMidServiceImp::SetCallback_OnRegisteredOnHub(const TOnRegisteredOnHubFunc &func)
{
	if (IsStarted())
		return FALSE;
	m_funcOnRegisteredOnHub = func;
	return TRUE;
}

BOOL CESBMidServiceImp::SetCallback_OnUnregisteredFromHub(const TOnUnregisteredFromHubFunc &func)
{
	if (IsStarted())
		return FALSE;
	m_hubConnection.SetCallback_OnUnregisteredFromHub(func);
	return TRUE;
}

BOOL CESBMidServiceImp::SetCallback_OnHubSessionLost(const TOnHubSessionLostFunc &func)
{
	if (IsStarted())
		return FALSE;
	m_hubConnection.SetCallback_OnHubSessionLost(func);
	return TRUE;
}

BOOL CESBMidServiceImp::SetCallback_OnClientInvoke(const IESBService::TOnClientInvokeFunc &func)
{
	if (IsStarted())
		return FALSE;
	m_funcOnInvoke = func;
	return TRUE;
}

BOOL CESBMidServiceImp::SetCallback_OnAccept(const IESBService::TOnAcceptFunc& func)
{
	return m_webService->SetCallback_OnAccept(func);
}

BOOL CESBMidServiceImp::SetCallback_OnStarted(const TOnStartFunc& func)
{
	return m_webService->SetCallback_OnStarted(func);
}

BOOL CESBMidServiceImp::SetCallback_OnStoped(const TOnStopFunc& func)
{
	return m_webService->SetCallback_OnStoped(func);
}

int	CESBMidServiceImp::RegisterToHub(const std::wstring& wsHubURL,
									const std::wstring& wsServiceURL,
									const GUID guidService,
									const std::wstring& wsServiceName,
									UINT maximumSession)
{
	if (!IsStarted())
		return -1;
	int nRet = 0;
	UINT curNum = 0;
	{
		SLOCK(m_plkMapUsers);
		curNum = m_mapUsers.size();
	}
	if (m_bAuthentication)
		nRet = m_hubConnection.RegisterToHub(wsHubURL, wsServiceURL, guidService, wsServiceName, maximumSession, curNum, &m_authenticationToHub);
	else
		nRet = m_hubConnection.RegisterToHub(wsHubURL, wsServiceURL, guidService, wsServiceName, maximumSession, curNum, NULL);
	if (0 == nRet)
	{
		::InterlockedExchange((volatile long*)&m_uMaxSessionNum, maximumSession);
		if (m_funcOnRegisteredOnHub)
			m_funcOnRegisteredOnHub(this);
	}
	return nRet;
}

int CESBMidServiceImp::GetPort(void) const
{
	return m_webService->GetPort();
}

std::wstring CESBMidServiceImp::GetClientIP(const struct soap* pSoap) const
{
	return m_webService->GetClientIP(pSoap);
}

IESBServiceHubConnection* CESBMidServiceImp::GetHubConnection()
{
	return &m_hubConnection;
}

BOOL CESBMidServiceImp::IsClientSessionExisted(const std::wstring& wsSession) const
{
	SLOCK(m_plkMapUsers);
	return m_mapUsers.find(wsSession) != m_mapUsers.end();
}

BOOL CESBMidServiceImp::IsClientSessionValid(const std::wstring& wsSession) const
{
	SLOCK(m_plkMapUsers);
	auto it = m_mapUsers.find(wsSession);
	if (it == m_mapUsers.end())
		return FALSE;
	return it->second.bConfirmed;
}

BOOL CESBMidServiceImp::RemoveClientSession(const std::wstring& wsSession, EMSessionEndReason reason /*= EMSessionEndReason::SERVER_MANIPULATE*/)
{
	SLOCK(m_plkMapUsers);
	auto it = m_mapUsers.find(wsSession);
	if (it == m_mapUsers.end())
		return FALSE;
	m_mapUsers.erase(it);

	if (m_funcOnClientSessionEnd)
		m_funcOnClientSessionEnd(this, wsSession, reason);
	return TRUE;
}

BOOL CESBMidServiceImp::CheckHubSession() const
{
	return m_hubConnection.IsValid();
}

UINT CESBMidServiceImp::GetMaximumSessionNum() const
{
	return m_uMaxSessionNum;
}

UINT CESBMidServiceImp::GetCurrentSessionNum() const
{
	SLOCK(m_plkMapUsers);
	return m_mapUsers.size();
}

void CESBMidServiceImp::Dispose()
{
	delete this;
}

void CESBMidServiceImp::_UpdateClientSessionTimePoint(const wstring& wsSession)
{
	SLOCK(m_plkMapUsers);
	auto it = m_mapUsers.find(wsSession);
	if (it == m_mapUsers.end())
		return;
	it->second.tpSession = chrono::steady_clock::now();
}

void CESBMidServiceImp::_OnTimer_SessionMgr(Utils::Thread::ITimer *)
{
	bool bChanged = false;
	SLOCK(m_plkMapUsers);
	for (auto iter = m_mapUsers.begin(); iter != m_mapUsers.end(); )
	{
		CLIENTINFO& userInfo = iter->second;
		if (userInfo.tpSession + chrono::seconds(m_uSessionTimeoutSecs) < chrono::steady_clock::now())
		{
			bChanged = true;
			// Remove registered service
			wstring session = iter->first;
			if (iter != m_mapUsers.begin())
			{
				auto tmpIter = iter;
				--tmpIter;
				m_mapUsers.erase(iter->first);
				iter = tmpIter;
			}
			else
			{
				m_mapUsers.erase(iter->first);
				iter = m_mapUsers.begin();
			}

			if (m_funcOnClientSessionEnd)
				m_funcOnClientSessionEnd(this, session, EMSessionEndReason::SESSION_EXPIRED);

			continue;
		}
		++iter;
	}
	
	if(bChanged)
		m_hubConnection.UpdateLoadState(m_uMaxSessionNum, m_mapUsers.size());
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
	ENUM_IDTYPE idType = ENUM_IDTYPE::IDTYPE_ESBService;
	CFinalize final1([&wsResults, &_wsResults, &idType]() {
		ESBServiceReply rp;
		rp.idType = idType;
		rp.contents = _wsResults;
		Data2String(wsResults, rp);
	});

	BOOL bNoFurtherProcess = FALSE;
	if(m_funcOnPreInvoke)
	{
		int nRet = m_funcOnPreInvoke(pthread, psoap, _wsSession, _wsInputs, bNoFurtherProcess, _wsResults, idType);
		if (nRet < 0 || bNoFurtherProcess)
		{
			return nRet;
		}
	}
	
	ESBServiceRequest request;
	if (!String2Data(request, _wsInputs))
		return -1;
	_wsInputs = request.contents;

	if (request.idType == ENUM_IDTYPE::IDTYPE_ESBHub)
		return _ProcessHubRequest(pthread, psoap, _wsSession, _wsInputs, _wsResults);
	else if (request.idType == ENUM_IDTYPE::IDTYPE_ESBClient)
	{
		if (!IsClientSessionExisted(_wsSession))
		{
			// TODO: modify the error message.
			_wsResults = L"No such session exists.";
			return -102;
		}

		_UpdateClientSessionTimePoint(_wsSession);
		
		ESBService_ServiceMethod_ClientRequest userRequest;
		if (!String2Data(userRequest, _wsInputs))
			return _ProcessClientRequest(pthread, psoap, _wsSession, _wsInputs, _wsResults);
		else
		{
			if (!IsClientSessionValid(_wsSession))
			{
				// TODO: modify the error message.
				_wsResults = L"Session Invalid.";
				return -102;
			}

			if (m_funcOnInvoke)
				return m_funcOnInvoke(pthread, psoap, _wsSession, userRequest.wsContent, _wsResults);
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
	ESBServiceToken param;
	if (String2Data(param, wsInputs))
		return _On_ESBService_ServiceMethod(wsSession, param, wsResults);

	return -101;
}

int CESBMidServiceImp::_ProcessClientRequest(SREF(Utils::Thread::IThread) pthread,
											struct soap* psoap,
											const std::wstring& wsSession,
											const std::wstring& wsInputs,
											std::wstring& wsResults)
{
	{
		ESBService_ServiceMethod_SessionConfirm sessionConfirm;
		if (String2Data(sessionConfirm, wsInputs))
		{
			SLOCK(m_plkMapUsers);
			CLIENTINFO& clientInfo = m_mapUsers[wsSession];
			auto tmNow = chrono::system_clock::now();
			if (tmNow >= clientInfo.token.timeStamp &&
				tmNow <= clientInfo.token.timeReplyDeadLine)
			{
				clientInfo.bConfirmed = TRUE;
				ESBService_ReplyOK replyOK;
				if (!Data2String(wsResults, replyOK))
					return -101;
			}
			else
			{
				m_mapUsers.erase(wsSession);
				return -205;
			}

			if (m_funcOnClientSessionConfirmed)
				m_funcOnClientSessionConfirmed(this, wsSession);

			return 0;
		}
	}
	{
		ESBService_ServiceMethod_EndSession sessionEnd;
		if (String2Data(sessionEnd, wsInputs))
		{
			{
				SLOCK(m_plkMapUsers);
				m_mapUsers.erase(wsSession);
				m_hubConnection.UpdateLoadState(m_uMaxSessionNum, m_mapUsers.size());
			}
			ESBService_ReplyOK replyOK;
			if (!Data2String(wsResults, replyOK))
				return -101;


			if (m_funcOnClientSessionEnd)
				m_funcOnClientSessionEnd(this, wsSession, EMSessionEndReason::CLIENT_REQUEST);

			return 0;
		}
	}
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
	// correct timestamp
	auto time_now = chrono::system_clock::now();
	if (time_now != clientInfo.token.timeStamp)
	{
		auto period = max(clientInfo.token.timeReplyDeadLine - clientInfo.token.timeStamp, chrono::seconds(1));
		clientInfo.token.timeStamp = time_now;
		clientInfo.token.timeReplyDeadLine = time_now + period;
	}
	m_mapUsers[clientInfo.token.wsClientSession] = clientInfo;
	reply.currentSessionNum = m_mapUsers.size();
	reply.maximumSession = m_uMaxSessionNum;

	Data2String(results, reply);


	return 0;
}

ESBMIDSERVICE_API IESBService* ESBMidService::CreateESBService(UINT uSessionTimeoutSecs /*= 600*/)
{
	return new CESBMidServiceImp(uSessionTimeoutSecs);
}