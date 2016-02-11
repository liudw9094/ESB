#pragma once

#include <ESBMidClient/ESBMidClient_Connection.h>

class CESBConnectionImp : public ESBMidClient::IESBConnection
{
private:
	SREF(ESBWebService::IESBWebServiceClient) m_webClient;
	mutable SREF(Utils::Thread::IThread)	m_threadClient;
	BOOL									m_bValid;
	ESBCommon::ESBClientToken				m_token;
	GUID									m_guidService;
	std::wstring							m_wsInitialURL;
	std::wstring							m_wsCurrentURL;
public:
	CESBConnectionImp();
	~CESBConnectionImp();

	virtual int StartSession(const std::wstring& wsURL, const GUID& guidService);
	virtual int EndSession(void);
	virtual BOOL IsSessionValid(void) const;
	virtual int Send(const std::wstring& wsContent, std::wstring& wsResult);
	virtual std::wstring GetInitialURL();
	virtual std::wstring GetCurrentURL();
	virtual GUID GetServiceGUID();
	virtual void Dispose();

private:
	void _InitializeClientThread();
	void _UninitializeClientThread();
};

