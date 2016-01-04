#include "stdafx.h"

#include <stdlib.h>

using namespace ESBCommon;

BOOL ESBDataSerialzer::Data2String(OUT std::wstring& string, IN const GUID& data)
{
	string = GUID2String(data);
	return TRUE;
}

BOOL ESBDataSerialzer::String2Data(OUT GUID& data, IN const std::wstring& string)
{
	data = String2GUID(string);
	return TRUE;
}

BOOL ESBDataSerialzer::Data2String(OUT std::wstring& string, IN const time_t& data)
{
	// TODO: add implementation.
	ASSERT(false);
	return FALSE;
}

BOOL ESBDataSerialzer::String2Data(OUT time_t& data, IN const std::wstring& string)
{
	// TODO: add implementation.
	ASSERT(false);
	return FALSE;
}

BOOL ESBDataSerialzer::Data2String(OUT std::wstring& string, IN const std::vector<char>& data)
{
	// TODO: add implementation.
	ASSERT(false);
	return FALSE;
}

BOOL ESBDataSerialzer::String2Data(OUT std::vector<char>& data, IN const std::wstring& string)
{
	// TODO: add implementation.
	ASSERT(false);
	return FALSE;
}

BOOL ESBDataSerialzer::Data2String(OUT std::wstring& string, IN int data)
{
	// TODO: add implementation.
	ASSERT(false);
	return FALSE;
}

BOOL ESBDataSerialzer::String2Data(OUT int& data, IN const std::wstring& string)
{
	// TODO: add implementation.
	ASSERT(false);
	return FALSE;
}

BOOL ESBDataSerialzer::Data2String(OUT std::wstring& string, IN unsigned int data)
{
	// TODO: add implementation.
	ASSERT(false);
	return FALSE;
}

BOOL ESBDataSerialzer::String2Data(OUT unsigned int& data, IN const std::wstring& string)
{
	// TODO: add implementation.
	ASSERT(false);
	return FALSE;
}

BOOL ESBDataSerialzer::Data2String(OUT std::wstring& string, IN long data)
{
	// TODO: add implementation.
	ASSERT(false);
	return FALSE;
}

BOOL ESBDataSerialzer::String2Data(OUT long& data, IN const std::wstring& string)
{
	// TODO: add implementation.
	ASSERT(false);
	return FALSE;
}

BOOL ESBDataSerialzer::Data2String(OUT std::wstring& string, IN unsigned long data)
{
	// TODO: add implementation.
	ASSERT(false);
	return FALSE;
}

BOOL ESBDataSerialzer::String2Data(OUT unsigned long& data, IN const std::wstring& string)
{
	// TODO: add implementation.
	ASSERT(false);
	return FALSE;
}

BOOL ESBDataSerialzer::Data2String(OUT std::wstring& string, IN double data)
{
	// TODO: add implementation.
	ASSERT(false);
	return FALSE;
}

BOOL ESBDataSerialzer::String2Data(OUT double& data, IN const std::wstring& string)
{
	// TODO: add implementation.
	ASSERT(false);
	return FALSE;
}

BOOL ESBDataSerialzer::Data2String(OUT std::wstring& string, IN float data)
{
	// TODO: add implementation.
	ASSERT(false);
	return FALSE;
}

BOOL ESBDataSerialzer::String2Data(OUT float& data, IN const std::wstring& string)
{
	// TODO: add implementation.
	ASSERT(false);
	return FALSE;
}