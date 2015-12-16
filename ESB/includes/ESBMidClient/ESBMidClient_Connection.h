#ifndef _INCLUDE_ESBMIDCLIENT_CONNECTION_H_
#define _INCLUDE_ESBMIDCLIENT_CONNECTION_H_

#include <string>
#include "../Utils/SafeCoding/IDisposable.h"

#ifndef ESBMIDCONN_API
#define ESBMIDCONN_API
#endif

namespace ESBMidClient
{
	class IESBConnection : public Utils::SafeCoding::IDisposable
	{
	public:
		virtual int StartSession(const std::wstring& wsURL, const GUID& guidService) = 0;
		virtual int EndSession(void) = 0;
		virtual BOOL IsSessionValid(void) const = 0;
		virtual int Send(const std::wstring& wsContent, std::wstring& wsResult) = 0;
		virtual std::wstring&& GetInitialURL() = 0;
		virtual std::wstring&& GetCurrentURL() = 0;
		virtual GUID&& GetServiceGUID() = 0;
	};

	ESBMIDCONN_API IESBConnection* CreateESBConnection();
};

#endif //_INCLUDE_ESBMIDCLIENT_CONNECTION_H_