#include "stdafx.h"
#include <string.h>
#include <stdio.h>
using namespace std;

std::wstring ESBCommon::GUID2String(const GUID& guid, BOOL bIncludeLine /*= TRUE*/)
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
	return wsGuid;
}



GUID ESBCommon::String2GUID(const std::wstring& wsGuid)
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
			return guid;
		if (szBuf[wcslen(szBuf) - 1] != '}')
			return guid;
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
		return guid;

	int field[11];
	if (swscanf_s(szBuf, wsFormat.c_str(),
		&field[0], &field[1], &field[2],
		&field[3], &field[4], &field[5], &field[6],
		&field[7], &field[8], &field[9], &field[10]) != 11)
		guid = GUID_NULL;
	else
	{
		guid.Data1 = field[0];
		guid.Data2 = (unsigned short)field[1];
		guid.Data3 = (unsigned short)field[2];
		

		guid.Data4[0] = (unsigned char)field[3];
		guid.Data4[1] = (unsigned char)field[4];
		guid.Data4[2] = (unsigned char)field[5];
		guid.Data4[3] = (unsigned char)field[6];

		guid.Data4[4] = (unsigned char)field[7];
		guid.Data4[5] = (unsigned char)field[8];
		guid.Data4[6] = (unsigned char)field[9];
		guid.Data4[7] = (unsigned char)field[10];
	}

	return guid;

}

std::wstring ESBCommon::CreateGuid(BOOL bLine /*= TRUE*/)
{
	GUID guid;
	::CoCreateGuid(&guid);
	return GUID2String(guid, bLine);
}