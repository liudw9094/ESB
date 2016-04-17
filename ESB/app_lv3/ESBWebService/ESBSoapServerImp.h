#pragma once

#include <map>
#include <string>
#include <Utils/Thread.h>
#include <ESBWebService/ESBWebServiceServer.h>
#include "soapH.h"

class CESBSoapServerImp : public ESBWebService::IESBWebServiceServer
{
private:
	soap *m_soap;
	volatile BOOL	m_bIsStarted;
	int m_nPort;
	ESBWebService::SAuthentication *m_pAuthentication;
	SREF(Utils::Thread::IThread) m_thdSoap;
	SREF(Utils::Thread::ICriticalSection) m_csSoapEnd;
	volatile BOOL	m_bExitThread;

	mutable SREF(Utils::Thread::ICriticalSection) m_plkMapAcceptSoap;
	std::map<struct soap*, SREF(Utils::Thread::IThread)>m_mapAcceptSoap;
	TOnClientInvokeFunc m_funcOnInvoke;
	TOnAcceptFunc m_funcOnAccept;
	TOnStartFunc m_funcOnStarted;
	TOnStopFunc m_funcOnStoped;
public:
	CESBSoapServerImp(void);
	~CESBSoapServerImp(void);

public:
	virtual BOOL Start(int nPort, const ESBWebService::SAuthentication *pAuthentication = NULL);
	virtual BOOL Stop();
	virtual BOOL IsStarted() const;
	virtual int GetPort() const;
	virtual std::wstring GetClientIP(const struct soap* pSoap) const;
	virtual BOOL SetCallback_OnClientInvoke(const TOnClientInvokeFunc& func);
	virtual BOOL SetCallback_OnAccept(const TOnAcceptFunc& func);
	virtual BOOL SetCallback_OnStarted(const TOnStartFunc& func);
	virtual BOOL SetCallback_OnStoped(const TOnStopFunc& func);
	virtual void Dispose();

	void SoapThread();
	UINT ProcessRequestThread(struct soap* pSoap);
	int ESBOperation(struct soap *soap, std::string session, std::string inputs, std::string &results);

};
