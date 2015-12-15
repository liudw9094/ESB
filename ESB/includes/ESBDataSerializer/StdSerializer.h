#ifndef _INCLUDE_ESBSERIALIZER_GUIDSERIALIZER_H_
#define _INCLUDE_ESBSERIALIZER_GUIDSERIALIZER_H_


namespace ESBDataSerialzer
{
	BOOL Data2String(const GUID& data, std::wstring& string);
	BOOL String2Data(const std::wstring& string, GUID& data);
	
	BOOL Data2String(const time_t& data, std::wstring& string);
	BOOL String2Data(const std::wstring& string, time_t& data);
};


#endif //_INCLUDE_ESBSERIALIZER_GUIDSERIALIZER_H_