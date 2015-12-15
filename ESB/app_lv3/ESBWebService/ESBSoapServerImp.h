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
	volatile BOOL	m_bIsIni;
	int m_nPort;

	SREF(Utils::Thread::IThread) m_thdSoap;
	volatile BOOL	m_bExitThread;

	mutable SREF(Utils::Thread::ICriticalSection) m_plkMapAcceptSoap;
	std::map<struct soap*, SREF(Utils::Thread::IThread)>m_mapAcceptSoap;
	std::function<int(SREF(Utils::Thread::IThread) pthread,
					struct soap* psoap,
					const std::wstring& wsSession,
					const std::wstring& wsInputs,
					std::wstring& wsResults)> m_funcInvoke;
	std::function<BOOL(const struct soap* sSoap)> m_funcAccept;
public:
	CESBSoapServerImp(void);
	~CESBSoapServerImp(void);

public:
	virtual BOOL Start(int nPort);
	virtual BOOL End();
	virtual BOOL IsStarted() const;
	virtual int GetPort() const;
	virtual std::wstring GetClientIP(struct soap* pSoap);
	virtual BOOL SetEvent_Invoke(const std::function<
										int(SREF(Utils::Thread::IThread) pthread,
										struct soap* psoap,
										const std::wstring& wsSession,
										const std::wstring& wsInputs,
										std::wstring& wsResults)>& func
								);
	virtual BOOL SetEvent_Accept(const std::function<BOOL(const struct soap* sSoap)>& func);
	virtual void Dispose();

	void SoapThread();
	UINT ProcessRequestThread(struct soap* pSoap);
	int ESBOperation(struct soap *soap, std::string session, std::string inputs, std::string &results);

};
