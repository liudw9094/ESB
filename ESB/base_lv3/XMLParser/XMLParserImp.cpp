#include "stdafx.h"
#include "XMLParser.h"
#include "XMLParserImp.h"

using namespace ESBXMLParser;
using namespace std;



/////////////////////// CXMLDocImp /////////////////////

CXMLDocImp::CXMLDocImp()
{

}

CXMLDocImp::~CXMLDocImp()
{

}

SREF(ESBXMLParser::IXMLNode) CXMLDocImp::Load(const std::wstring& lpFilename)
{
	SREF(ESBXMLParser::IXMLNode) ptrNode = new CXMLNodeImp(m_cyDoc.Load(lpFilename));
	return ptrNode;
}

SREF(ESBXMLParser::IXMLNode) CXMLDocImp::LoadXML(const std::wstring& lpContent)
{
	SREF(ESBXMLParser::IXMLNode) ptrNode = new CXMLNodeImp(m_cyDoc.LoadXML(lpContent));
	return ptrNode;
}

std::wstring CXMLDocImp::GetXml()
{
	return m_cyDoc.GetXml();
}

SREF(ESBXMLParser::IXMLNode) CXMLDocImp::CreateRootNode(const std::wstring& lpNodeName, bool bCreateHeadRow /*= false*/)
{
	SREF(ESBXMLParser::IXMLNode) ptrNode = new CXMLNodeImp(m_cyDoc.CreateRootNode(lpNodeName, bCreateHeadRow));
	return ptrNode;
}

BOOL CXMLDocImp::Save(const std::wstring& lpFilename)
{
	return m_cyDoc.Save(lpFilename);
}

void CXMLDocImp::Unload()
{
	m_cyDoc.Unload();
}

BOOL CXMLDocImp::GetUTF8Str(std::string& str)
{
	return m_cyDoc.GetUTF8Str(str);
}

void CXMLDocImp::Dispose()
{
	delete this;
}


////////////////// CXMLNodeImp ///////////////

CXMLNodeImp::CXMLNodeImp()
{

}

CXMLNodeImp::CXMLNodeImp(CyNode& node) : m_cyNode(node)
{

}

CXMLNodeImp::~CXMLNodeImp()
{

}

std::wstring CXMLNodeImp::GetNodeName() const
{
	return m_cyNode.GetNodeName();
}

SREF(IXMLNode) CXMLNodeImp::CreateSubNode(const std::wstring& lpName)
{
	SREF(IXMLNode) ptr = new CXMLNodeImp(m_cyNode.CreateSubNode(lpName));
	return ptr;
}

SREF(IXMLNode) CXMLNodeImp::CreateSubNode(const IXMLNode* node)
{
	CXMLNodeImp* imp = const_cast<CXMLNodeImp*>(static_cast<const CXMLNodeImp*>(node));
	SREF(IXMLNode) ptr = new CXMLNodeImp(m_cyNode.CreateSubNode(imp->m_cyNode));
	return ptr;
}

BOOL CXMLNodeImp::CreateSubNode(const std::wstring& lpName, const std::wstring& lpValue)
{
	return m_cyNode.CreateSubNode(lpName, lpValue);
}

void CXMLNodeImp::RemoveSubNode(const IXMLNode* node)
{
	const CXMLNodeImp* imp = static_cast<const CXMLNodeImp*>(node);
	m_cyNode.RemoveSubNode(imp->m_cyNode);
}

void CXMLNodeImp::ReplaceSubNode(const IXMLNode* dest, const IXMLNode* src)
{
	const CXMLNodeImp* impDst = static_cast<const CXMLNodeImp*>(dest);
	const CXMLNodeImp* impSrc = static_cast<const CXMLNodeImp*>(src);
	m_cyNode.ReplaceSubNode(impDst->m_cyNode, impSrc->m_cyNode);
}

std::wstring CXMLNodeImp::GetNodeValueStr() const
{
	return m_cyNode.GetNodeValueStr();
}

void CXMLNodeImp::SetNodeValueStr(const std::wstring& lpValue)
{
	m_cyNode.SetNodeValueStr(lpValue);
}

void CXMLNodeImp::RemoveAttributeStr(const std::wstring& lpAttrName)
{
	m_cyNode.RemoveAttributeStr(lpAttrName);
}

std::wstring CXMLNodeImp::GetAttributeStr(const std::wstring& lpAttrName) const
{
	return m_cyNode.GetAttributeStr(lpAttrName);
}

void CXMLNodeImp::SetAttributeStr(const std::wstring& lpAttrName, const std::wstring& lpValue)
{
	m_cyNode.SetAttributeStr(lpAttrName, lpValue);
}

void CXMLNodeImp::AddAttributeStr(const std::wstring& lpAttrName, const std::wstring& lpValue)
{
	m_cyNode.AddAttributeStr(lpAttrName, lpValue);
}

std::wstring CXMLNodeImp::ToXMLString() const
{
	return m_cyNode.ToXMLString();
}

SREF(IXMLNode) CXMLNodeImp::GetSubNode(const std::wstring& lpNodeName) const
{
	SREF(IXMLNode) ptr = new CXMLNodeImp(m_cyNode.GetSubNode(lpNodeName));
	return ptr;
}

std::wstring CXMLNodeImp::GetSubNodeValue(const std::wstring& lpNodeName) const
{
	return m_cyNode.GetSubNodeValue(lpNodeName);
}

BOOL CXMLNodeImp::GetSubNodeValue(const std::wstring& lpNodeName, OUT std::wstring& strValue) const
{
	return m_cyNode.GetSubNodeValue(lpNodeName, strValue);
}

SREF(IXMLNodeList) CXMLNodeImp::GetSubNodes() const
{
	SREF(IXMLNodeList) ptr = new CXMLNodeListImp(m_cyNode.GetSubNodes());
	return ptr;
}

SREF(IXMLNodeList) CXMLNodeImp::GetSubNodesByTag(const std::wstring& lpName) const
{
	SREF(IXMLNodeList) ptr = new CXMLNodeListImp(m_cyNode.GetSubNodesByTag(lpName));
	return ptr;
}

int CXMLNodeImp::GetSubNodesCount() const
{
	return m_cyNode.GetSubNodesCount();
}

int CXMLNodeImp::GetSubNodesCountByTag(const std::wstring& lpName) const
{
	return m_cyNode.GetSubNodesCountByTag(lpName);
}

SREF(IXMLNode) CXMLNodeImp::SelectSingleNode(const std::wstring& lpPath) const
{
	SREF(IXMLNode) ptr = new CXMLNodeImp(m_cyNode.SelectSingleNode(lpPath));
	return ptr;
}

std::wstring CXMLNodeImp::GetSingleNodeValue(const std::wstring& lpPath) const
{
	return m_cyNode.GetSingleNodeValue(lpPath);
}

BOOL CXMLNodeImp::GetSingleNodeValue(const std::wstring& lpPath, OUT std::wstring& strValue) const
{
	return m_cyNode.GetSingleNodeValue(lpPath, strValue);
}

SREF(IXMLNodeList) CXMLNodeImp::SelectNodes(const std::wstring& lpPath) const
{
	SREF(IXMLNodeList) ptr = new CXMLNodeListImp(m_cyNode.SelectNodes(lpPath));
	return ptr;
}

BOOL CXMLNodeImp::HasAttribute(const std::wstring& lpName) const
{
	return m_cyNode.HasAttribute(lpName);
}

SREF(IXMLNode) CXMLNodeImp::CloneSubNode(const std::wstring& nodeName) const
{
	SREF(IXMLNode) ptr = new CXMLNodeImp(m_cyNode.CloneSubNode(nodeName));
	return ptr;
}

BOOL CXMLNodeImp::IsValid() const
{
	return m_cyNode.IsValid();
}

void CXMLNodeImp::Dispose()
{
	delete this;
}


///////////////// CXMLNodeListImp ///////////////

CXMLNodeListImp::CXMLNodeListImp(CyNodeList& list) :
	m_cyList(list)
{

}

LONG CXMLNodeListImp::GetCount()
{
	return m_cyList.GetCount();
}

SREF(IXMLNode) CXMLNodeListImp::Next() const
{
	SREF(IXMLNode) ptr = new CXMLNodeImp(m_cyList.Next());
	return ptr;
}

void CXMLNodeListImp::Reset()
{
	m_cyList.Reset();
}

void CXMLNodeListImp::SetTagName(const std::wstring& lpName)
{
	m_cyList.SetTagName(lpName);
}

BOOL CXMLNodeListImp::IsValid() const
{
	return m_cyList.IsValid();
}

void CXMLNodeListImp::Dispose()
{
	delete this;
}

XMLPARSER_API IXMLDoc* ESBXMLParser::CreateXMLDoc()
{
	return new CXMLDocImp;
}

XMLPARSER_API IXMLNode* ESBXMLParser::CreateXMLNode()
{
	return new CXMLNodeImp;
}