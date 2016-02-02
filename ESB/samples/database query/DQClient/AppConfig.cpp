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
	auto pXmlAppNode = pXmlRoot->GetSubNode(L"DQClient");
	if (!pXmlAppNode->IsValid())
		return FALSE;
	if (pXmlAppNode->HasAttribute(L"URL"))
	{
		wstring szHubURL = pXmlAppNode->GetAttributeStr(L"URL");
		if (szHubURL != L"")
			m_cfg.szHubURL = szHubURL;
	}


	if (pXmlAppNode->HasAttribute(L"URL"))
	{
		wstring szHubURL = pXmlAppNode->GetAttributeStr(L"URL");
		if (szHubURL != L"")
			m_cfg.szHubURL = szHubURL;
	}


	return TRUE;
}

BOOL CAppConfig::Save()
{
	throw logic_error("Unimplemented method: CAppConfig::Save()");
}

SAppConfig&& CAppConfig::GetConfig()
{
	return move(m_cfg);
}
