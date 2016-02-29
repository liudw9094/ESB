#include "stdafx.h"
#include "AppConfig.h"

using namespace std;
using namespace Utils::SafeCoding;
using namespace ESBXMLParser;
using namespace ESBDataSerialzer;

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
	auto pXmlAppNode = pXmlRoot->GetSubNode(L"ESBHubService");
	if (!pXmlAppNode->IsValid())
		return FALSE;
	if (pXmlAppNode->HasAttribute(L"Port"))
	{
		wstring szPort = pXmlAppNode->GetAttributeStr(L"Port");
		if (szPort != L"")
			String2Data(m_cfg.nPort, szPort);
	}
	return TRUE;
}

BOOL CAppConfig::Save()
{
	SREF(IXMLDoc) pXmlDoc = CreateXMLDoc();
	auto pXmlRoot = pXmlDoc->CreateRootNode(L"Root", true);
	if (!pXmlRoot->IsValid())
		return FALSE;
	auto pXmlAppNode = pXmlRoot->CreateSubNode(L"ESBHubService");
	if (!pXmlAppNode->IsValid())
		return FALSE;
	wstring szPort;
	Data2String(szPort, m_cfg.nPort);
	pXmlAppNode->AddAttributeStr(L"Port", szPort);
	return pXmlDoc->Save(m_szFilePath);
}

SAppConfig&& CAppConfig::GetConfig()
{
	return move(m_cfg);
}