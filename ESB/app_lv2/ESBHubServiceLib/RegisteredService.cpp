#include "stdafx.h"
#include "RegisteredService.h"

using namespace std;
using namespace ESBMidService;
using namespace Utils::Thread;
using namespace ESBWebService;
using namespace ESBCommon;
using namespace ESBDataSerialzer;
using namespace ESBXMLParser;

CRegisteredService::CRegisteredService(const std::wstring& session,
	const ESBService_HubMethod_RegisterToHub param) :
	m_threadCommu(CreateThread()),
	m_commu(CreateESBWebServiceClient()),
	m_paramCreated(param),
	m_wsSession(session)
{
	m_commu->SetURL(param.wsServiceURL);
}

CRegisteredService::~CRegisteredService()
{

}

void CRegisteredService::Dispose()
{
	delete this;
}

std::wstring&& CRegisteredService::GetURL() const
{
	wstring wsURL;
	m_threadCommu->Invoke([this, &wsURL]() {
		wsURL = m_paramCreated.wsServiceURL;
	});
	return move(wsURL);
}

std::wstring&& CRegisteredService::GetSession() const
{
	wstring wsSession = m_wsSession;
	m_threadCommu->Invoke([this, &wsSession]() {
		wsSession = m_wsSession;
	});
	return move(wsSession);
}

GUID&& CRegisteredService::GetServiceGUID() const
{
	GUID guidService = { 0 };
	m_threadCommu->Invoke([this, &guidService]() {
		guidService = m_paramCreated.guidService;
	});
	return move(guidService);
}

float CRegisteredService::GetCurrentCapacityUsageRate() const
{
	float rate = { 0 };
	m_threadCommu->Invoke([this, &rate]() {
		rate = (float)m_paramCreated.currentSessionNum / (float)m_paramCreated.maximumSession;
	});
	return rate;
}

BOOL CRegisteredService::NewToken(ESBCommon::ESBClientToken& token)
{
	BOOL bRet = FALSE;
	m_threadCommu->Invoke([this, &bRet, &token]() {
		if (m_paramCreated.currentSessionNum >= m_paramCreated.maximumSession)
			return;

		ESBCommon::ESBClientToken newToken_Client;
		ESBCommon::ESBServiceToken newToken_Service;
		newToken_Service.wsClientSession = newToken_Client.wsClientSession = CreateGuid();
		newToken_Service.timeStamp = newToken_Client.timeStamp = time(NULL);
		newToken_Service.timeReplyDeadLine = newToken_Client.timeReplyDeadLine = newToken_Client.timeStamp + 120;
		newToken_Client.wsURLRedirection = m_paramCreated.wsServiceURL;

		ESBServiceRequest request;
		request.idType = IDTYPE_ESBHub;
		Data2String(request.contents, newToken_Service);
		wstring wsRequest, wsReply;
		if (m_commu->Invoke(m_wsSession, wsRequest, wsReply) != 0)
			return;

		ESBServiceReply reply;
		if (!String2Data(reply, wsReply) || reply.idType != IDTYPE_ESBService)
			return;

		ESBService_ServiceReply_LoadStateUpdate replyContent;
		if (!String2Data(replyContent, reply.contents))
			return;

		// Update load state.
		m_paramCreated.maximumSession = replyContent.maximumSession;
		m_paramCreated.currentSessionNum = replyContent.currentSessionNum;

		bRet = TRUE;
		return;
	});
	return bRet;
}
