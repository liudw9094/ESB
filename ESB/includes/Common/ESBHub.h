#ifndef _INCLUDE_COMMON_ESBHUBTOKEN_H_
#define _INCLUDE_COMMON_ESBHUBTOKEN_H_

#include <string>

namespace ESBCommon
{
	struct ESBServiceRequest
	{
		std::wstring contents;
		static struct _TAGNAME
		{
			const wchar_t * const ROOTNAME = L"ESBServiceRequest";
		} NAMES;
	};
	

	struct ESBServiceReply
	{
		std::wstring contents;
		static struct _TAGNAME
		{
			const wchar_t * const ROOTNAME = L"ESBServiceReply";
		} NAMES;
	};

	struct ESBService_ReplyOK
	{
		static struct _TAGNAME
		{
			const wchar_t * const ROOTNAME = L"ESBService_ReplyOK";
		} NAMES;
	};

	struct ESBServiceToken
	{
		std::wstring	wsClientSession;
		time_t			timeStamp;
		time_t			timeReplyDeadLine;

		static struct _TAGNAME
		{
			const wchar_t * const ROOTNAME = L"ESBServiceToken";
			const wchar_t * const WSCLIENTSESSION = L"wsClientSession";
			const wchar_t * const TIMESTAMP = L"timeStamp";
			const wchar_t * const TIMEREPLYDEADLINE = L"timeReplyDeadLine";
		} NAMES;
	};

	struct ESBClientToken
	{
		std::wstring	wsClientSession;
		time_t			timeStamp;
		time_t			timeReplyDeadLine;
		std::wstring	wsURLRedirection;

		static struct _TAGNAME
		{
			const wchar_t * const ROOTNAME = L"ESBClientToken";
			const wchar_t * const WSCLIENTSESSION = L"wsClientSession";
			const wchar_t * const TIMESTAMP = L"timeStamp";
			const wchar_t * const TIMEREPLYDEADLINE = L"timeReplyDeadLine";
			const wchar_t * const WSURLREDIRECTION = L"wsURLRedirection";
		} NAMES;
	};

	struct ESBServiceHubSessionReply
	{
		std::wstring	wsServiceSession;

		static struct _TAGNAME
		{
			const wchar_t * const ROOTNAME = L"ESBServiceHubSessionReply";
			const wchar_t * const WSSERVICESESSION = L"wsServiceSession";
		} NAMES;
	};

	struct ESBService_HubMethod_RegisterToHub
	{
		std::wstring wsServiceURL;
		GUID guidService;
		std::wstring wsServiceName;
		UINT maximumSession;

		ESBService_HubMethod_RegisterToHub() : guidService({ 0 }), maximumSession(0)
		{
		}

		static struct _TAGNAME
		{
			const wchar_t * const ROOTNAME = L"ESBService_HubMethod_RegisterToHub";
			const wchar_t * const WSSERVICEURL = L"wsServiceURL";
			const wchar_t * const GUIDSERVICE = L"guidService";
			const wchar_t * const WSSERVICENAME = L"wsServiceName";
			const wchar_t * const MAXIMUMSESSION = L"maximumSession";
		} NAMES;
	};

	struct ESBService_HubMethod_ModifySessionLimitation
	{
		UINT maximumSession;
		static struct _TAGNAME
		{
			const wchar_t * const ROOTNAME = L"ESBService_HubMethod_ModifySessionLimitation";
			const wchar_t * const MAXIMUMSESSION = L"maximumSession";
		} NAMES;
	};

	struct ESBService_HubMethod_IncreaseSessionLoad
	{
		static struct _TAGNAME
		{
			const wchar_t * const ROOTNAME = L"ESBService_HubMethod_IncreaseSessionLoad";
		} NAMES;
	};

	struct ESBService_HubMethod_DecreaseSessionLoad
	{
		static struct _TAGNAME
		{
			const wchar_t * const ROOTNAME = L"ESBService_HubMethod_DecreaseSessionLoad";
		} NAMES;
	};

	struct ESBService_HubMethod_Unregister
	{
		static struct _TAGNAME
		{
			const wchar_t * const ROOTNAME = L"ESBService_HubMethod_Unregister";
		} NAMES;
	};
};

#endif // _INCLUDE_COMMON_ESBHUBTOKEN_H_