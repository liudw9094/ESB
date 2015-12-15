#ifndef _INCLUDE_ESBSERIALIZER_ESBHUBSERIALIZER_H_
#define _INCLUDE_ESBSERIALIZER_ESBHUBSERIALIZER_H_

#include <string>
#include "../Common/ESBHub.h"

namespace ESBDataSerialzer
{
	BOOL Data2String(IN const ESBCommon::ESBClientToken& data, OUT std::wstring& string);
	BOOL String2Data(IN const std::wstring& string, OUT ESBCommon::ESBClientToken& data);

	BOOL Data2String(IN const ESBCommon::ESBServiceToken& data, OUT std::wstring& string);
	BOOL String2Data(IN const std::wstring& string, OUT ESBCommon::ESBServiceToken& data);

	BOOL Data2String(IN const ESBCommon::ESBServiceHubSession& data, OUT std::wstring& string);
	BOOL String2Data(IN const std::wstring& string, OUT ESBCommon::ESBServiceHubSession& data);

	BOOL Data2String(IN const ESBCommon::ESBService_RegisterToHub& data, OUT std::wstring& string);
	BOOL String2Data(IN const std::wstring& string, OUT ESBCommon::ESBService_RegisterToHub& data);
};


#endif //_INCLUDE_ESBSERIALIZER_ESBHUBSERIALIZER_H_