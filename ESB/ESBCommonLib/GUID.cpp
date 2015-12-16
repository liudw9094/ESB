#include "stdafx.h"
#include <string.h>
#include <stdio.h>
using namespace std;

std::wstring&& ESBCommon::GUID2String(const GUID& guid, BOOL bIncludeLine /*= TRUE*/)
{
	wchar_t szGuid[50];
	memset(szGuid, '\0', 50);
	if (bIncludeLine)
	{
		swprintf_s(szGuid, L"%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X", guid.Data1, guid.Data2, guid.Data3
			, guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3]
			, guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
	}
	else
	{
		swprintf_s(szGuid, L"%08X%04X%04X%02X%02X%02X%02X%02X%02X%02X%02X", guid.Data1, guid.Data2, guid.Data3
			, guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3]
			, guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
	}
	wstring wsGuid = szGuid;
	return move(wsGuid);
}

inline GUID&& ESBCommon::String2GUID(const wstring& wsGuid)
{
	GUID guid = GUID_NULL;
	wchar_t szBuf[50];
	const wchar_t* szGuid = wsGuid.c_str();
	if (szGuid[0] == '{')
	{
		++szGuid;
		wcscpy_s(szBuf, szGuid);
		int len = wcslen(szBuf);
		if (len <= 0)
			return move(guid);
		if (szBuf[wcslen(szBuf) - 1] != '}')
			return move(guid);
		szBuf[wcslen(szBuf) - 1] = '\0';
	}
	else
		wcscpy_s(szBuf, szGuid);

	int len = wcslen(szBuf);
	wstring wsFormat;
	if (len == 36)
		wsFormat = L"%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X";
	else if (len == 32)
		wsFormat = L"%08X%04X%04X%02X%02X%02X%02X%02X%02X%02X%02X";
	else
		return move(guid);
	if (swscanf_s(szGuid, wsFormat.c_str(),
		&guid.Data1, &guid.Data2, &guid.Data3,
		&guid.Data4[0], &guid.Data4[1], &guid.Data4[2], &guid.Data4[3],
		&guid.Data4[4], &guid.Data4[5], &guid.Data4[6], &guid.Data4[7]) != 11)
		guid = GUID_NULL;
	return move(guid);
}

std::wstring&& ESBCommon::CreateGuid(BOOL bLine /*= TRUE*/)
{
	GUID guid;
	::CoCreateGuid(&guid);
	return GUID2String(guid, bLine);
}