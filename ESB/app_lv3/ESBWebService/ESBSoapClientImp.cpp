#include "StdAfx.h"
#include "ESBWebService.h"
#include "ESBSoapClientImp.h"



using namespace std;
using namespace ESBWebService;

CESBSoapClientImp::CESBSoapClientImp(void)
{
	//soap_init( &soapClient );
	soap_set_mode(&m_soapClient, SOAP_C_UTFSTRING);
}

CESBSoapClientImp::~CESBSoapClientImp(void)
{
}

void CESBSoapClientImp::SetURL(const std::wstring& wsURL)
{
	std::string url;
	wstring_convert<codecvt_utf8_utf16<wchar_t>, wchar_t> convUTF8UTF16;
	m_wsURL = wsURL;
	url = convUTF8UTF16.to_bytes(wsURL);
	strcpy_s(m_soapClient.endpoint, url.c_str());
}

std::wstring&& CESBSoapClientImp::GetURL()
{
	return move(m_wsURL);
}

int CESBSoapClientImp::Invoke(const std::wstring& wsSession, const std::wstring& wsInputs, std::wstring& wsResults)
{
	wstring_convert<codecvt_utf8_utf16<wchar_t>, wchar_t> convUTF8UTF16;
	std::string url, session, inputs, results;
	session = convUTF8UTF16.to_bytes(wsSession);
	inputs = convUTF8UTF16.to_bytes(wsInputs);

	int iResult = m_soapClient.ESBOperation(session, inputs, results);
	wsResults = convUTF8UTF16.from_bytes(results);
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

ESBWEBSERVICE_API IESBWebServiceClient* ESBWebService::CreateESBWebServiceClient()
{
	return new CESBSoapClientImp;
}