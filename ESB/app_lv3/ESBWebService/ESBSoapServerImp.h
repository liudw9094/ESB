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

	SREF(Utils::Thread::IThread) m_thdSoap;
	volatile BOOL	m_bExitThread;

	mutable SREF(Utils::Thread::ICriticalSection) m_plkMapAcceptSoap;
	std::map<struct soap*, SREF(Utils::Thread::IThread)>m_mapAcceptSoap;
	TInvokeFunc m_funcInvoke;
	TAcceptFunc m_funcAccept;
public:
	CESBSoapServerImp(void);
	~CESBSoapServerImp(void);

public:
	virtual BOOL Start(int nPort);
	virtual BOOL Stop();
	virtual BOOL IsStarted() const;
	virtual int GetPort() const;
	virtual std::wstring&& GetClientIP(const struct soap* pSoap) const;
	virtual BOOL SetCallback_Invoke(const TInvokeFunc& func);
	virtual BOOL SetCallback_Accept(const TAcceptFunc& func);
	virtual void Dispose();

	void SoapThread();
	UINT ProcessRequestThread(struct soap* pSoap);
	int ESBOperation(struct soap *soap, std::string session, std::string inputs, std::string &results);

};
