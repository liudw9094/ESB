#include "stdafx.h"

using namespace std;

std::string ESBCommon::WStrToUtf8(const std::wstring& wstr)
{
	wstring_convert<codecvt_utf8_utf16<wchar_t>, wchar_t> convUTF8UTF16;
	return convUTF8UTF16.to_bytes(wstr);
}

std::wstring ESBCommon::Utf8ToWStr(const std::string& str)
{
	wstring_convert<codecvt_utf8_utf16<wchar_t>, wchar_t> convUTF8UTF16;
	return convUTF8UTF16.from_bytes(str);
}