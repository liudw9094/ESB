#ifndef _INCLUDE_ESBSERIALIZER_ESBHUBSERIALIZER_H_
#define _INCLUDE_ESBSERIALIZER_ESBHUBSERIALIZER_H_

#include <string>
#include "../Common/ESBHub.h"
#include "../Common/InOut.h"


namespace ESBDataSerialzer
{
	BOOL Data2String(OUT std::wstring& string, IN const ESBCommon::ESBHeartBeat& data);
	BOOL String2Data(OUT ESBCommon::ESBHeartBeat& data, IN const std::wstring& string);

	BOOL Data2String(OUT std::wstring& string, IN const ESBCommon::ENUM_IDTYPE& data);
	BOOL String2Data(OUT ESBCommon::ENUM_IDTYPE& data, IN const std::wstring& string);

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

	BOOL Data2String(OUT std::wstring& string, IN const ESBCommon::ESBServiceSessionReply& data);
	BOOL String2Data(OUT ESBCommon::ESBServiceSessionReply& data, IN const std::wstring& string);

	BOOL Data2String(OUT std::wstring& string, IN const ESBCommon::ESBServiceSessionReply& data);
	BOOL String2Data(OUT ESBCommon::ESBServiceSessionReply& data, IN const std::wstring& string);

	BOOL Data2String(OUT std::wstring& string, IN const ESBCommon::ESBService_HubMethod_RegisterToHub& data);
	BOOL String2Data(OUT ESBCommon::ESBService_HubMethod_RegisterToHub& data, IN const std::wstring& string);

	BOOL Data2String(OUT std::wstring& string, IN const ESBCommon::ESBService_HubMethod_UpdateLoadState& data);
	BOOL String2Data(OUT ESBCommon::ESBService_HubMethod_UpdateLoadState& data, IN const std::wstring& string);

	BOOL Data2String(OUT std::wstring& string, IN const ESBCommon::ESBService_HubMethod_Unregister& data);
	BOOL String2Data(OUT ESBCommon::ESBService_HubMethod_Unregister& data, IN const std::wstring& string);

	BOOL Data2String(OUT std::wstring& string, IN const ESBCommon::ESBService_HubMethod_StartSession& data);
	BOOL String2Data(OUT ESBCommon::ESBService_HubMethod_StartSession& data, IN const std::wstring& string);

	BOOL Data2String(OUT std::wstring& string, IN const ESBCommon::ESBService_HubMethod_ClientSessionEnd& data);
	BOOL String2Data(OUT ESBCommon::ESBService_HubMethod_ClientSessionEnd& data, IN const std::wstring& string);

	BOOL Data2String(OUT std::wstring& string, IN const ESBCommon::ESBService_ServiceMethod_SessionConfirm& data);
	BOOL String2Data(OUT ESBCommon::ESBService_ServiceMethod_SessionConfirm& data, IN const std::wstring& string);

	BOOL Data2String(OUT std::wstring& string, IN const ESBCommon::ESBService_ServiceMethod_EndSession& data);
	BOOL String2Data(OUT ESBCommon::ESBService_ServiceMethod_EndSession& data, IN const std::wstring& string);

	BOOL Data2String(OUT std::wstring& string, IN const ESBCommon::ESBService_ServiceMethod_ClientRequest& data);
	BOOL String2Data(OUT ESBCommon::ESBService_ServiceMethod_ClientRequest& data, IN const std::wstring& string);

	BOOL Data2String(OUT std::wstring& string, IN const ESBCommon::ESBService_ServiceReply_LoadStateUpdate& data);
	BOOL String2Data(OUT ESBCommon::ESBService_ServiceReply_LoadStateUpdate& data, IN const std::wstring& string);
};


#endif //_INCLUDE_ESBSERIALIZER_ESBHUBSERIALIZER_H_