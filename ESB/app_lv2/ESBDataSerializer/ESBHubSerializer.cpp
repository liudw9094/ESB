#include "stdafx.h"


using namespace ESBXMLParser;
using namespace std;
using namespace ESBCommon;
using namespace ESBDataSerialzer;

BOOL ESBDataSerialzer::Data2String(OUT std::wstring& string, IN const ESBCommon::ESBServiceRequest& data)
{
	SREF(IXMLDoc) xmlDoc = CreateXMLDoc();
	SREF(IXMLNode) root = xmlDoc->CreateRootNode(ESBService_ReplyOK::NAMES.ROOTNAME);
	root->SetNodeValueStr(data.contents);
	string = root->ToXMLString();
	return TRUE;
}

BOOL ESBDataSerialzer::String2Data(OUT ESBCommon::ESBServiceRequest& data, IN const std::wstring& string)
{
	SREF(IXMLDoc) xmlDoc = CreateXMLDoc();
	SREF(IXMLNode) root = xmlDoc->LoadXML(string);
	if (!root->IsValid() || root->GetNodeName() != ESBServiceRequest::NAMES.ROOTNAME)
		return FALSE;
	data.contents = root->GetNodeValueStr();
	return TRUE;
}

BOOL ESBDataSerialzer::Data2String(OUT std::wstring& string, IN const ESBCommon::ESBServiceReply& data)
{
	SREF(IXMLDoc) xmlDoc = CreateXMLDoc();
	SREF(IXMLNode) root = xmlDoc->CreateRootNode(ESBServiceReply::NAMES.ROOTNAME);
	root->SetNodeValueStr(data.contents);
	string = root->ToXMLString();
	return TRUE;
}

BOOL ESBDataSerialzer::String2Data(OUT ESBCommon::ESBServiceReply& data, IN const std::wstring& string)
{
	SREF(IXMLDoc) xmlDoc = CreateXMLDoc();
	SREF(IXMLNode) root = xmlDoc->LoadXML(string);
	if (!root->IsValid() || root->GetNodeName() != ESBServiceReply::NAMES.ROOTNAME)
		return FALSE;
	data.contents = root->GetNodeValueStr();
	return TRUE;
}

BOOL ESBDataSerialzer::Data2String(OUT std::wstring& string, IN const ESBCommon::ESBService_ReplyOK& data)
{
	SREF(IXMLDoc) xmlDoc = CreateXMLDoc();
	SREF(IXMLNode) root = xmlDoc->CreateRootNode(ESBService_ReplyOK::NAMES.ROOTNAME);
	string = root->ToXMLString();
	return TRUE;
}

BOOL ESBDataSerialzer::String2Data(OUT ESBCommon::ESBService_ReplyOK& data, IN const std::wstring& string)
{
	SREF(IXMLDoc) xmlDoc = CreateXMLDoc();
	SREF(IXMLNode) root = xmlDoc->LoadXML(string);
	if (!root->IsValid() || root->GetNodeName() != ESBService_ReplyOK::NAMES.ROOTNAME)
		return FALSE;
	return TRUE;
}

BOOL ESBDataSerialzer::Data2String(OUT std::wstring& string, IN const ESBCommon::ESBClientToken& data)
{
	SREF(IXMLDoc) xmlDoc = CreateXMLDoc();
	SREF(IXMLNode) root = xmlDoc->CreateRootNode(ESBClientToken::NAMES.ROOTNAME);
	root->AddAttributeStr(ESBClientToken::NAMES.WSCLIENTSESSION, data.wsClientSession);
	wstring wsTimeStamp, wsTimeReplyDeadline;
	Data2String(wsTimeStamp, data.timeStamp);
	Data2String(wsTimeReplyDeadline, data.timeReplyDeadLine);
	root->AddAttributeStr(ESBClientToken::NAMES.TIMESTAMP, wsTimeStamp);
	root->AddAttributeStr(ESBClientToken::NAMES.TIMEREPLYDEADLINE, wsTimeReplyDeadline);
	root->AddAttributeStr(ESBClientToken::NAMES.WSURLREDIRECTION, data.wsURLRedirection);
	string = root->ToXMLString();
	return TRUE;
}

BOOL ESBDataSerialzer::String2Data(OUT ESBCommon::ESBClientToken& data, IN const std::wstring& string)
{
	SREF(IXMLDoc) xmlDoc = CreateXMLDoc();
	SREF(IXMLNode) root = xmlDoc->LoadXML(string);
	if (!root->IsValid() || root->GetNodeName() != ESBClientToken::NAMES.ROOTNAME)
		return FALSE;
	data.wsClientSession = root->GetAttributeStr(ESBClientToken::NAMES.TIMEREPLYDEADLINE);
	wstring wsTimeStamp = root->GetAttributeStr(ESBClientToken::NAMES.TIMESTAMP);
	wstring wsTimeReplyDeadline = root->GetAttributeStr(ESBClientToken::NAMES.TIMEREPLYDEADLINE);
	String2Data(data.timeStamp, wsTimeStamp);
	String2Data(data.timeReplyDeadLine, wsTimeReplyDeadline);
	data.wsURLRedirection = root->GetAttributeStr(ESBClientToken::NAMES.WSURLREDIRECTION);
	return TRUE;
}

BOOL ESBDataSerialzer::Data2String(OUT std::wstring& string, IN const ESBCommon::ESBServiceToken& data)
{
	SREF(IXMLDoc) xmlDoc = CreateXMLDoc();
	SREF(IXMLNode) root = xmlDoc->CreateRootNode(ESBServiceToken::NAMES.ROOTNAME);
	root->AddAttributeStr(ESBServiceToken::NAMES.WSCLIENTSESSION, data.wsClientSession);
	wstring wsTimeStamp, wsTimeReplyDeadline;
	Data2String(wsTimeStamp, data.timeStamp);
	Data2String(wsTimeReplyDeadline, data.timeReplyDeadLine);
	root->AddAttributeStr(ESBServiceToken::NAMES.TIMESTAMP, wsTimeStamp);
	root->AddAttributeStr(ESBServiceToken::NAMES.TIMEREPLYDEADLINE, wsTimeReplyDeadline);
	string = root->ToXMLString();
	return TRUE;
}

BOOL ESBDataSerialzer::String2Data(OUT ESBCommon::ESBServiceToken& data, IN const std::wstring& string)
{
	SREF(IXMLDoc) xmlDoc = CreateXMLDoc();
	SREF(IXMLNode) root = xmlDoc->LoadXML(string);
	if (!root->IsValid() || root->GetNodeName() != ESBServiceToken::NAMES.ROOTNAME)
		return FALSE;
	data.wsClientSession = root->GetAttributeStr(ESBServiceToken::NAMES.TIMEREPLYDEADLINE);
	wstring wsTimeStamp = root->GetAttributeStr(ESBServiceToken::NAMES.TIMESTAMP);
	wstring wsTimeReplyDeadline = root->GetAttributeStr(ESBServiceToken::NAMES.TIMEREPLYDEADLINE);
	String2Data(data.timeStamp, wsTimeStamp);
	String2Data(data.timeReplyDeadLine, wsTimeReplyDeadline);
	return TRUE;
}

BOOL ESBDataSerialzer::Data2String(OUT std::wstring& string, IN const ESBCommon::ESBServiceHubSessionReply& data)
{
	SREF(IXMLDoc) xmlDoc = CreateXMLDoc();
	SREF(IXMLNode) root = xmlDoc->CreateRootNode(ESBServiceHubSessionReply::NAMES.ROOTNAME);
	root->AddAttributeStr(ESBServiceHubSessionReply::NAMES.WSSERVICESESSION, data.wsServiceSession);
	string = root->ToXMLString();
	return TRUE;
}

BOOL ESBDataSerialzer::String2Data(OUT ESBCommon::ESBServiceHubSessionReply& data, IN const std::wstring& string)
{
	SREF(IXMLDoc) xmlDoc = CreateXMLDoc();
	SREF(IXMLNode) root = xmlDoc->LoadXML(string);
	if (!root->IsValid() || root->GetNodeName() != ESBServiceHubSessionReply::NAMES.ROOTNAME)
		return FALSE;
	data.wsServiceSession = root->GetAttributeStr(ESBServiceHubSessionReply::NAMES.WSSERVICESESSION);
	return TRUE;
}

BOOL ESBDataSerialzer::Data2String(OUT std::wstring& string, IN const ESBCommon::ESBService_HubMethod_RegisterToHub& data)
{
	SREF(IXMLDoc) xmlDoc = CreateXMLDoc();
	SREF(IXMLNode) root = xmlDoc->CreateRootNode(ESBService_HubMethod_RegisterToHub::NAMES.ROOTNAME);
	root->AddAttributeStr(ESBService_HubMethod_RegisterToHub::NAMES.WSSERVICEURL, data.wsServiceURL);
	wstring wsguidService;
	Data2String(wsguidService, data.guidService);
	root->AddAttributeStr(ESBService_HubMethod_RegisterToHub::NAMES.GUIDSERVICE, wsguidService);
	root->AddAttributeStr(ESBService_HubMethod_RegisterToHub::NAMES.WSSERVICENAME, data.wsServiceName);
	wstring wsmaximumSession;
	Data2String(wsmaximumSession, data.maximumSession);
	root->AddAttributeStr(ESBService_HubMethod_RegisterToHub::NAMES.MAXIMUMSESSION, wsmaximumSession);
	string = root->ToXMLString();
	return TRUE;

}

BOOL ESBDataSerialzer::String2Data(OUT ESBCommon::ESBService_HubMethod_RegisterToHub& data, IN const std::wstring& string)
{
	SREF(IXMLDoc) xmlDoc = CreateXMLDoc();
	SREF(IXMLNode) root = xmlDoc->LoadXML(string);
	if (!root->IsValid() || root->GetNodeName() != ESBService_HubMethod_RegisterToHub::NAMES.ROOTNAME)
		return FALSE;
	data.wsServiceURL = root->GetAttributeStr(ESBService_HubMethod_RegisterToHub::NAMES.WSSERVICEURL);
	wstring wsguidService = root->GetAttributeStr(ESBService_HubMethod_RegisterToHub::NAMES.GUIDSERVICE);
	String2Data(data.guidService, wsguidService);
	data.wsServiceName = root->GetAttributeStr(ESBService_HubMethod_RegisterToHub::NAMES.WSSERVICENAME);
	wstring wsmaximumSession = root->GetAttributeStr(ESBService_HubMethod_RegisterToHub::NAMES.MAXIMUMSESSION);
	String2Data(data.maximumSession, wsmaximumSession);
	return TRUE;
}

BOOL ESBDataSerialzer::Data2String(OUT std::wstring& string, IN const ESBCommon::ESBService_HubMethod_ModifySessionLimitation& data)
{
	SREF(IXMLDoc) xmlDoc = CreateXMLDoc();
	SREF(IXMLNode) root = xmlDoc->CreateRootNode(ESBService_HubMethod_ModifySessionLimitation::NAMES.ROOTNAME);
	wstring wsMaximumSession;
	Data2String(wsMaximumSession, data.maximumSession);
	root->AddAttributeStr(ESBService_HubMethod_ModifySessionLimitation::NAMES.MAXIMUMSESSION, wsMaximumSession);
	string = root->ToXMLString();
	return TRUE;
}

BOOL ESBDataSerialzer::String2Data(OUT ESBCommon::ESBService_HubMethod_ModifySessionLimitation& data, IN const std::wstring& string)
{
	SREF(IXMLDoc) xmlDoc = CreateXMLDoc();
	SREF(IXMLNode) root = xmlDoc->LoadXML(string);
	if (!root->IsValid() || root->GetNodeName() != ESBService_HubMethod_ModifySessionLimitation::NAMES.ROOTNAME)
		return FALSE;
	wstring wsMaximumSession = root->GetAttributeStr(ESBService_HubMethod_ModifySessionLimitation::NAMES.MAXIMUMSESSION);
	String2Data(data.maximumSession, wsMaximumSession);
	return TRUE;
}

BOOL ESBDataSerialzer::Data2String(OUT std::wstring& string, IN const ESBCommon::ESBService_HubMethod_IncreaseSessionLoad& data)
{
	SREF(IXMLDoc) xmlDoc = CreateXMLDoc();
	SREF(IXMLNode) root = xmlDoc->CreateRootNode(ESBService_HubMethod_IncreaseSessionLoad::NAMES.ROOTNAME);
	string = root->ToXMLString();
	return TRUE;
}

BOOL ESBDataSerialzer::String2Data(OUT ESBCommon::ESBService_HubMethod_IncreaseSessionLoad& data, IN const std::wstring& string)
{
	SREF(IXMLDoc) xmlDoc = CreateXMLDoc();
	SREF(IXMLNode) root = xmlDoc->LoadXML(string);
	if (!root->IsValid() || root->GetNodeName() != ESBService_HubMethod_IncreaseSessionLoad::NAMES.ROOTNAME)
		return FALSE;
	return TRUE;
}

BOOL ESBDataSerialzer::Data2String(OUT std::wstring& string, IN const ESBCommon::ESBService_HubMethod_DecreaseSessionLoad& data)
{
	SREF(IXMLDoc) xmlDoc = CreateXMLDoc();
	SREF(IXMLNode) root = xmlDoc->CreateRootNode(ESBService_HubMethod_DecreaseSessionLoad::NAMES.ROOTNAME);
	string = root->ToXMLString();
	return TRUE;
}

BOOL ESBDataSerialzer::String2Data(OUT ESBCommon::ESBService_HubMethod_DecreaseSessionLoad& data, IN const std::wstring& string)
{
	SREF(IXMLDoc) xmlDoc = CreateXMLDoc();
	SREF(IXMLNode) root = xmlDoc->LoadXML(string);
	if (!root->IsValid() || root->GetNodeName() != ESBService_HubMethod_DecreaseSessionLoad::NAMES.ROOTNAME)
		return FALSE;
	return TRUE;
}

BOOL ESBDataSerialzer::Data2String(OUT std::wstring& string, IN const ESBCommon::ESBService_HubMethod_Unregister& data)
{
	SREF(IXMLDoc) xmlDoc = CreateXMLDoc();
	SREF(IXMLNode) root = xmlDoc->CreateRootNode(ESBService_HubMethod_Unregister::NAMES.ROOTNAME);
	string = root->ToXMLString();
	return TRUE;
}

BOOL ESBDataSerialzer::String2Data(OUT ESBCommon::ESBService_HubMethod_Unregister& data, IN const std::wstring& string)
{
	SREF(IXMLDoc) xmlDoc = CreateXMLDoc();
	SREF(IXMLNode) root = xmlDoc->LoadXML(string);
	if (!root->IsValid() || root->GetNodeName() != ESBService_HubMethod_Unregister::NAMES.ROOTNAME)
		return FALSE;
	return TRUE;
}
