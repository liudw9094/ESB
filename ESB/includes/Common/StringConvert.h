#ifndef _INCLUDE_ESBSERIALIZER_STRINGCONVERT_H_
#define _INCLUDE_ESBSERIALIZER_STRINGCONVERT_H_

#include <string>

namespace ESBCommon
{
	std::string WStrToUtf8(const std::wstring& wstr);
	std::wstring Utf8ToWStr(const std::string& str);
};

#endif //_INCLUDE_ESBSERIALIZER_STRINGCONVERT_H_