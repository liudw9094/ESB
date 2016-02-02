#include "stdafx.h"
#include "AppConfig.h"

using namespace std;
using namespace Utils::SafeCoding;
using namespace ESBXMLParser;
using namespace ESBDataSerialzer;

// {2FED03ED-F251-4A03-9EC4-40ADB4D65E7F}
const GUID SERVICE_GUID =
{ 0x2fed03ed, 0xf251, 0x4a03,{ 0x9e, 0xc4, 0x40, 0xad, 0xb4, 0xd6, 0x5e, 0x7f } };


CAppConfig::CAppConfig(const wchar_t *path)
	: m_szFilePath(path)
{
}


CAppConfig::~CAppConfig()
{
}


BOOL CAppConfig::Load()
{
	SREF(IXMLDoc) pXmlDoc = CreateXMLDoc();
	auto pXmlRoot = pXmlDoc->Load(m_szFilePath);
	if (!pXmlRoot->IsValid() || pXmlRoot->GetNodeName() != L"Root")
		return FALSE;
	auto pXmlAppNode = pXmlRoot->GetSubNode(L"DQService");
	if (!pXmlAppNode->IsValid())
		return FALSE;
	if (pXmlAppNode->HasAttribute(L"Port"))
	{
		wstring szPort = pXmlAppNode->GetAttributeStr(L"Port");
		if (szPort != L"")
			String2Data(m_cfg.nPort, szPort);
	}

	{
		auto pXmlHubConnection = pXmlAppNode->GetSubNode(L"HubConnection");
		if (!pXmlHubConnection->IsValid())
			return FALSE;
		_LoadHubConnection(pXmlHubConnection);
	}

	{
		auto pXmlDbConnection = pXmlAppNode->GetSubNode(L"DbConnection");
		if (!pXmlDbConnection->IsValid())
			return FALSE;
		_LoadDbConnection(pXmlDbConnection);
	}

	return TRUE;
}

BOOL CAppConfig::Save()
{
	throw logic_error("Unimplemented method: CAppConfig::Save()");
	/*
	SREF(IXMLDoc) pXmlDoc = CreateXMLDoc();
	auto pXmlRoot = pXmlDoc->CreateRootNode(L"ESBRoot", true);
	if (!pXmlRoot->IsValid())
		return FALSE;

	auto pXmlAppNode = pXmlRoot->CreateSubNode(L"ESBHubService");
	if (!pXmlAppNode->IsValid())
		return FALSE;
	wstring szPort;
	Data2String(szPort, m_cfg.nPort);
	pXmlAppNode->AddAttributeStr(L"Port", szPort);

	auto pXmlHubConnection = pXmlAppNode->CreateSubNode(L"HubConnection");
	if (!pXmlHubConnection->IsValid())
		return FALSE;
	pXmlHubConnection->AddAttributeStr(L"URL", m_cfg.hubConnection.szURL);


	return pXmlDoc->Save(m_szFilePath);
	*/
}

SAppConfig&& CAppConfig::GetConfig()
{
	return move(m_cfg);
}

void CAppConfig::_LoadHubConnection(const ESBXMLParser::IXMLNode *node)
{
	if (node->HasAttribute(L"HubURL"))
	{
		wstring value = node->GetAttributeStr(L"HubURL");
		if (value != L"")
			m_cfg.hubConnection.szHubURL = value;
	}
	if (node->HasAttribute(L"ServiceURL"))
	{
		wstring value = node->GetAttributeStr(L"HubURL");
		if (value != L"")
			m_cfg.hubConnection.szServiceURL = value;
	}
	if (node->HasAttribute(L"ServiceGUID"))
	{
		wstring value = node->GetAttributeStr(L"HubURL");
		if (value != L"")
			String2Data(m_cfg.hubConnection.szServiceGUID, value);
	}
	if (node->HasAttribute(L"ServiceName"))
	{
		wstring value = node->GetAttributeStr(L"HubURL");
		if (value != L"")
			m_cfg.hubConnection.szServiceName = value;
	}
	if (node->HasAttribute(L"MaximumSessionCount"))
	{
		wstring value = node->GetAttributeStr(L"MaximumSessionCount");
		if (value != L"")
			String2Data(m_cfg.hubConnection.nMaximumSession, value);
	}
}

void CAppConfig::_LoadDbConnection(const ESBXMLParser::IXMLNode * node)
{
	m_cfg.dbConnection.szConStr = node->GetAttributeStr(L"ConStr");
	m_cfg.dbConnection.szUsername = node->GetAttributeStr(L"Username");
	m_cfg.dbConnection.szPwd = node->GetAttributeStr(L"Pwd");
}
