#ifndef _INCLUDE_ESBSERIALIZER_ESBHUBSERIALIZER_H_
#define _INCLUDE_ESBSERIALIZER_ESBHUBSERIALIZER_H_

#include <string>
#include "../Common/ESBHub.h"
#include "../Common/InOut.h"


namespace ESBDataSerialzer
{
	BOOL Data2String(OUT std::wstring& string, IN const ESBCommon::ESBServiceRequest& data);
	BOOL String2Data(OUT ESBCommon::ESBServiceRequest& data, IN const std::wstring& string);

	BOOL Data2String(OUT std::wstring& string, IN const ESBCommon::ESBServiceReply& data);
	BOOL String2Data(OUT ESBCommon::ESBServiceReply& data, IN const std::wstring& string);

	BOOL Data2String(OUT std::wstring& string, IN const ESBCommon::ESBService_ReplyOK& data);
	BOOL String2Data(OUT ESBCommon::ESBService_ReplyOK& data, IN const std::wstring& string);

	BOOL Data2String(OUT std::wstring& string, IN const ESBCommon::ESBClientToken& data);
	BOOL String2Data(OUT ESBCommon::ESBClientToken& data, IN const std::wstring& string);

	BOOL Data2String(OUT std::wstring& string, IN const ESBCommon::ESBServiceToken& data);
	BOOL String2Data(OUT ESBCommon::ESBServiceToken& data, IN const std::wstring& string);

	BOOL Data2String(OUT std::wstring& string, IN const ESBCommon::ESBServiceHubSessionReply& data);
	BOOL String2Data(OUT ESBCommon::ESBServiceHubSessionReply& data, IN const std::wstring& string);

	BOOL Data2String(OUT std::wstring& string, IN const ESBCommon::ESBServiceHubSessionReply& data);
	BOOL String2Data(OUT ESBCommon::ESBServiceHubSessionReply& data, IN const std::wstring& string);

	BOOL Data2String(OUT std::wstring& string, IN const ESBCommon::ESBService_HubMethod_RegisterToHub& data);
	BOOL String2Data(OUT ESBCommon::ESBService_HubMethod_RegisterToHub& data, IN const std::wstring& string);

	BOOL Data2String(OUT std::wstring& string, IN const ESBCommon::ESBService_HubMethod_ModifySessionLimitation& data);
	BOOL String2Data(OUT ESBCommon::ESBService_HubMethod_ModifySessionLimitation& data, IN const std::wstring& string);

	BOOL Data2String(OUT std::wstring& string, IN const ESBCommon::ESBService_HubMethod_IncreaseSessionLoad& data);
	BOOL String2Data(OUT ESBCommon::ESBService_HubMethod_IncreaseSessionLoad& data, IN const std::wstring& string);

	BOOL Data2String(OUT std::wstring& string, IN const ESBCommon::ESBService_HubMethod_DecreaseSessionLoad& data);
	BOOL String2Data(OUT ESBCommon::ESBService_HubMethod_DecreaseSessionLoad& data, IN const std::wstring& string);

	BOOL Data2String(OUT std::wstring& string, IN const ESBCommon::ESBService_HubMethod_Unregister& data);
	BOOL String2Data(OUT ESBCommon::ESBService_HubMethod_Unregister& data, IN const std::wstring& string);
};


#endif //_INCLUDE_ESBSERIALIZER_ESBHUBSERIALIZER_H_