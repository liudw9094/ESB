#ifndef _INCLUDE_ESBSERIALIZER_GUIDSERIALIZER_H_
#define _INCLUDE_ESBSERIALIZER_GUIDSERIALIZER_H_

#include <vector>
#include <chrono>
#include "../Common/InOut.h"

namespace ESBDataSerialzer
{
	BOOL Data2String(OUT std::wstring& string, IN const GUID& data);
	BOOL String2Data(OUT GUID& data, IN const std::wstring& string);
	
	BOOL Data2String(OUT std::wstring& string, IN const std::vector<BYTE>& data);
	BOOL Data2String(OUT std::wstring& string, IN const void *data, IN size_t len);
	BOOL String2Data(OUT std::vector<BYTE>& data, IN const std::wstring& string);

	BOOL Data2String(OUT std::wstring& string, IN int data);
	BOOL String2Data(OUT int& data, IN const std::wstring& string);

	BOOL Data2String(OUT std::wstring& string, IN unsigned int data);
	BOOL String2Data(OUT unsigned int& data, IN const std::wstring& string);

	BOOL Data2String(OUT std::wstring& string, IN long data);
	BOOL String2Data(OUT long& data, IN const std::wstring& string);

	BOOL Data2String(OUT std::wstring& string, IN unsigned long data);
	BOOL String2Data(OUT unsigned long& data, IN const std::wstring& string);

	BOOL Data2String(OUT std::wstring& string, IN long long data);
	BOOL String2Data(OUT long long& data, IN const std::wstring& string);

	BOOL Data2String(OUT std::wstring& string, IN unsigned long long data);
	BOOL String2Data(OUT unsigned long long& data, IN const std::wstring& string);

	BOOL Data2String(OUT std::wstring& string, IN double data);
	BOOL String2Data(OUT double& data, IN const std::wstring& string);

	BOOL Data2String(OUT std::wstring& string, IN float data);
	BOOL String2Data(OUT float& data, IN const std::wstring& string);

	BOOL Data2String(OUT std::wstring& string, IN const std::chrono::steady_clock::time_point& data);
	BOOL String2Data(OUT std::chrono::steady_clock::time_point& data, IN const std::wstring& string);

};


#endif //_INCLUDE_ESBSERIALIZER_GUIDSERIALIZER_H_