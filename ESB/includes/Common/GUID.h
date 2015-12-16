#ifndef _INCLUDE_ESBSERIALIZER_GUID_H_
#define _INCLUDE_ESBSERIALIZER_GUID_H_

#include <string>

namespace ESBCommon
{
	std::wstring&& GUID2String(const GUID& guid, BOOL bIncludeLine = TRUE);
	GUID&& String2GUID(const std::wstring& wsGuid);
	std::wstring&& CreateGuid(BOOL bLine = TRUE);
};

#endif //_INCLUDE_ESBSERIALIZER_GUID_H_