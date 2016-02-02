// AyXML.h: interface for the AcuXML class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AcuXML__INCLUDED_)
#define AcuXML__INCLUDED_


//using namespace MSXML;
#include <string>


class CyDoc;
class CyNode;
class CyNodeList;

class CyDoc
{
public:
	CyDoc();
	virtual ~CyDoc();
	CyNode Load(const std::wstring& lpFilename);
	CyNode LoadXML(const std::wstring& lpContent);
	std::wstring  GetXml();
	CyNode CreateRootNode(const std::wstring& lpNodeName, bool bCreateHeadRow = false);
	BOOL Save(const std::wstring& lpFilename);
	void Unload();
	bool GetUTF8Str(std::string& str);
protected:
	static MSXML2::IXMLDOMDocumentPtr GetDocument();	
	MSXML2::IXMLDOMDocumentPtr m_spDocument;
public:
};


class CyNode
{	
public:	
	CyNode();
	CyNode(CyNode& node);
	CyNode(MSXML2::IXMLDOMDocumentPtr& spDoc);
	CyNode(MSXML2::IXMLDOMElementPtr& spThisNode);
	CyNode(MSXML2::IXMLDOMNodePtr& spThisNode);
	virtual ~CyNode();
	
	std::wstring GetNodeName() const;
	CyNode CreateSubNode(const std::wstring& lpName);
	CyNode CreateSubNode(CyNode& node);
	BOOL CreateSubNode(const std::wstring& lpName, const std::wstring& lpValue);
	void RemoveSubNode(const CyNode& node);
	void ReplaceSubNode(const CyNode& dest, const CyNode& src);
		
	std::wstring GetNodeValueStr() const;
	void	SetNodeValueStr(const std::wstring& lpValue);
	
	void	RemoveAttributeStr(const std::wstring& lpAttrName);

	std::wstring GetAttributeStr(const std::wstring& lpAttrName) const;
	void	SetAttributeStr(const std::wstring& lpAttrName, const std::wstring& lpValue);
	void	AddAttributeStr(const std::wstring& lpAttrName, const std::wstring& lpValue);
	
	std::wstring	ToXMLString() const;
	
	CyNode  GetSubNode(const std::wstring& lpNodeName) const;
	std::wstring GetSubNodeValue(const std::wstring& lpNodeName) const;
	BOOL	GetSubNodeValue(const std::wstring& lpNodeName, OUT std::wstring& strValue) const;

	CyNodeList GetSubNodes() const;
	CyNodeList GetSubNodesByTag(const std::wstring& lpName) const;
	int GetSubNodesCount() const;
	int GetSubNodesCountByTag(const std::wstring& lpName) const;

	CyNode SelectSingleNode(const std::wstring& lpPath) const;
	std::wstring GetSingleNodeValue(const std::wstring& lpPath) const;
	BOOL GetSingleNodeValue(const std::wstring& lpPath, OUT std::wstring& strValue) const;
	CyNodeList SelectNodes(const std::wstring& lpPath) const;
	
	BOOL HasAttribute(const std::wstring& lpName) const ;

	CyNode CloneSubNode(const std::wstring& nodeName) const ;
	MSXML2::IXMLDOMDocumentPtr GetDocumentPtr(){ return m_spDocument; }
	MSXML2::IXMLDOMNodePtr  GetNodePtr(){ return m_spNode; }

	BOOL IsValid() const { return m_spNode!=NULL; }
	//operator
	operator bool() const{ return m_spNode!=NULL;}
	BOOL operator ==(BOOL) const{ return m_spNode==NULL;}
	BOOL operator !=(BOOL) const{ return m_spNode!=NULL;}

	int m_nLevel;

protected:	
	
	void AppendHeaderText();
	void AppendTailText();

protected:
	MSXML2::IXMLDOMDocumentPtr	m_spDocument;
	MSXML2::IXMLDOMNodePtr		m_spNode;
};

class CyNodeList {
public:
	CyNodeList(CyNodeList& list);
	CyNodeList(MSXML2::IXMLDOMNodeListPtr& spList);
	LONG GetCount();
	CyNode Next() const;
	void Reset();
	MSXML2::IXMLDOMNodeListPtr GetNodeListPtr() {return m_spList;}
	void SetTagName(const std::wstring& lpName) { m_TagName =lpName; }

	BOOL IsValid() const { return m_spList!=NULL; }
	//operator
	operator bool() const{ return m_spList!=NULL;}
	BOOL operator ==(BOOL) const{ return m_spList==NULL;}
	BOOL operator !=(BOOL) const{ return m_spList!=NULL;}

protected:
	MSXML2::IXMLDOMNodeListPtr m_spList;
	std::wstring m_TagName;
};

#endif // !defined(AcuXML__INCLUDED_)
