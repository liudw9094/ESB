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
	const ESBService_HubMethod_RegisterToHub& info)
	: m_threadCommu(CreateThread()),
	m_commu(CreateESBWebServiceClient()),
	m_serviceInfo(info),
	m_wsSession(session)
{
	m_commu->SetURL(info.wsServiceURL);
}

CRegisteredService::~CRegisteredService()
{

}

void CRegisteredService::Dispose()
{
	delete this;
}

std::wstring CRegisteredService::GetURL() const
{
	wstring wsURL;
	m_threadCommu->Invoke([this, &wsURL]() {
		wsURL = m_serviceInfo.wsServiceURL;
	});
	return wsURL;
}

std::wstring CRegisteredService::GetSession() const
{
	wstring wsSession = m_wsSession;
	m_threadCommu->Invoke([this, &wsSession]() {
		wsSession = m_wsSession;
	});
	return wsSession;
}

GUID CRegisteredService::GetServiceGUID() const
{
	GUID guidService = { 0 };
	m_threadCommu->Invoke([this, &guidService]() {
		guidService = m_serviceInfo.guidService;
	});
	return guidService;
}

float CRegisteredService::GetCurrentCapacityUsageRate() const
{
	float rate = { 0 };
	m_threadCommu->Invoke([this, &rate]() {
		rate = (float)m_serviceInfo.currentSessionNum / (float)m_serviceInfo.maximumSession;
	});
	return rate;
}

BOOL CRegisteredService::NewToken(ESBCommon::ESBClientToken& token)
{
	BOOL bRet = FALSE;
	m_threadCommu->Invoke([this, &bRet, &token]() {
		if (m_serviceInfo.currentSessionNum >= m_serviceInfo.maximumSession)
			return;

		ESBCommon::ESBClientToken newToken_Client;
		ESBCommon::ESBServiceToken newToken_Service;
		newToken_Service.wsClientSession = newToken_Client.wsClientSession = CreateGuid();
		newToken_Service.timeStamp = newToken_Client.timeStamp = chrono::steady_clock::now();
		newToken_Service.timeReplyDeadLine = newToken_Client.timeReplyDeadLine = newToken_Client.timeStamp + chrono::seconds(120);
		newToken_Client.wsURLRedirection = m_serviceInfo.wsServiceURL;

		ESBServiceRequest request;
		request.idType = IDTYPE_ESBHub;
		Data2String(request.contents, newToken_Service);
		wstring wsRequest, wsReply;
		Data2String(wsRequest, request);
		if (m_commu->Invoke(m_wsSession, wsRequest, wsReply) != 0)
			return;

		ESBServiceReply reply;
		if (!String2Data(reply, wsReply) || reply.idType != IDTYPE_ESBService)
			return;

		ESBService_ServiceReply_LoadStateUpdate replyContent;
		if (!String2Data(replyContent, reply.contents))
			return;

		// Update load state.
		m_serviceInfo.maximumSession = replyContent.maximumSession;
		m_serviceInfo.currentSessionNum = replyContent.currentSessionNum;

		token = newToken_Client;
		bRet = TRUE;
		return;
	});
	return bRet;
}

BOOL CRegisteredService::SetLoadState(UINT maxSessions, UINT currentSession, const std::chrono::steady_clock::time_point& timeStamp)
{
	BOOL nRet = FALSE;
	m_threadCommu->Invoke([this, &nRet, maxSessions, currentSession, &timeStamp]() {
		if (m_serviceInfo.timeStamp < timeStamp)
		{
			m_serviceInfo.maximumSession = maxSessions;
			m_serviceInfo.currentSessionNum = currentSession;
			m_serviceInfo.timeStamp = timeStamp;
			nRet = TRUE;
		}
	});
	return nRet;
}

BOOL CRegisteredService::UpdateServiceInfo(const ESBCommon::ESBService_HubMethod_RegisterToHub& info, BOOL cmpTimestamp /*= TRUE*/)
{
	BOOL nRet = FALSE;
	m_threadCommu->Invoke([this, &nRet, &info, cmpTimestamp]() {
		if (!cmpTimestamp || m_serviceInfo.timeStamp < info.timeStamp)
		{
			m_serviceInfo = info;
			nRet = TRUE;
		}
	});
	return nRet;
}
