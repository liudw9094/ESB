#include "stdafx.h"


using namespace std;

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

BOOL ESBDataSerialzer::Data2String(OUT std::wstring& string, IN const std::vector<BYTE>& data)
{
	return Data2String(string, &data[0], data.size());
}

BOOL ESBDataSerialzer::Data2String(OUT std::wstring& string, IN const void *data, IN size_t len)
{
	try
	{
		string = DataToBase64W(data, len);
		return TRUE;
	}
	catch (const std::runtime_error &e)
	{
		clog << e.what();
		return FALSE;
	}
}

BOOL ESBDataSerialzer::String2Data(OUT std::vector<BYTE>& data, IN const std::wstring& string)
{
	try
	{
		data = Base64ToDataW(string);
		return TRUE;
	}
	catch (const std::runtime_error &e)
	{
		clog << e.what();
		return FALSE;
	}
}

BOOL ESBDataSerialzer::Data2String(OUT std::wstring& string, IN int data)
{
	string = to_wstring(data);
	return TRUE;
}

BOOL ESBDataSerialzer::String2Data(OUT int& data, IN const std::wstring& string)
{
	data = stoi(string);
	return TRUE;
}

BOOL ESBDataSerialzer::Data2String(OUT std::wstring& string, IN unsigned int data)
{
	string = to_wstring(data);
	return TRUE;
}

BOOL ESBDataSerialzer::String2Data(OUT unsigned int& data, IN const std::wstring& string)
{
	data = (unsigned int)stoul(string);
	return TRUE;
}

BOOL ESBDataSerialzer::Data2String(OUT std::wstring& string, IN long data)
{
	string = to_wstring(data);
	return TRUE;
}

BOOL ESBDataSerialzer::String2Data(OUT long& data, IN const std::wstring& string)
{
	data = stol(string);
	return TRUE;
}

BOOL ESBDataSerialzer::Data2String(OUT std::wstring& string, IN unsigned long data)
{
	string = to_wstring(data);
	return TRUE;
}

BOOL ESBDataSerialzer::String2Data(OUT unsigned long& data, IN const std::wstring& string)
{
	data = stoul(string);
	return TRUE;
}

BOOL ESBDataSerialzer::Data2String(OUT std::wstring& string, IN long long data)
{
	string = to_wstring(data);
	return TRUE;
}

BOOL ESBDataSerialzer::String2Data(OUT long long& data, IN const std::wstring& string)
{
	data = stoull(string);
	return TRUE;
}

BOOL ESBDataSerialzer::Data2String(OUT std::wstring& string, IN unsigned long long data)
{
	string = to_wstring(data);
	return TRUE;
}

BOOL ESBDataSerialzer::String2Data(OUT unsigned long long& data, IN const std::wstring& string)
{
	data = stoull(string);
	return TRUE;
}

BOOL ESBDataSerialzer::Data2String(OUT std::wstring& string, IN double data)
{
	string = to_wstring(data);
	return TRUE;
}

BOOL ESBDataSerialzer::String2Data(OUT double& data, IN const std::wstring& string)
{
	data = stod(string);
	return TRUE;
}

BOOL ESBDataSerialzer::Data2String(OUT std::wstring& string, IN float data)
{
	string = to_wstring(data);
	return TRUE;
}

BOOL ESBDataSerialzer::String2Data(OUT float& data, IN const std::wstring& string)
{
	data = stof(string);
	return TRUE;
}