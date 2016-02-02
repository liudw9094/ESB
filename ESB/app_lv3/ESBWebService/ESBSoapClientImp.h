#pragma once

#include <ESBWebService/ESBWebServiceClient.h>
#include "soapESBServiceSoapProxy.h"

class CESBSoapClientImp : public ESBWebService::IESBWebServiceClient
{
private:
	ESBServiceSoapProxy m_soapClient;
	std::wstring m_wsURL;
public:
	CESBSoapClientImp(void);
	~CESBSoapClientImp(void);

	virtual void SetURL(const std::wstring& wsURL);
	virtual std::wstring GetURL();
	virtual int Invoke(const std::wstring& wsSession, const std::wstring& wsInputs, std::wstring& wsResults);
	virtual void Dispose();
};

