#pragma once

#include <map>
#include <string>
#include <Utils/Thread.h>

struct soap;

#include "soapH.h"

class CSoapServerTool_Callback
{
public:
	virtual int Invoke(SREF(Utils::Thread::IThread) pthread, struct soap* psoap, const std::wstring& wsSession, const std::wstring& wsInputs, std::wstring& wsResults) =0;
	virtual BOOL CheckClientStatue(const struct soap& sSoap) =0;
};

class CSoapServerTool
{
public:
	CSoapServerTool(void);
	~CSoapServerTool(void);

public:
	BOOL Start(int iPort);
	BOOL End();
	BOOL IsStarted() { return m_bIsIni;	}

	void SoapThread();

	UINT ProcessRequestThread(struct soap* pSoap);
	std::wstring getClientIP(struct soap* pSoap);

	void setCallback(CSoapServerTool_Callback* pCallback);
	//int ToIngestInvoke(struct soap* pSoap, _ns1__ToIngestInvokeHead *pHead, _ns1__ToIngestInvokeData *pData, _ns1__ToIngestInvokeResponse *pResponse);

	int ESBOperation(struct soap *soap, std::string session, std::string inputs, std::string &results);
protected:
	soap *soap;
	volatile BOOL	m_bIsIni;

	SREF(Utils::Thread::IThread) m_thdSoap;
	volatile BOOL	m_bExitThread;

	mutable SREF(Utils::Thread::ICriticalSection) m_plkMapAcceptSoap;
	std::map<struct soap*, SREF(Utils::Thread::IThread)>m_mapAcceptSoap;
	CSoapServerTool_Callback* m_pCallback;
};
