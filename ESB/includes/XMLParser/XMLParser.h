#ifndef _INCLUDE_XMLPARSER_XMLPARSER_H
#define _INCLUDE_XMLPARSER_XMLPARSER_H

#include "../Utils/SafeCoding/IDisposable.h"
#include "../Utils/SafeCoding/SmtPtr.h"

#include <string>

#ifndef XMLPARSER_API
#define XMLPARSER_API
#endif //XMLPARSER_API

namespace ESBXMLParser
{
	class IXMLDoc;
	class IXMLNode;
	class IXMLNodeList;

	class IXMLDoc : public Utils::SafeCoding::IDisposable
	{
	public:
		virtual SREF(IXMLNode)&& Load(const std::wstring& filename) = 0;
		virtual SREF(IXMLNode)&& LoadXML(const std::wstring& content) = 0;
		virtual std::wstring&&  GetXml() = 0;
		virtual SREF(IXMLNode)&& CreateRootNode(const std::wstring& nodeName, bool bCreateHeadRow = false) = 0;
		virtual BOOL Save(const std::wstring& filename) = 0;
		virtual void Unload() = 0;
		virtual BOOL GetUTF8Str(std::string& str) = 0;
	};


	class IXMLNode : public Utils::SafeCoding::IDisposable
	{
	public:
		virtual std::wstring&& GetNodeName() const = 0;
		virtual SREF(IXMLNode)&& CreateSubNode(const std::wstring& name) = 0;
		virtual SREF(IXMLNode)&& CreateSubNode(const IXMLNode* node) = 0;
		virtual BOOL CreateSubNode(const std::wstring& name, const std::wstring& value) = 0;
		virtual void RemoveSubNode(const IXMLNode* node) = 0;
		virtual void ReplaceSubNode(const IXMLNode* dest, const IXMLNode* src) = 0;

		virtual std::wstring&& GetNodeValueStr() const = 0;
		virtual void	SetNodeValueStr(const std::wstring& value) = 0;

		virtual void	RemoveAttributeStr(const std::wstring& attrName) = 0;

		virtual std::wstring&& GetAttributeStr(const std::wstring& attrName) const = 0;
		virtual void	SetAttributeStr(const std::wstring& attrName, const std::wstring& value) = 0;
		virtual void	AddAttributeStr(const std::wstring& attrName, const std::wstring& value) = 0;

		virtual std::wstring&&	ToXMLString() const = 0;

		virtual SREF(IXMLNode)&&  GetSubNode(const std::wstring& nodeName) const = 0;
		virtual std::wstring&& GetSubNodeValue(const std::wstring& nodeName) const = 0;
		virtual BOOL	GetSubNodeValue(const std::wstring& nodeName, OUT std::wstring& strValue) const = 0;

		virtual SREF(IXMLNodeList) GetSubNodes() const = 0;
		virtual SREF(IXMLNodeList) GetSubNodesByTag(const std::wstring& name) const = 0;
		virtual int GetSubNodesCount() const = 0;
		virtual int GetSubNodesCountByTag(const std::wstring& name) const = 0;

		virtual SREF(IXMLNode)&& SelectSingleNode(const std::wstring& path) const = 0;
		virtual std::wstring&& GetSingleNodeValue(const std::wstring& path) const = 0;
		virtual BOOL GetSingleNodeValue(const std::wstring& path, OUT std::wstring& strValue) const = 0;
		virtual SREF(IXMLNodeList)&& SelectNodes(const std::wstring& path) const = 0;

		virtual BOOL HasAttribute(const std::wstring& name) const = 0;

		virtual SREF(IXMLNode)&& CloneSubNode(const std::wstring& nodeName) const = 0;

		virtual BOOL IsValid() const = 0;
	};

	class IXMLNodeList : public Utils::SafeCoding::IDisposable
	{
	public:
		virtual LONG GetCount() = 0;
		virtual SREF(IXMLNode)&& Next() const = 0;
		virtual void Reset() = 0;
		virtual void SetTagName(const std::wstring& name) = 0;

		virtual BOOL IsValid() const = 0;
	};

	XMLPARSER_API IXMLDoc* CreateXMLDoc();
	XMLPARSER_API IXMLNode* CreateXMLNode();
};

#endif