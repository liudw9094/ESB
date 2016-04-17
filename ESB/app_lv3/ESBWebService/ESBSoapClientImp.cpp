#include "StdAfx.h"
#include "ESBWebService.h"
#include "ESBSoapClientImp.h"


#define AUTO_NULL_STR(wstr) (!(wstr).empty() ? WStrToUtf8((wstr)).c_str() : NULL)


using namespace std;
using namespace ESBWebService;
using namespace ESBCommon;
using namespace Utils::SafeCoding;



CESBSoapClientImp::CESBSoapClientImp(const SAuthentication* pAuthentication /*= NULL*/)
	: m_pAuthentication(NULL)
{
	//soap_init( &m_soapClient );

#if defined(WITH_OPENSSL) || defined(WITH_GNUTLS)
	if (pAuthentication != NULL)
	{
		m_pAuthentication = new SAuthentication(*pAuthentication);
		ASSERT((0 == soap_ssl_client_context(&m_soapClient,
			/* SOAP_SSL_NO_AUTHENTICATION, */ /* for encryption w/o authentication */
											  /* SOAP_SSL_DEFAULT | SOAP_SSL_SKIP_HOST_CHECK, */	/* if we don't want the host name checks since these will change from machine to machine */
			SOAP_SSL_DEFAULT,	/* use SOAP_SSL_DEFAULT in production code */
			AUTO_NULL_STR(m_pAuthentication->keyfile), 	/* keyfile (cert+key): required only when client must authenticate to server (see SSL docs to create this file) */
			AUTO_NULL_STR(m_pAuthentication->password), 	/* password to read the keyfile */
			AUTO_NULL_STR(m_pAuthentication->cafile),		/* optional cacert file to store trusted certificates, use cacerts.pem for all public certificates issued by common CAs */
			AUTO_NULL_STR(m_pAuthentication->capath),		/* optional capath to directory with trusted certificates */
			AUTO_NULL_STR(m_pAuthentication->randomfile)	/* if randfile!=NULL: use a file with random data to seed randomness */
			)));
	}
#endif
	m_soapClient.send_timeout = 10; // 10 seconds 
	m_soapClient.recv_timeout = 10; // 10 seconds 
	m_soapClient.connect_timeout = 10;
	soap_set_mode(&m_soapClient, SOAP_C_UTFSTRING);
}

CESBSoapClientImp::~CESBSoapClientImp(void)
{
	SafeDelete(m_pAuthentication);
	m_soapClient.destroy();
}

void CESBSoapClientImp::SetURL(const std::wstring& wsURL)
{
	m_wsURL = wsURL;
}

std::wstring CESBSoapClientImp::GetURL()
{
	return m_wsURL;
}

int CESBSoapClientImp::Invoke(const std::wstring& wsSession, const std::wstring& wsInputs, std::wstring& wsResults)
{
	std::string url, session, inputs, results;
	session = WStrToUtf8(wsSession);
	inputs = WStrToUtf8(wsInputs);
	url = WStrToUtf8(m_wsURL);
	int iResult = m_soapClient.ESBOperation(url.c_str(), NULL, session, inputs, results);
	if (iResult)
	{
		if(m_soapClient.labbuf)
			wsResults = Utf8ToWStr(string(m_soapClient.labbuf));
	}
	else
		wsResults = Utf8ToWStr(results);
	return iResult;
}

void CESBSoapClientImp::Dispose()
{
	delete this;
}

ESBWEBSERVICE_API int ESBWebService::InstantClientInvoke(const std::wstring& wsURL, const std::wstring& wsSession, const std::wstring& wsInputs, std::wstring& wsResults)
{
	CESBSoapClientImp client;
	client.SetURL(wsURL);
	return client.Invoke(wsSession, wsInputs, wsResults);
}

ESBWEBSERVICE_API IESBWebServiceClient* ESBWebService::CreateESBWebServiceClient(const SAuthentication *pAuthentication/* = NULL*/)
{
	return new CESBSoapClientImp(pAuthentication);
}