#include "stdafx.h"
#include "ESBServiceHubConnectionImp.h"

using namespace std;
using namespace ESBCommon;
using namespace Utils::Thread;
using namespace ESBDataSerialzer;
using namespace ESBWebService;

CESBServiceHubConnectionImp::CESBServiceHubConnectionImp() :
	m_webClient(CreateESBWebServiceClient()),
	m_threadClient(CreateThread()),
	m_bValid(FALSE)
{
}


CESBServiceHubConnectionImp::~CESBServiceHubConnectionImp()
{
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
	reg.wsServiceName = wsServiceURL;
	reg.guidService = guidService;
	reg.wsServiceName = wsServiceName;
	reg.maximumSession = maximumSession;
	reg.currentSessionNum = currentSessionNum;
	reg.timeStamp = chrono::steady_clock::now();
	ESBServiceRequest rq;
	rq.idType = IDTYPE_ESBService;
	if (!Data2String(rq.contents, reg))
		return -2;
	wstring wsRequest;
	if (!Data2String(wsRequest, rq))
		return -3;

	m_threadClient->Invoke([this, &wsHubURL, &wsRequest, &nRet]() {
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
		if (!String2Data(rp, wsReply) || rp.idType != IDTYPE_ESBHub)
		{
			nRet = -4;
			return;
		}
		if (!String2Data(m_wsHubSession, rp.contents))
		{
			nRet = -5;
			return;
		}
		m_bValid = TRUE;
		nRet = 0;
	});
	return nRet;
}

int CESBServiceHubConnectionImp::Unregister()
{
	int nRet = 0;
	ESBService_HubMethod_Unregister command;
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

		m_bValid = FALSE;
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
	ESBService_HubMethod_UpdateLoadState command;
	command.maximumSession = maximumSessionNum;
	command.currentSessionNum = currentSessionNum;
	command.timeStamp = chrono::steady_clock::now();
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

		m_bValid = FALSE;
		nRet = 0;
	});
	return nRet;
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

		m_bValid = FALSE;
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

		m_bValid = FALSE;
		nRet = 0;
	});
	return nRet;
}

*/

BOOL CESBServiceHubConnectionImp::IsHubSessionValid(const wstring& wsSession)
{
	BOOL bValid = FALSE;
	m_threadClient->Invoke([this, &bValid, &wsSession]() {
		bValid = (wsSession == m_wsHubSession.wsServiceSession);
	});
	return bValid;
}

