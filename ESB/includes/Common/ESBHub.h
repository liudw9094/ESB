#ifndef _INCLUDE_COMMON_ESBHUBTOKEN_H_
#define _INCLUDE_COMMON_ESBHUBTOKEN_H_

#include <string>

namespace ESBCommon
{


	struct ESBServiceToken
	{
		std::wstring	wsClientSession;
		time_t			timeStamp;
		time_t			timeReplyDeadLine;
	};

	struct ESBClientToken : public ESBServiceToken
	{
		std::wstring	wsURLRedirection;
	};

	struct ESBServiceHubSession
	{
		std::wstring	wsServiceSession;
	};

	struct ESBService_RegisterToHub
	{
		std::wstring wsServiceURL;
		GUID guidService;
		std::wstring wsServiceName;
		UINT maximumSession;

		ESBService_RegisterToHub() : guidService({ 0 }), maximumSession(0)
		{
		}
	};
};

#endif // _INCLUDE_COMMON_ESBHUBTOKEN_H_