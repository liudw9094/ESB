#include "stdafx.h"


using namespace ESBXMLParser;
using namespace std;

#define STR_ESBCLIENTTOKEN L"ESBClientToken"
#define		STR_ESBCLIENTTOKEN_ATTB_WSSESSION L"wsSession"
#define		STR_ESBCLIENTTOKEN_ATTB_WSURLDST L"wsURLDst"


BOOL ESBDataSerialzer::Data2String(IN const ESBCommon::ESBClientToken& data, OUT std::wstring& string)
{
	SREF(IXMLDoc) xmlDoc = CreateXMLDoc();
	SREF(IXMLNode) root = xmlDoc->CreateRootNode(STR_ESBCLIENTTOKEN);
	root->AddAttributeStr(STR_ESBCLIENTTOKEN_ATTB_WSSESSION, data.wsClientSession);
	root->AddAttributeStr(STR_ESBCLIENTTOKEN_ATTB_WSURLDST, data.wsURLRedirection);
	string = root->ToXMLString();
	return TRUE;
}

BOOL ESBDataSerialzer::String2Data(IN const std::wstring& string, OUT ESBCommon::ESBClientToken& data)
{
	SREF(IXMLDoc) xmlDoc = CreateXMLDoc();
	SREF(IXMLNode) root = xmlDoc->LoadXML(string);
	if (!root->IsValid() || root->GetNodeName() != STR_ESBCLIENTTOKEN)
		return FALSE;
	data.wsSession = root->GetAttributeStr(STR_ESBTOKEN_ATTB_WSSESSION);
	data.wsURLDst = root->GetAttributeStr(STR_ESBTOKEN_ATTB_WSURLDST);
}

BOOL ESBDataSerialzer::Data2String(IN const ESBCommon::ESBServiceToken& data, OUT std::wstring& string);
BOOL ESBDataSerialzer::String2Data(IN const std::wstring& string, OUT ESBCommon::ESBServiceToken& data);

BOOL ESBDataSerialzer::Data2String(IN const ESBCommon::ESBServiceHubSession& data, OUT std::wstring& string);
BOOL ESBDataSerialzer::String2Data(IN const std::wstring& string, OUT ESBCommon::ESBServiceHubSession& data);

BOOL ESBDataSerialzer::Data2String(IN const ESBCommon::ESBService_RegisterToHub& data, OUT std::wstring& string);
BOOL ESBDataSerialzer::String2Data(IN const std::wstring& string, OUT ESBCommon::ESBService_RegisterToHub& data);
