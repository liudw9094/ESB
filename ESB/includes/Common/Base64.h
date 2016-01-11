#ifndef _INCLUDE_ESBSERIALIZER_BASE64_H_
#define _INCLUDE_ESBSERIALIZER_BASE64_H_

#include <string>
#include <vector>

namespace ESBCommon
{
	std::string&& DataToBase64(const void* data, size_t len);
	std::string&& DataToBase64(const std::vector<BYTE>& data);
	std::vector<BYTE>&& Base64ToData(const char* szBase64);
	std::vector<BYTE>&& Base64ToData(const std::string& szBase64);


	std::wstring&& DataToBase64W(const void* data, size_t len);
	std::wstring&& DataToBase64W(const std::vector<BYTE>& data);
	std::vector<BYTE>&& Base64ToDataW(const wchar_t* szBase64);
	std::vector<BYTE>&& Base64ToDataW(const std::wstring& szBase64);
};

#endif //_INCLUDE_ESBSERIALIZER_BASE64_H_