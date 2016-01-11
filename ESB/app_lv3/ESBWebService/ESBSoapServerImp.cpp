#include "StdAfx.h"
#include "ESBWebService.h"
#include "ESBSoapServerImp.h"
#include "ESBServiceSoap.nsmap"

using namespace Utils::Thread;
using namespace ESBWebService;
using namespace std;

CESBSoapServerImp::CESBSoapServerImp(void) :
	m_bIsStarted(FALSE),
	m_bExitThread(FALSE),
	m_plkMapAcceptSoap(CreateCriticalSection()),
	m_nPort(-1)
{
	m_soap = soap_new();
	m_funcInvoke = [](SREF(Utils::Thread::IThread),
						struct soap* psoap,
						const std::wstring& wsSession,
						const std::wstring& wsInputs,
						std::wstring& wsResults) -> int
	{
		return 0;
	};
	
	m_funcAccept = [](const struct soap* sSoap)->BOOL
	{
		return TRUE;
	};
}

CESBSoapServerImp::~CESBSoapServerImp(void)
{
	soap_free(m_soap);
}

BOOL CESBSoapServerImp::Start(int iPort)
{
	if (m_bIsStarted)
		Stop();

	soap_init(m_soap);
	m_soap->send_timeout = 10; // 10 seconds 
	m_soap->recv_timeout = 10; // 10 seconds 
	//m_soap.accept_timeout = 3600; // server stops after 1 hour of inactivity 
	m_soap->max_keep_alive = 100; // max keep-alive sequence 
	m_soap->user = this;

	soap_set_mode(m_soap, SOAP_C_UTFSTRING);

	char* pURI = NULL;
	if (!!soap_valid_socket(this->m_soap->master) && !soap_valid_socket(soap_bind(m_soap, NULL, iPort, 100)))
	{
		soap_print_fault(m_soap, stderr);
		//soap_print_fault(&m_soap, stderr);

		return FALSE;
	}
	else
	{
		m_thdSoap = CreateThread();
		m_thdSoap->AsynInvoke([this]() {SoapThread();});
		::InterlockedExchange(reinterpret_cast<volatile LONG*>(&m_bIsStarted), TRUE);
		m_nPort = iPort;
		return TRUE;
	}
}

BOOL CESBSoapServerImp::Stop()
{
	if (!m_bIsStarted)
		return FALSE;

	::InterlockedExchange(reinterpret_cast<volatile LONG*>(&m_bExitThread), TRUE);
	// Dispose the main soap thread object
	m_thdSoap = NULL;

	//soap_destroy(m_soap);
	soap_end(m_soap);
	soap_done(m_soap);

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
				}
			}
			m_mapAcceptSoap.clear();
		}
	}

	::InterlockedExchange(reinterpret_cast<volatile LONG*>(&m_bIsStarted), FALSE);
	m_nPort = -1;
	return TRUE;
}

BOOL CESBSoapServerImp::IsStarted() const
{
	return m_bIsStarted;
}

int CESBSoapServerImp::GetPort() const
{
	return m_nPort;
}

wstring&& CESBSoapServerImp::GetClientIP(const struct soap* pSoap) const
{
	wchar_t buf[256];
	swprintf_s(buf, L"%d.%d.%d.%d:%d",
		(pSoap->ip >> 24) & 0xFF,
		(pSoap->ip >> 16) & 0xFF,
		(pSoap->ip >> 8) & 0xFF,
		pSoap->ip & 0xFF,
		pSoap->port);
	wstring str(buf);
	return move(str);
}

BOOL CESBSoapServerImp::SetCallback_Invoke(const IESBWebServiceServer::TInvokeFunc& func)
{
	if (!IsStarted())
	{
		m_funcInvoke = func;
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CESBSoapServerImp::SetCallback_Accept(const IESBWebServiceServer::TAcceptFunc& func)
{
	if (!IsStarted())
	{
		m_funcAccept = func;
		return TRUE;
	}
	else
		return FALSE;
}

void CESBSoapServerImp::Dispose()
{
	delete this;
}


UINT CESBSoapServerImp::ProcessRequestThread(struct soap* pSoap)
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

void CESBSoapServerImp::SoapThread()
{
	struct soap* pCloneSoap = NULL;
	while (!m_bExitThread)
	{
		pCloneSoap = soap_copy(m_soap); // make a safe copy

		int s = (int)soap_accept(pCloneSoap); // master and slave sockets

		if (s < 0)
		{
			//soap_print_fault(pCloneSoap, stderr);
			soap_print_fault(m_soap, stderr);
			break;
		}

		if (m_funcAccept)
		{
			if (!m_funcAccept(pCloneSoap))
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



int CESBSoapServerImp::ESBOperation(struct soap *psoap, std::string session, std::string inputs, std::string &results)
{
	if (!m_funcInvoke)
		return 0;

	wstring_convert<codecvt_utf8_utf16<wchar_t>, wchar_t> convUTF8UTF16;
	wstring wssession, wsinputs, wsresults;
	wssession = convUTF8UTF16.from_bytes(session);
	wsinputs = convUTF8UTF16.from_bytes(inputs);
	SREF(IThread) pthread = m_mapAcceptSoap[psoap];
	int hr = m_funcInvoke(pthread, psoap, wssession, wsinputs, wsresults);
	results = convUTF8UTF16.to_bytes(wsresults);

	return hr;
}

int __ns1__ESBOperation(struct soap *psoap, std::string session, std::string inputs, std::string &results)
{
	CESBSoapServerImp* pClass = (CESBSoapServerImp*)psoap->user;
	return pClass->ESBOperation(psoap, session, inputs, results);
}

ESBWEBSERVICE_API IESBWebServiceServer* ESBWebService::CreateESBWebServiceServer()
{
	return new CESBSoapServerImp();
}