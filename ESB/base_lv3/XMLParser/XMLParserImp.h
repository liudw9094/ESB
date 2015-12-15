#pragma once

#include <XMLParser/XMLParser.h>

#include "XMLPaserImp_MSWin.h"

class CXMLDocImp : public ESBXMLParser::IXMLDoc
{
private:
	CyDoc m_cyDoc;
public:
	CXMLDocImp();
	~CXMLDocImp();
	virtual SREF(ESBXMLParser::IXMLNode) && Load(const std::wstring& lpFilename);
	virtual SREF(ESBXMLParser::IXMLNode) && LoadXML(const std::wstring& lpContent);
	virtual std::wstring&&  GetXml();
	virtual SREF(ESBXMLParser::IXMLNode) && CreateRootNode(const std::wstring& lpNodeName, bool bCreateHeadRow = false);
	virtual BOOL Save(const std::wstring& lpFilename);
	virtual void Unload();
	virtual BOOL GetUTF8Str(std::string& str);
	virtual void Dispose();
};


class CXMLNodeImp : public ESBXMLParser::IXMLNode
{
private:
	CyNode m_cyNode;
public:
	CXMLNodeImp();
	CXMLNodeImp(CyNode& node);
	~CXMLNodeImp();

	virtual std::wstring&& GetNodeName() const;
	virtual SREF(ESBXMLParser::IXMLNode) && CreateSubNode(const std::wstring& lpName);
	virtual SREF(ESBXMLParser::IXMLNode) && CreateSubNode(const ESBXMLParser::IXMLNode* node);
	virtual BOOL CreateSubNode(const std::wstring& lpName, const std::wstring& lpValue);
	virtual void RemoveSubNode(const ESBXMLParser::IXMLNode* node);
	virtual void ReplaceSubNode(const ESBXMLParser::IXMLNode* dest, const ESBXMLParser::IXMLNode* src);

	virtual std::wstring&& GetNodeValueStr() const;
	virtual void	SetNodeValueStr(const std::wstring& lpValue);

	virtual void	RemoveAttributeStr(const std::wstring& lpAttrName);

	virtual std::wstring&& GetAttributeStr(const std::wstring& lpAttrName) const;
	virtual void	SetAttributeStr(const std::wstring& lpAttrName, const std::wstring& lpValue);
	virtual void	AddAttributeStr(const std::wstring& lpAttrName, const std::wstring& lpValue);

	virtual std::wstring&&	ToXMLString() const;

	virtual SREF(ESBXMLParser::IXMLNode) && GetSubNode(const std::wstring& lpNodeName) const;
	virtual std::wstring&& GetSubNodeValue(const std::wstring& lpNodeName) const;
	virtual BOOL	GetSubNodeValue(const std::wstring& lpNodeName, OUT std::wstring& strValue) const;

	virtual SREF(ESBXMLParser::IXMLNodeList) GetSubNodes() const;
	virtual SREF(ESBXMLParser::IXMLNodeList) GetSubNodesByTag(const std::wstring& lpName) const;
	virtual int GetSubNodesCount() const;
	virtual int GetSubNodesCountByTag(const std::wstring& lpName) const;

	virtual SREF(ESBXMLParser::IXMLNode) && SelectSingleNode(const std::wstring& lpPath) const;
	virtual std::wstring&& GetSingleNodeValue(const std::wstring& lpPath) const;
	virtual BOOL GetSingleNodeValue(const std::wstring& lpPath, OUT std::wstring& strValue) const;
	virtual SREF(ESBXMLParser::IXMLNodeList) && SelectNodes(const std::wstring& lpPath) const;

	virtual BOOL HasAttribute(const std::wstring& lpName) const;

	virtual SREF(ESBXMLParser::IXMLNode) && CloneSubNode(const std::wstring& nodeName) const;

	virtual BOOL IsValid() const;

	virtual void Dispose();
};

class CXMLNodeListImp : public ESBXMLParser::IXMLNodeList
{
private:
	CyNodeList m_cyList;
public:
	CXMLNodeListImp(CyNodeList& list);

	virtual LONG GetCount();
	virtual SREF(ESBXMLParser::IXMLNode) && Next() const;
	virtual void Reset();
	virtual void SetTagName(const std::wstring& lpName);

	virtual BOOL IsValid() const;

	virtual void Dispose();
};
