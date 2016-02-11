#include "stdafx.h"
#include "ESBMidConn.h"
#include "ESBConnectionImp.h"

using namespace std;
using namespace ESBMidClient;
using namespace Utils::Thread;
using namespace ESBCommon;
using namespace ESBDataSerialzer;
using namespace ESBWebService;

CESBConnectionImp::CESBConnectionImp() :
	m_webClient(CreateESBWebServiceClient()),
	m_threadClient(CreateThread([this](IThread*) {_InitializeClientThread();}, [this](IThread*) {_UninitializeClientThread();})),
	m_bValid(FALSE)
{
	memset(&m_guidService, 0, sizeof(m_guidService));
}


CESBConnectionImp::~CESBConnectionImp()
{
	if (IsSessionValid())
		EndSession();
}

int CESBConnectionImp::StartSession(const std::wstring& wsURL, const GUID& guidService)
{
	int nRet = 0;

	m_threadClient->Invoke([this, &wsURL, &guidService, &nRet]() {

		if (IsSessionValid())
		{
			nRet = -1;
			return;
		}

		ESBService_HubMethod_StartSession reg;
		reg.guidService = guidService;
		ESBServiceRequest rq;
		rq.idType = ENUM_IDTYPE::IDTYPE_ESBClient;
		if (!Data2String(rq.contents, reg))
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

		wstring wsInitialURL = wsURL;
		wstring wsCurrentURL;
		m_webClient->SetURL(wsInitialURL);

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
		ESBClientToken token;
		if (!String2Data(token, rp.contents))
		{
			nRet = -5;
			return;
		}

		wsCurrentURL = token.wsURLRedirection;
		m_webClient->SetURL(wsCurrentURL);

		ESBService_ServiceMethod_SessionConfirm sessionConfirm;
		ESBServiceRequest rq2;
		rq2.idType = ENUM_IDTYPE::IDTYPE_ESBClient;
		if (!Data2String(rq2.contents, sessionConfirm) ||
			!Data2String(wsRequest, rq2))
		{
			nRet = -6;
			return;
		}
		nRet = m_webClient->Invoke(token.wsClientSession, wsRequest, wsReply);
		if (nRet != 0)
			return;
		if (!String2Data(rp, wsReply) || rp.idType != ENUM_IDTYPE::IDTYPE_ESBService)
		{
			nRet = -7;
			return;
		}
		ESBService_ReplyOK replyOK;
		if (!String2Data(replyOK, rp.contents))
		{
			nRet = -8;
			return;
		}

		m_token = token;
		m_guidService = guidService;
		m_wsInitialURL = wsInitialURL;
		m_wsCurrentURL = wsCurrentURL;
		m_bValid = TRUE;
		nRet = 0;
	});

	return nRet;
}

int CESBConnectionImp::EndSession(void)
{
	int nRet = 0;

	m_threadClient->Invoke([this, &nRet]() {
		if (!IsSessionValid())
		{
			nRet = -1;
			return;
		}

		ESBService_ServiceMethod_EndSession reg;
		ESBServiceRequest rq;
		rq.idType = ENUM_IDTYPE::IDTYPE_ESBClient;
		if (!Data2String(rq.contents, reg))
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

		wstring wsReply;
		nRet = m_webClient->Invoke(m_token.wsClientSession, wsRequest, wsReply);
		if (nRet != 0)
			return;
		ESBServiceReply rp;
		if (!String2Data(rp, wsReply) || rp.idType != ENUM_IDTYPE::IDTYPE_ESBService)
		{
			nRet = -4;
			return;
		}
		ESBService_ReplyOK replyOK;
		if (!String2Data(replyOK, rp.contents))
		{
			nRet = -5;
			return;
		}

		m_token = ESBClientToken();
		memset(&m_guidService, 0, sizeof(m_guidService));
		m_wsInitialURL = L"";
		m_wsCurrentURL = L"";
		m_bValid = FALSE;
		nRet = 0;
	});

	return nRet;
}

BOOL CESBConnectionImp::IsSessionValid(void) const
{
	BOOL nRet = 0;

	m_threadClient->Invoke([this, &nRet]() {
		nRet = m_bValid;
	});

	return nRet;
}

int CESBConnectionImp::Send(const std::wstring& wsContent, std::wstring& wsResult)
{
	int nRet = 0;

	m_threadClient->Invoke([this, &nRet, &wsContent, &wsResult]() {
		if (!IsSessionValid())
		{
			nRet = -1;
			return;
		}

		ESBService_ServiceMethod_ClientRequest clientreq;
		clientreq.wsContent = wsContent;
		ESBServiceRequest rq;
		rq.idType = ENUM_IDTYPE::IDTYPE_ESBClient;
		if (!Data2String(rq.contents, clientreq))
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

		wstring wsReply;
		nRet = m_webClient->Invoke(m_token.wsClientSession, wsRequest, wsReply);
		if (nRet != 0)
		{
			wsResult = wsReply;
			return;
		}

		ESBServiceReply rp;
		if (!String2Data(rp, wsReply) || rp.idType != ENUM_IDTYPE::IDTYPE_ESBService)
		{
			nRet = -4;
			return;
		}
		wsResult = rp.contents;

		nRet = 0;
	});

	return nRet;
}

std::wstring CESBConnectionImp::GetInitialURL()
{
	wstring wsRet;

	m_threadClient->Invoke([this, &wsRet]() {
		wsRet = m_wsInitialURL;
	});

	return wsRet;
}

std::wstring CESBConnectionImp::GetCurrentURL()
{
	wstring wsRet;

	m_threadClient->Invoke([this, &wsRet]() {
		wsRet = m_wsCurrentURL;
	});

	return wsRet;
}

GUID CESBConnectionImp::GetServiceGUID()
{
	GUID guidRet = { 0 };

	m_threadClient->Invoke([this, &guidRet]() {
		guidRet = m_guidService;
	});

	return guidRet;
}

void CESBConnectionImp::Dispose()
{
	delete this;
}

void CESBConnectionImp::_InitializeClientThread()
{
	::CoInitialize(NULL);
}

void CESBConnectionImp::_UninitializeClientThread()
{
	::CoUninitialize();
}

ESBMIDCONN_API IESBConnection* ESBMidClient::CreateESBConnection()
{
	return new CESBConnectionImp;
}