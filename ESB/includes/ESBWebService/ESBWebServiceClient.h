#ifndef _INCLUDE_ESBWEBSERVICE_ESBWEBSERVICECLIENT_H_
#define _INCLUDE_ESBWEBSERVICE_ESBWEBSERVICECLIENT_H_

#include <string>
#include "../Utils/SafeCoding/IDisposable.h"
#include "Authenticate.h"

#ifndef ESBWEBSERVICE_API
#define ESBWEBSERVICE_API
#endif

struct soap;

namespace ESBWebService
{
	class IESBWebServiceClient : public Utils::SafeCoding::IDisposable
	{
	public:
		virtual void SetURL(const std::wstring& wsURL) = 0;
		virtual std::wstring GetURL() = 0;
		virtual int Invoke(const std::wstring& wsSession, const std::wstring& wsInputs, std::wstring& wsResults) = 0;
	};
	ESBWEBSERVICE_API int InstantClientInvoke(const std::wstring& wsURL, const std::wstring& wsSession, const std::wstring& wsInputs, std::wstring& wsResults);

	ESBWEBSERVICE_API IESBWebServiceClient* CreateESBWebServiceClient(const SAuthentication *pAuthentication = NULL);
};

#endif //_INCLUDE_ESBWEBSERVICE_ESBWEBSERVICECLIENT_H_
