#include "stdafx.h"


using namespace ESBXMLParser;
using namespace std;
using namespace ESBCommon;
using namespace ESBDataSerialzer;

BOOL ESBDataSerialzer::Data2String(OUT std::wstring& string, IN const ESBCommon::ENUM_IDTYPE& data)
{
	switch (data)
	{
	case IDTYPE_ESBClient:
		string = L"IDTYPE_ESBClient";
		break;
	case IDTYPE_ESBService:
		string = L"IDTYPE_ESBService";
		break;
	case IDTYPE_ESBHub:
		string = L"IDTYPE_ESBHub";
		break;
	default:
		string = L"Unknown";
	};
	return TRUE;
}

BOOL ESBDataSerialzer::String2Data(OUT ESBCommon::ENUM_IDTYPE& data, IN const std::wstring& string)
{
	if (string == L"IDTYPE_ESBClient")
		data = IDTYPE_ESBClient;
	else if (string == L"IDTYPE_ESBService")
		data = IDTYPE_ESBService;
	else if (string == L"IDTYPE_ESBService")
		data = IDTYPE_ESBService;
	else
		data = IDTYPE_ESBUnknown;
	return TRUE;
}

BOOL ESBDataSerialzer::Data2String(OUT std::wstring& string, IN const ESBCommon::ESBServiceRequest& data)
{
	SREF(IXMLDoc) xmlDoc = CreateXMLDoc();
	SREF(IXMLNode) root = xmlDoc->CreateRootNode(ESBServiceRequest::NAMES.ROOTNAME);
	wstring wsIDType;
	Data2String(wsIDType, data.idType);
	root->SetAttributeStr(ESBServiceRequest::NAMES.IDTYPE, wsIDType);
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
	wstring wsIDType = root->GetAttributeStr(ESBServiceRequest::NAMES.IDTYPE);
	String2Data(data.idType, wsIDType);
	data.contents = root->GetNodeValueStr();
	return TRUE;
}

BOOL ESBDataSerialzer::Data2String(OUT std::wstring& string, IN const ESBCommon::ESBServiceReply& data)
{
	SREF(IXMLDoc) xmlDoc = CreateXMLDoc();
	SREF(IXMLNode) root = xmlDoc->CreateRootNode(ESBServiceReply::NAMES.ROOTNAME);
	wstring wsIDType;
	Data2String(wsIDType, data.idType);
	root->SetAttributeStr(ESBServiceReply::NAMES.IDTYPE, wsIDType);
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
	wstring wsIDType = root->GetAttributeStr(ESBServiceReply::NAMES.IDTYPE);
	String2Data(data.idType, wsIDType);
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

BOOL ESBDataSerialzer::Data2String(OUT std::wstring& string, IN const ESBCommon::ESBServiceSessionReply& data)
{
	SREF(IXMLDoc) xmlDoc = CreateXMLDoc();
	SREF(IXMLNode) root = xmlDoc->CreateRootNode(ESBServiceSessionReply::NAMES.ROOTNAME);
	root->AddAttributeStr(ESBServiceSessionReply::NAMES.WSSERVICESESSION, data.wsServiceSession);
	string = root->ToXMLString();
	return TRUE;
}

BOOL ESBDataSerialzer::String2Data(OUT ESBCommon::ESBServiceSessionReply& data, IN const std::wstring& string)
{
	SREF(IXMLDoc) xmlDoc = CreateXMLDoc();
	SREF(IXMLNode) root = xmlDoc->LoadXML(string);
	if (!root->IsValid() || root->GetNodeName() != ESBServiceSessionReply::NAMES.ROOTNAME)
		return FALSE;
	data.wsServiceSession = root->GetAttributeStr(ESBServiceSessionReply::NAMES.WSSERVICESESSION);
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

BOOL ESBDataSerialzer::Data2String(OUT std::wstring& string, IN const ESBCommon::ESBService_HubMethod_StartSession& data)
{
	SREF(IXMLDoc) xmlDoc = CreateXMLDoc();
	SREF(IXMLNode) root = xmlDoc->CreateRootNode(ESBService_HubMethod_StartSession::NAMES.ROOTNAME);
	wstring wsGuidService;
	Data2String(wsGuidService, data.guidService);
	root->AddAttributeStr(ESBService_HubMethod_StartSession::NAMES.GUIDSERVICE, wsGuidService);
	string = root->ToXMLString();
	return TRUE;
}

BOOL ESBDataSerialzer::String2Data(OUT ESBCommon::ESBService_HubMethod_StartSession& data, IN const std::wstring& string)
{
	SREF(IXMLDoc) xmlDoc = CreateXMLDoc();
	SREF(IXMLNode) root = xmlDoc->LoadXML(string);
	if (!root->IsValid() || root->GetNodeName() != ESBService_HubMethod_StartSession::NAMES.ROOTNAME)
		return FALSE;
	wstring wsGuidService = root->GetAttributeStr(ESBService_HubMethod_StartSession::NAMES.GUIDSERVICE);
	String2Data(data.guidService, wsGuidService);
	
	return TRUE;
}

BOOL ESBDataSerialzer::Data2String(OUT std::wstring& string, IN const ESBCommon::ESBService_HubMethod_ClientSessionEnd& data)
{
	SREF(IXMLDoc) xmlDoc = CreateXMLDoc();
	SREF(IXMLNode) root = xmlDoc->CreateRootNode(ESBService_HubMethod_ClientSessionEnd::NAMES.ROOTNAME);
	root->AddAttributeStr(ESBService_HubMethod_ClientSessionEnd::NAMES.WSCLIENTSESSION, data.wsClientSession);
	string = root->ToXMLString();
	return TRUE;
}

BOOL ESBDataSerialzer::String2Data(OUT ESBCommon::ESBService_HubMethod_ClientSessionEnd& data, IN const std::wstring& string)
{
	SREF(IXMLDoc) xmlDoc = CreateXMLDoc();
	SREF(IXMLNode) root = xmlDoc->LoadXML(string);
	if (!root->IsValid() || root->GetNodeName() != ESBService_HubMethod_ClientSessionEnd::NAMES.ROOTNAME)
		return FALSE;
	data.wsClientSession = root->GetAttributeStr(ESBService_HubMethod_ClientSessionEnd::NAMES.WSCLIENTSESSION);

	return TRUE;
}

BOOL ESBDataSerialzer::Data2String(OUT std::wstring& string, IN const ESBCommon::ESBService_ServiceMethod_SessionConfirm& data)
{
	SREF(IXMLDoc) xmlDoc = CreateXMLDoc();
	SREF(IXMLNode) root = xmlDoc->CreateRootNode(ESBService_ServiceMethod_SessionConfirm::NAMES.ROOTNAME);
	string = root->ToXMLString();
	return TRUE;
}

BOOL ESBDataSerialzer::String2Data(OUT ESBCommon::ESBService_ServiceMethod_SessionConfirm& data, IN const std::wstring& string)
{
	SREF(IXMLDoc) xmlDoc = CreateXMLDoc();
	SREF(IXMLNode) root = xmlDoc->LoadXML(string);
	if (!root->IsValid() || root->GetNodeName() != ESBService_ServiceMethod_SessionConfirm::NAMES.ROOTNAME)
		return FALSE;
	return TRUE;
}

BOOL ESBDataSerialzer::Data2String(OUT std::wstring& string, IN const ESBCommon::ESBService_ServiceMethod_EndSession& data)
{
	SREF(IXMLDoc) xmlDoc = CreateXMLDoc();
	SREF(IXMLNode) root = xmlDoc->CreateRootNode(ESBService_ServiceMethod_EndSession::NAMES.ROOTNAME);
	string = root->ToXMLString();
	return TRUE;
}

BOOL ESBDataSerialzer::String2Data(OUT ESBCommon::ESBService_ServiceMethod_EndSession& data, IN const std::wstring& string)
{
	SREF(IXMLDoc) xmlDoc = CreateXMLDoc();
	SREF(IXMLNode) root = xmlDoc->LoadXML(string);
	if (!root->IsValid() || root->GetNodeName() != ESBService_ServiceMethod_EndSession::NAMES.ROOTNAME)
		return FALSE;
	return TRUE;
}

BOOL ESBDataSerialzer::Data2String(OUT std::wstring& string, IN const ESBCommon::ESBService_ServiceMethod_ClientRequest& data)
{
	SREF(IXMLDoc) xmlDoc = CreateXMLDoc();
	SREF(IXMLNode) root = xmlDoc->CreateRootNode(ESBService_ServiceMethod_ClientRequest::NAMES.ROOTNAME);
	wstring wsIDType;
	root->SetNodeValueStr(data.wsContent);
	string = root->ToXMLString();
	return TRUE;
}

BOOL ESBDataSerialzer::String2Data(OUT ESBCommon::ESBService_ServiceMethod_ClientRequest& data, IN const std::wstring& string)
{
	SREF(IXMLDoc) xmlDoc = CreateXMLDoc();
	SREF(IXMLNode) root = xmlDoc->LoadXML(string);
	if (!root->IsValid() || root->GetNodeName() != ESBService_ServiceMethod_ClientRequest::NAMES.ROOTNAME)
		return FALSE;
	data.wsContent = root->GetNodeValueStr();
	return TRUE;
}