#pragma once

#include <Utils/SafeCoding/IDisposable.h>

class CRegisteredService : public Utils::SafeCoding::IDisposable
{
private:
	mutable SREF(Utils::Thread::IThread)			m_threadCommu;
	SREF(ESBWebService::IESBWebServiceClient)		m_commu;

	ESBCommon::ESBService_HubMethod_RegisterToHub	m_serviceInfo;
	std::wstring									m_wsSession;
public:
	CRegisteredService(const std::wstring& session, const ESBCommon::ESBService_HubMethod_RegisterToHub& info);
	~CRegisteredService();

	virtual void Dispose();

	std::wstring GetURL() const;
	std::wstring GetSession() const;
	GUID GetServiceGUID() const;
	float GetCurrentCapacityUsageRate() const;

	BOOL NewToken(ESBCommon::ESBClientToken& token);
	BOOL SetLoadState(UINT maxSessions, UINT currentSession, const std::chrono::steady_clock::time_point& timeStamp);
	BOOL UpdateServiceInfo(const ESBCommon::ESBService_HubMethod_RegisterToHub& info, BOOL cmpTimestamp = TRUE);
};

