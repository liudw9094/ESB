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
	m_cfg = SAppConfig();

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

	auto pXmlAuthenticationNode = pXmlAppNode->GetSubNode(L"Authentication");
	if (pXmlAuthenticationNode->IsValid())
	{
		m_cfg.bAuthentication = true;
		m_cfg.authentication.keyfile = pXmlAuthenticationNode->GetAttributeStr(L"keyfile");
		m_cfg.authentication.password = pXmlAuthenticationNode->GetAttributeStr(L"password");
		m_cfg.authentication.cafile = pXmlAuthenticationNode->GetAttributeStr(L"cafile");
		m_cfg.authentication.capath = pXmlAuthenticationNode->GetAttributeStr(L"capath");
		m_cfg.authentication.dhfile = pXmlAuthenticationNode->GetAttributeStr(L"dhfile");
		m_cfg.authentication.randomfile = pXmlAuthenticationNode->GetAttributeStr(L"randomfile");
		m_cfg.authentication.sid = pXmlAuthenticationNode->GetAttributeStr(L"sid");
	}

	return TRUE;
}

BOOL CAppConfig::Save()
{
	throw runtime_error("Not implemented.");
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