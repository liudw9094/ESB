#ifndef _INCLUDE_ESBWEBSERVICE_AUTHENTICATE_H_
#define _INCLUDE_ESBWEBSERVICE_AUTHENTICATE_H_

#include <string.h>

namespace ESBWebService
{
	struct SAuthentication
	{
		std::wstring keyfile;
		std::wstring password;
		std::wstring cafile;
		std::wstring capath;
		std::wstring dhfile;
		std::wstring randomfile;
		std::wstring sid;
	};
};

#endif //_INCLUDE_ESBWEBSERVICE_AUTHENTICATE_H_