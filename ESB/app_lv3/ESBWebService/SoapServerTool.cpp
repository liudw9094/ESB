#include "StdAfx.h"
#include "SoapServerTool.h"
#include "ESBServiceSoap.nsmap"

using namespace Utils::Thread;
using namespace std;

CSoapServerTool::CSoapServerTool(void) :
	m_bIsIni(FALSE),
	m_bExitThread(FALSE),
	m_plkMapAcceptSoap(CreateCriticalSection())
{
	soap = soap_new();
}

CSoapServerTool::~CSoapServerTool(void)
{
	soap_destroy(soap);
}

BOOL CSoapServerTool::Start(int iPort)
{
	if (m_bIsIni)
		End();

	soap_init(soap);
	soap->send_timeout = 10; // 10 seconds 
	soap->recv_timeout = 10; // 10 seconds 
	//m_soap.accept_timeout = 3600; // server stops after 1 hour of inactivity 
	soap->max_keep_alive = 100; // max keep-alive sequence 
	soap->user = this;

	soap_set_mode(soap, SOAP_C_UTFSTRING);

	char* pURI = NULL;
	if (!!soap_valid_socket(this->soap->master) && !soap_valid_socket(soap_bind(soap, NULL, iPort, 100)))
	{
		soap_print_fault(soap, stderr);
		//soap_print_fault(&m_soap, stderr);

		return FALSE;
	}
	else
	{
		m_thdSoap = CreateThread();
		m_thdSoap->AsynInvoke([this]() {SoapThread();});
		m_bIsIni = TRUE;
	}

	return TRUE;
}

BOOL CSoapServerTool::End()
{
	if (!m_bIsIni)
		return FALSE;

	::InterlockedExchange(reinterpret_cast<volatile LONG*>(&m_bExitThread), TRUE);

	//soap_destroy(soap);
	soap_end(soap);
	soap_done(soap);

	{
		SLOCK(m_plkMapAcceptSoap);
		if (!m_mapAcceptSoap.empty())
		{
			for (auto p = m_mapAcceptSoap.begin(); p != m_mapAcceptSoap.end(); ++p)
			{
				struct soap* pSoap = p->first;
				SREF(IThread);
				if (pSoap)
				{
					soap_destroy(pSoap); // dealloc C++ data
					soap_end(pSoap); // dealloc data and clean up
					soap_done(pSoap); // detach soap struct
					soap_free(pSoap);
				}
			}
			m_mapAcceptSoap.clear();
		}
	}
	// Dispose the main soap thread object
	m_thdSoap = NULL;

	m_bIsIni = FALSE;
	return TRUE;
}

UINT CSoapServerTool::ProcessRequestThread(struct soap* pSoap)
{
	ASSERT(pSoap);

	soap_serve(pSoap);

	soap_destroy(pSoap); // dealloc C++ data
	soap_end(pSoap); // dealloc data and clean up
	soap_done(pSoap); // detach soap struct
	soap_free(pSoap);

	{
		SLOCK(m_plkMapAcceptSoap);
		m_mapAcceptSoap.erase(pSoap);
	}

	return 0;
}

void CSoapServerTool::SoapThread()
{
	struct soap* pCloneSoap = NULL;
	while (!m_bExitThread)
	{
		pCloneSoap = soap_copy(soap); // make a safe copy

		int s = (int)soap_accept(pCloneSoap); // master and slave sockets

		if (s < 0)
		{
			//soap_print_fault(pCloneSoap, stderr);
			soap_print_fault(soap, stderr);
			break;
		}

		if (m_pCallback)
		{
			if (!m_pCallback->CheckClientStatue(*pCloneSoap))
				continue;
		}

		SREF(IThread) threadRequest = CreateThread();
		{
			SLOCK(m_plkMapAcceptSoap);
			m_mapAcceptSoap[pCloneSoap] = threadRequest;
		}
		threadRequest->AsynInvoke([this, threadRequest, pCloneSoap]() { ProcessRequestThread(pCloneSoap); });
		pCloneSoap = NULL;
	}

	if (pCloneSoap)
	{
		soap_destroy(pCloneSoap); // dealloc C++ data
		soap_end(pCloneSoap); // dealloc data and clean up
		soap_done(pCloneSoap); // detach soap struct
		soap_free(pCloneSoap);
	}
}

SOAP_FMAC5 int SOAP_FMAC6 ns1__callback(struct soap*, char *_strInput, char *&_strOutput)
{
	return 0;
}

wstring CSoapServerTool::getClientIP(struct soap* pSoap)
{
	/*
	string str;

	str.Format(_T("%d.%d.%d.%d:%d"),
		(sSoap.ip >> 24)&0xFF, (sSoap.ip >> 16)&0xFF, (sSoap.ip >> 8)&0xFF, sSoap.ip&0xFF,
		sSoap.port);
	return str;
	*/
	return L"";
}

void CSoapServerTool::setCallback(CSoapServerTool_Callback* pCallback)
{
	m_pCallback = pCallback;
}

int CSoapServerTool::ESBOperation(struct soap *soap, std::string session, std::string inputs, std::string &results)
{
	//ASSERT(pSoap && pHead && pData && pResponse);

	//if (!pHead->strXmlHead)
	//	return -1;
	//CString strHead = Utf8ToUnicode(pHead->strXmlHead);

	//if (!pData->strData)
	//	return -1;
	//CString strData = Utf8ToUnicode(pData->strData);

	//CString strOut;
	if (m_pCallback == NULL)
		return 0;
	SREF(IThread) pthread = m_mapAcceptSoap[soap];
	wstring wssession, wsinputs, wsresults;
	int hr = m_pCallback->Invoke(pthread, soap, wssession, wsinputs, wsresults);
	//CStringA strUtf8Out = UnicodeToUtf8(strOut);
	//pResponse->ToIngestInvokeResult = CSoapServerTool::malloc_charA(pSoap, strUtf8Out.GetBuffer());
	//pResponse->soap = pSoap;

	return 0;
}

int __ns1__ESBOperation(struct soap *soap, std::string session, std::string inputs, std::string &results)
{
	CSoapServerTool* pClass = (CSoapServerTool*)soap->user;
	return pClass->ESBOperation(soap, session, inputs, results);
}