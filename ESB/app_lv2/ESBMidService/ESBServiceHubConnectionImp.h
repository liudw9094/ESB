#pragma once

#include <ESBMidService/ESBMidService_HubConnection.h>
#include <Common/ESBHub.h>

class CESBServiceHubConnectionImp : public ESBMidService::IESBServiceHubConnection
{
private:
	SREF(ESBWebService::IESBWebServiceClient) m_webClient;
	mutable SREF(Utils::Thread::IThread)	m_threadClient;
	BOOL									m_bValid;
	ESBCommon::ESBServiceSessionReply		m_wsHubSession;
	SREF(Utils::Thread::ITimer)				m_timerHeartBeat;
public:
	CESBServiceHubConnectionImp();
	~CESBServiceHubConnectionImp();



	virtual int	RegisterToHub(const std::wstring& wsHubURL,
		const std::wstring& wsServiceURL,
		const GUID guidService,
		const std::wstring& wsServiceName,
		UINT maximumSessionNum,
		UINT currentSessionNum);
	virtual int Unregister();
	virtual BOOL IsValid() const;
	virtual int ModifySessionLimitation(UINT maximumSessionNum);
	virtual int UpdateLoadState(UINT maximumSessionNum, UINT currentSessionNum);

private:
	void _InitializeClientThread();
	void _UninitializeClientThread();
	void _OnHeartBeatTimer();

	/*
	// TODO: remove the codes later.
	virtual int IncreaseSessionLoad();
	virtual int DecreaseSessionLoad();

public:
	BOOL IsHubSessionValid(const std::wstring& wsSession) const;
	*/
};

