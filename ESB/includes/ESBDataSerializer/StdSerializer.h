#ifndef _INCLUDE_ESBSERIALIZER_GUIDSERIALIZER_H_
#define _INCLUDE_ESBSERIALIZER_GUIDSERIALIZER_H_

#include <vector>
#include "../Common/InOut.h"

namespace ESBDataSerialzer
{
	BOOL Data2String(OUT std::wstring& string, IN const GUID& data);
	BOOL String2Data(OUT GUID& data, IN const std::wstring& string);
	
	BOOL Data2String(OUT std::wstring& string, IN const time_t& data);
	BOOL String2Data(OUT time_t& data, IN const std::wstring& string);

	BOOL Data2String(OUT std::wstring& string, IN const std::vector<char>& data);
	BOOL String2Data(OUT std::vector<char>& data, IN const std::wstring& string);

	BOOL Data2String(OUT std::wstring& string, IN int data);
	BOOL String2Data(OUT int& data, IN const std::wstring& string);

	BOOL Data2String(OUT std::wstring& string, IN unsigned int data);
	BOOL String2Data(OUT unsigned int& data, IN const std::wstring& string);

	BOOL Data2String(OUT std::wstring& string, IN long data);
	BOOL String2Data(OUT long& data, IN const std::wstring& string);

	BOOL Data2String(OUT std::wstring& string, IN unsigned long data);
	BOOL String2Data(OUT unsigned long& data, IN const std::wstring& string);

	BOOL Data2String(OUT std::wstring& string, IN double data);
	BOOL String2Data(OUT double& data, IN const std::wstring& string);

	BOOL Data2String(OUT std::wstring& string, IN float data);
	BOOL String2Data(OUT float& data, IN const std::wstring& string);
};


#endif //_INCLUDE_ESBSERIALIZER_GUIDSERIALIZER_H_