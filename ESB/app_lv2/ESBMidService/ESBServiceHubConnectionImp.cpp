#include "stdafx.h"
#include "ESBServiceHubConnectionImp.h"

using namespace std;
using namespace ESBCommon;
using namespace Utils::Thread;
using namespace ESBDataSerialzer;
using namespace ESBWebService;

CESBServiceHubConnectionImp::CESBServiceHubConnectionImp() :
	m_webClient(CreateESBWebServiceClient()),
	m_bValid(FALSE)
{
	m_threadClient = CreateThread([this](IThread*) {_InitializeClientThread();}, [this](IThread*) {_UninitializeClientThread();});
}


CESBServiceHubConnectionImp::~CESBServiceHubConnectionImp()
{
	m_threadClient = NULL;
}

int	CESBServiceHubConnectionImp::RegisterToHub(const std::wstring& wsHubURL,
	const std::wstring& wsServiceURL,
	const GUID guidService,
	const std::wstring& wsServiceName,
	UINT maximumSession,
	UINT currentSessionNum)
{
	int nRet = 0;
	ESBService_HubMethod_RegisterToHub reg;
	reg.wsServiceURL = wsServiceURL;
	reg.guidService = guidService;
	reg.wsServiceName = wsServiceName;
	reg.maximumSession = maximumSession;
	reg.currentSessionNum = currentSessionNum;
	reg.timeStamp = chrono::steady_clock::now();
	ESBServiceRequest rq;
	rq.idType = ENUM_IDTYPE::IDTYPE_ESBService;
	if (!Data2String(rq.contents, reg))
		return -2;
	wstring wsRequest;
	if (!Data2String(wsRequest, rq))
		return -3;

	m_threadClient->Invoke([this, &wsHubURL, wsRequest, &nRet]() {
		if (IsValid())
		{
			nRet = -1;
			return;
		}
		m_webClient->SetURL(wsHubURL);

		wstring wsReply;
		nRet = m_webClient->Invoke(L"", wsRequest, wsReply);
		if (nRet != 0)
			return;
		ESBServiceReply rp;
		if (!String2Data(rp, wsReply) || rp.idType != ENUM_IDTYPE::IDTYPE_ESBHub)
		{
			nRet = -4;
			return;
		}
		if (!String2Data(m_wsHubSession, rp.contents))
		{
			nRet = -5;
			return;
		}

		m_timerHeartBeat->Enable(true);
		m_bValid = TRUE;
		nRet = 0;
	});
	return nRet;
}

int CESBServiceHubConnectionImp::Unregister()
{
	int nRet = 0;

	m_threadClient->Invoke([this, &nRet]() {
		m_timerHeartBeat->Enable(false);
		m_wsHubSession = ESBServiceSessionReply();
		m_bValid = FALSE;

		ESBService_HubMethod_Unregister command;
		ESBServiceRequest rq;
		rq.idType = ENUM_IDTYPE::IDTYPE_ESBService;
		if (!Data2String(rq.contents, command))
		{
			nRet = -2;
			return;
		}
		wstring wsRequest;
		if (!Data2String(wsRequest, rq))
		{
			nRet = -3;
			return;
		}

		if (!IsValid())
		{
			nRet = -1;
			return;
		}

		wstring wsReply;
		nRet = m_webClient->Invoke(m_wsHubSession.wsServiceSession, wsRequest, wsReply);
		if (nRet != 0)
			return;
		ESBServiceReply rp;
		if (!String2Data(rp, wsReply) || rp.idType != ENUM_IDTYPE::IDTYPE_ESBHub)
		{
			nRet = -3;
			return;
		}
		ESBService_ReplyOK replyContent;
		if (!String2Data(replyContent, rp.contents))
		{
			nRet = -4;
			return;
		}
		nRet = 0;
	});
	return nRet;
}

BOOL CESBServiceHubConnectionImp::IsValid() const
{
	BOOL bValid = FALSE;
	m_threadClient->Invoke([this, &bValid]() {
		bValid = m_bValid;
	});
	return bValid;
}

int CESBServiceHubConnectionImp::ModifySessionLimitation(UINT maximumSessionNum)
{
	return UpdateLoadState(maximumSessionNum, ESBService_HubMethod_UpdateLoadState::DONT_CHANGE);
}

int CESBServiceHubConnectionImp::UpdateLoadState(UINT maximumSessionNum, UINT currentSessionNum)
{
	int nRet = 0;

	m_threadClient->Invoke([this, &maximumSessionNum, &currentSessionNum, &nRet]() {

		ESBService_HubMethod_UpdateLoadState command;
		command.maximumSession = maximumSessionNum;
		command.currentSessionNum = currentSessionNum;
		command.timeStamp = chrono::steady_clock::now();
		ESBServiceRequest rq;
		rq.idType = ENUM_IDTYPE::IDTYPE_ESBService;
		if (!Data2String(rq.contents, command))
		{
			nRet = -2;
			return;
		}
		wstring wsRequest;
		if (!Data2String(wsRequest, rq))
		{
			nRet = -3;
			return;
		}


		if (!IsValid())
		{
			nRet = -1;
			return;
		}

		wstring wsReply;
		nRet = m_webClient->Invoke(m_wsHubSession.wsServiceSession, wsRequest, wsReply);
		if (nRet != 0)
			return;
		ESBServiceReply rp;
		if (!String2Data(rp, wsReply) || rp.idType != ENUM_IDTYPE::IDTYPE_ESBHub)
		{
			nRet = -3;
			return;
		}
		ESBService_ReplyOK replyContent;
		if (!String2Data(replyContent, rp.contents))
		{
			nRet = -4;
			return;
		}

		nRet = 0;
	});
	return nRet;
}

void CESBServiceHubConnectionImp::_InitializeClientThread()
{
	::CoInitialize(NULL);
	m_timerHeartBeat = CreateTimer(SERVICE_SESSION_HEARTBEAT_TIME_INTERVAL * 1000, false);
	m_timerHeartBeat->AddFunc(bind(&CESBServiceHubConnectionImp::_OnHeartBeatTimer, this));
}

void CESBServiceHubConnectionImp::_UninitializeClientThread()
{
	m_timerHeartBeat = NULL;
	::CoUninitialize();
}

void CESBServiceHubConnectionImp::_OnHeartBeatTimer()
{
	m_threadClient->Invoke([this]() {
		if (!IsValid())
		{
			m_timerHeartBeat->Enable(false);
			Unregister();
			return;
		}

		ESBHeartBeat command;
		ESBServiceRequest rq;
		rq.idType = ENUM_IDTYPE::IDTYPE_ESBService;
		if (!Data2String(rq.contents, command))
			throw std::logic_error("Unexpected behavior.");

		wstring wsRequest;
		if (!Data2String(wsRequest, rq))
			throw std::logic_error("Unexpected behavior.");

		wstring wsReply;
		int nRet = m_webClient->Invoke(m_wsHubSession.wsServiceSession, wsRequest, wsReply);
		ESBServiceReply rp;
		ESBService_ReplyOK replyContent;
		if (nRet != 0 ||
			!String2Data(rp, wsReply) ||
			rp.idType != ENUM_IDTYPE::IDTYPE_ESBHub ||
			!String2Data(replyContent, rp.contents))
		{
			m_timerHeartBeat->Enable(false);
			Unregister();
		}

	});
}

/*
// TODO: remove the codes later.
int CESBServiceHubConnectionImp::IncreaseSessionLoad()
{
	int nRet = 0;
	ESBService_HubMethod_IncreaseSessionLoad command;
	ESBServiceRequest rq;
	rq.idType = IDTYPE_ESBService;
	if (!Data2String(rq.contents, command))
		return -2;
	wstring wsRequest;
	if (!Data2String(wsRequest, rq))
		return -3;

	m_threadClient->Invoke([this, &wsRequest, &nRet]() {
		if (!IsValid())
		{
			nRet = -1;
			return;
		}

		wstring wsReply;
		nRet = m_webClient->Invoke(L"", wsRequest, wsReply);
		if (nRet != 0)
			return;
		ESBServiceReply rp;
		if (!String2Data(rp, wsReply) || rp.idType != IDTYPE_ESBHub)
		{
			nRet = -3;
			return;
		}
		ESBService_ReplyOK replyContent;
		if (!String2Data(replyContent, rp.contents))
		{
			nRet = -4;
			return;
		}

		nRet = 0;
	});
	return nRet;
}

int CESBServiceHubConnectionImp::DecreaseSessionLoad()
{
	int nRet = 0;
	ESBService_HubMethod_DecreaseSessionLoad command;
	ESBServiceRequest rq;
	rq.idType = IDTYPE_ESBService;
	if (!Data2String(rq.contents, command))
		return -2;
	wstring wsRequest;
	if (!Data2String(wsRequest, rq))
		return -3;

	m_threadClient->Invoke([this, &wsRequest, &nRet]() {
		if (!IsValid())
		{
			nRet = -1;
			return;
		}

		wstring wsReply;
		nRet = m_webClient->Invoke(L"", wsRequest, wsReply);
		if (nRet != 0)
			return;
		ESBServiceReply rp;
		if (!String2Data(rp, wsReply) || rp.idType != IDTYPE_ESBHub)
		{
			nRet = -3;
			return;
		}
		ESBService_ReplyOK replyContent;
		if (!String2Data(replyContent, rp.contents))
		{
			nRet = -4;
			return;
		}

		nRet = 0;
	});
	return nRet;
}



BOOL CESBServiceHubConnectionImp::IsHubSessionValid(const wstring& wsSession) const
{
	BOOL bValid = FALSE;
	m_threadClient->Invoke([this, &bValid, &wsSession]() {
		bValid = (wsSession == m_wsHubSession.wsServiceSession);
	});
	return bValid;
}

*/