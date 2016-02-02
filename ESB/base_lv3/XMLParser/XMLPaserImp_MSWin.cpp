// AcuXML.cpp: implementation of the AcuXML class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XMLPaserImp_MSWin.h"


using namespace std;

//////////////////////////////////////////////////////////////////////
// Utility function
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// CyDoc
//////////////////////////////////////////////////////////////////////

CyDoc::CyDoc()
{

}

CyDoc::~CyDoc()
{

}

void CyDoc::Unload()
{
	if(m_spDocument!=NULL){
		m_spDocument = NULL;
	}
}

CyNode CyDoc::Load(const std::wstring& lpFilename)
{
	do 
	{
		MSXML2::IXMLDOMDocumentPtr spDoc;
		try
		{
			spDoc = GetDocument();
			if(spDoc!=NULL){
				spDoc->load(_variant_t(lpFilename.c_str()));
			}
		} catch(...) {
			break;
		}
		m_spDocument = spDoc;
		if ( !m_spDocument || !m_spDocument->documentElement )
			break;
		return CyNode(spDoc);
	} while (FALSE);

	// load xml from a file
	ifstream f;
	f.open(lpFilename);
	if(!f.is_open())
		return CyNode();
	f.seekg(0, f.end);
	streamoff size = f.tellg();
	f.seekg(0, f.beg);
	string buffer;
	buffer.resize((size_t)size);
	f.read(&buffer[0], size);
	f.close();
	wstring wsContent;
	wstring_convert<codecvt_utf8_utf16<wchar_t>, wchar_t> convUTF8UTF16;
	wsContent = convUTF8UTF16.from_bytes(buffer);
	return LoadXML(wsContent);
}

CyNode CyDoc::LoadXML(const std::wstring& lpContent)
{
	MSXML2::IXMLDOMDocumentPtr spDoc;
	try
	{
		spDoc = GetDocument();
		if(spDoc!=NULL){
			spDoc->loadXML(lpContent.c_str());
		}
	} catch(...) {
	}
	m_spDocument = spDoc;
	return CyNode(spDoc);
}

wstring CyDoc::GetXml()
{
	wstring strRet = L"";
	try{
		if(m_spDocument != NULL )
		{
			strRet = (LPTSTR)m_spDocument->xml;
		}
	}catch(...){
	}
	return strRet;
}

bool CyDoc::GetUTF8Str(std::string& str)
{
	wstring sRet = L"";
	if(m_spDocument != NULL)
	{
		sRet = (const wchar_t*)m_spDocument->xml;
	}
	if (sRet.length() == 0)
	{
		return false;
	}
	// add encoding "UTF-8"
	wstring key = L"<?xml version=\"1.0\"?>";
	size_t pos = sRet.find(key);
	if (pos != wstring::npos)
	{
		sRet.replace(pos, key.length(),
			L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
	}

	wstring_convert<codecvt_utf8_utf16<wchar_t>, wchar_t> convUTF8UTF16;
	str = convUTF8UTF16.to_bytes(sRet);
	return true;
}

CyNode CyDoc::CreateRootNode(const std::wstring& lpNodeName, bool bCreateHeadRow /*= false*/)
{
	MSXML2::IXMLDOMDocumentPtr spDoc;
	try
	{		
		spDoc = GetDocument();
		if(spDoc){
			if ( bCreateHeadRow )
			{
				// Create a processing instruction targeted for xml.		
				MSXML2::IXMLDOMProcessingInstructionPtr pi;
				pi = spDoc->createProcessingInstruction(L"xml", L"version=\"1.0\" encoding=\"UTF-8\"");
				if (pi != NULL) {
					spDoc->appendChild(pi);
				}
			}
			
			// Create a comment for the document.
// 			MSXML2::IXMLDOMCommentPtr pc;
// 			pc = spDoc->createComment(_T("Generate by junyu application."));
// 			if (pc != NULL) {
// 				spDoc->appendChild(pc);
// 			}
					
			// Create the root element (i.e., the documentElement).
			MSXML2::IXMLDOMElementPtr pe;
			pe = spDoc->createElement(lpNodeName.c_str());		
			if (pe != NULL) {
				spDoc->appendChild(pe);
			}
		}

	} catch(...) {
		spDoc = NULL;
	}
	m_spDocument = spDoc;
	return CyNode(spDoc);
}

BOOL CyDoc::Save(const std::wstring& lpFilename)
{
	BOOL bRet = FALSE;
	try
	{		
		//IXMLDOMDocumentPtr spDoc = node.GetDocumentPtr();
		// not use this method because the saved file is not begin with 
		// byte order 0xFEFF
		//spDoc->save(_variant_t(_T("xml_") + sFilename));

		if(m_spDocument){			

			ofstream file;
			file.open(lpFilename, ofstream::out);
			if(file.is_open()){
				wstring sContent = (LPTSTR) m_spDocument->xml;
				

				// add encoding "UTF-8"
				wstring key = L"<?xml version=\"1.0\"?>";
				size_t pos = sContent.find(key);
				if (pos != wstring::npos)
				{
					sContent.replace(pos, key.length(),
						L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
				}

				// convert unicode to utf8 start
				wstring_convert<codecvt_utf8_utf16<wchar_t>, wchar_t> convUTF8UTF16;
				string buf = convUTF8UTF16.to_bytes(sContent);

				file.write(&buf[0], buf.length() + 1);
				file.close();
				bRet = TRUE;
				
			}	
			
		}
	} catch (...) {
		bRet = FALSE;
	}	
	return bRet;
}

MSXML2::IXMLDOMDocumentPtr CyDoc::GetDocument()
{
	MSXML2::IXMLDOMDocumentPtr spDoc;
	spDoc.CreateInstance(__uuidof(MSXML2::DOMDocument));
	spDoc->async = VARIANT_FALSE;
	spDoc->preserveWhiteSpace = VARIANT_TRUE;
	spDoc->resolveExternals = VARIANT_FALSE;
	return spDoc;
}


//////////////////////////////////////////////////////////////////////
// CyNode
//////////////////////////////////////////////////////////////////////
CyNode::CyNode()
{
	m_nLevel = 1;
}

CyNode::CyNode(CyNode& node)
{	
	m_spDocument = node.GetDocumentPtr();
	m_spNode = node.GetNodePtr();
	m_nLevel = node.m_nLevel;
}

CyNode::CyNode(MSXML2::IXMLDOMDocumentPtr& spDoc)
{
	m_nLevel = 1;
	if(spDoc){
		m_spDocument = spDoc;
		m_spNode = spDoc->documentElement;
	}	
}

CyNode::CyNode(MSXML2::IXMLDOMElementPtr& spThisNode)
{
	m_nLevel = 1;
	if(spThisNode){

		m_spDocument = spThisNode->ownerDocument;
		m_spNode = spThisNode;
	}	
}

CyNode::CyNode(MSXML2::IXMLDOMNodePtr& spThisNode)
{
	m_nLevel = 1;
	if(spThisNode!=NULL && spThisNode->nodeType == MSXML2::NODE_ELEMENT){
		m_spDocument = spThisNode->ownerDocument;
		m_spNode = spThisNode;
	}
}


CyNode::~CyNode()
{
}

std::wstring CyNode::GetNodeName() const
{
	wstring sRet;
	if(m_spNode){
		sRet = (const wchar_t *)m_spNode->nodeName;
	}
	return sRet;
}

CyNode CyNode::CreateSubNode(CyNode& clonenode)
{
	MSXML2::IXMLDOMElementPtr pe;
	if(m_spDocument!=NULL && m_spNode!=NULL && clonenode.m_spNode!=NULL){		
		pe = clonenode.m_spNode->cloneNode(VARIANT_TRUE);
		if (pe != NULL) {
			AppendHeaderText();
			m_spNode->appendChild(pe);
			AppendTailText();
		}
	}
	CyNode node(pe);
	node.m_nLevel = m_nLevel;
	if(m_nLevel<10){
		node.m_nLevel++;
	}
	return node;
}
	
CyNode CyNode::CreateSubNode(const std::wstring& lpName)
{
	MSXML2::IXMLDOMElementPtr pe;
	if(m_spDocument!=NULL && m_spNode!=NULL){	
		pe = m_spDocument->createElement(lpName.c_str());		
		if (pe != NULL) {
			AppendHeaderText();
			m_spNode->appendChild(pe);
			AppendTailText();
		}
	}
	CyNode node(pe);
	node.m_nLevel = m_nLevel;
	if(m_nLevel<10){
		node.m_nLevel++;
	}
	return node;
}

BOOL CyNode::CreateSubNode(const std::wstring& lpName, const std::wstring& lpValue)
{
	CyNode cNode = CreateSubNode(lpName);
	if ( !cNode.IsValid() )
		return FALSE;
	cNode.SetNodeValueStr(lpValue);
	return TRUE;
}

void CyNode::RemoveSubNode(const CyNode& node)
{
	if(m_spDocument!=NULL && m_spNode!=NULL){
		m_spNode->removeChild(node.m_spNode);
	}
}

void CyNode::ReplaceSubNode(const CyNode& dest, const CyNode& src)
{
	MSXML2::IXMLDOMElementPtr pe;
	if(m_spDocument!=NULL && m_spNode!=NULL &&  dest.m_spNode != NULL && src.m_spNode != NULL){
		m_spNode->replaceChild(src.m_spNode, dest.m_spNode);
	}
}

wstring CyNode::GetNodeValueStr() const
{
	wstring sRet;
	if(m_spNode){
		sRet = (const wchar_t *)m_spNode->text;
	}
	return sRet;
}

void CyNode::SetNodeValueStr(const std::wstring& lpValue)
{
	if(m_spNode)
	{
		m_spNode->text = lpValue.c_str();
	}
}

wstring CyNode::GetAttributeStr(const std::wstring& lpAttrName) const
{
	wstring sRet;
	if(HasAttribute(lpAttrName)){
		MSXML2::IXMLDOMNodePtr spAttribute = m_spNode->attributes->getQualifiedItem(lpAttrName.c_str(), _bstr_t());
		if (spAttribute) {
			sRet = (const wchar_t *) spAttribute->text;
		}
	}
	return sRet;
}

void	CyNode::SetAttributeStr(const std::wstring& lpAttrName, const std::wstring& lpValue)
{
	if(m_spNode){	
		if(HasAttribute(lpAttrName)){
			MSXML2::IXMLDOMAttributePtr spAttribute = m_spNode->attributes->getQualifiedItem(lpAttrName.c_str(), _bstr_t());
			if (spAttribute)
			{
				spAttribute->text = lpValue.c_str();
			}
		} else {
			AddAttributeStr(lpAttrName,lpValue);
		}
	}
}

void CyNode::AddAttributeStr(const std::wstring& lpAttrName, const std::wstring& lpValue)
{
	if(m_spDocument!=NULL && m_spNode!=NULL){
		if(!HasAttribute(lpAttrName)){
			MSXML2::IXMLDOMAttributePtr pa;
			pa = m_spDocument->createAttribute(lpAttrName.c_str());
			if (pa != NULL) 
			{
				pa->text = lpValue.c_str();
				m_spNode->attributes->setNamedItem(pa);
			}
		}
	}
}

void CyNode::RemoveAttributeStr(const std::wstring& lpAttrName)
{
	if(m_spNode){	
		m_spNode->attributes->removeNamedItem(lpAttrName.c_str());
	}
}

wstring CyNode::ToXMLString() const
{
	wstring sRet;
	if(m_spNode){
		sRet = (const wchar_t*)m_spNode->xml;
	}
	return sRet;
}


CyNode CyNode::GetSubNode(const std::wstring& lpNodeName) const
{
	MSXML2::IXMLDOMNodePtr spChild;
	if(m_spNode){	
		int nCount = 0;
		for (spChild = m_spNode->firstChild; spChild; spChild = spChild->nextSibling){
			wstring csNodeName = (const wchar_t*)(spChild->nodeName);
			if (csNodeName == lpNodeName){
				break;
			}
		}	
	}
	return CyNode(spChild);
}

wstring CyNode::GetSubNodeValue(const std::wstring& lpNodeName) const
{
	CyNode cNode = GetSubNode(lpNodeName);
	if ( !cNode.IsValid() )
		return wstring(L"");
	return cNode.GetNodeValueStr();
}

BOOL CyNode::GetSubNodeValue(const std::wstring& lpNodeName, OUT std::wstring& strValue) const
{
	CyNode cNode = GetSubNode(lpNodeName);
	if ( !cNode.IsValid() )
		return FALSE;
	strValue = cNode.GetNodeValueStr();
	return TRUE;
}

CyNodeList CyNode::GetSubNodes() const 
{
	MSXML2::IXMLDOMNodeListPtr list;
	if(m_spNode){
		list = m_spNode->GetchildNodes();
	}	
	return CyNodeList(list);	
}

CyNodeList CyNode::GetSubNodesByTag(const std::wstring& lpName) const 
{
	CyNodeList list(m_spNode->GetchildNodes());	
	list.SetTagName(lpName);
	return list;
}


CyNode CyNode::SelectSingleNode(const std::wstring& lpPath) const
{
	CyNode ret;
	if(m_spNode){
		ret = m_spNode->selectSingleNode(lpPath.c_str());
	}	
	return ret;
}

wstring CyNode::GetSingleNodeValue(const std::wstring& lpPath) const
{
	CyNode cNode = SelectSingleNode(lpPath);
	if ( !cNode.IsValid() )
		return wstring(L"");
	return cNode.GetNodeValueStr();
}

BOOL CyNode::GetSingleNodeValue(const std::wstring& lpPath, OUT std::wstring& strValue) const
{
	CyNode cNode = SelectSingleNode(lpPath);
	if ( !cNode.IsValid() )
		return FALSE;
	strValue = cNode.GetNodeValueStr();
	return TRUE;
}

CyNodeList CyNode::SelectNodes(const std::wstring& lpPath) const
{
	CyNodeList ret(m_spNode->selectNodes(lpPath.c_str()));
	return ret;
}

void CyNode::AppendHeaderText()
{
// 	if(m_spDocument!=NULL && m_spNode!=NULL){	
// 		CString strText;
// 		strText = _T("\r\n");
// 		for(int i=0;i<m_nLevel;i++){
// 			strText += _T("\t");
// 		}
// 		m_spNode->appendChild(m_spDocument->createTextNode( _bstr_t(strText)));
// 	}
}
void CyNode::AppendTailText()
{
// 	if(m_spDocument!=NULL && m_spNode!=NULL){
// 		m_spNode->appendChild(m_spDocument->createTextNode(_T("\r\n")));
// 	}	
}

BOOL CyNode::HasAttribute(const std::wstring& lpName) const 
{
	BOOL bRet = FALSE;
	if(m_spNode){
		bRet = m_spNode->attributes->getQualifiedItem(lpName.c_str(), _bstr_t()) ? 1 : 0;
	}
	return bRet;
}

int CyNode::GetSubNodesCount() const 
{
	int nCount = 0;	
	if(m_spNode){	
		for (MSXML2::IXMLDOMNodePtr spChild = m_spNode->firstChild; spChild; spChild = spChild->nextSibling){
			if (spChild->nodeType == MSXML2::NODE_ELEMENT){
				nCount++;
			}
		}
	}
	return nCount;
}

int CyNode::GetSubNodesCountByTag(const std::wstring& lpName) const 
{
	int nCount = 0;	
	if(m_spNode){	
		for (MSXML2::IXMLDOMNodePtr spChild = m_spNode->firstChild; spChild; spChild = spChild->nextSibling){
			if ((lpName == (const wchar_t*)spChild->text) && spChild->nodeType == MSXML2::NODE_ELEMENT){
				nCount++;
			}
		}
	}
	return nCount;
}

//add by lh for close a sub node on 2010-08-19
CyNode CyNode::CloneSubNode(const std::wstring&  nodeName) const
{
	MSXML2::IXMLDOMNodePtr spChild;
	MSXML2::IXMLDOMNodePtr spCloneChild;
	if(m_spNode){	
		int nCount = 0;
		for (spChild = m_spNode->firstChild; spChild; spChild = spChild->nextSibling){
			wstring csNodeName = (const wchar_t*)(spChild->nodeName);
			if (csNodeName == csNodeName){
				spCloneChild = spChild->cloneNode(-1);
				break;
			}
		}	
	}
	return CyNode(spCloneChild);
}

//////////////////////////////////////////////////////////////////////
// CyNodeList
//////////////////////////////////////////////////////////////////////

CyNodeList::CyNodeList(CyNodeList& list)
{
	m_spList = list.GetNodeListPtr();
	m_TagName = list.m_TagName;
	Reset();
}

CyNodeList::CyNodeList(MSXML2::IXMLDOMNodeListPtr& spList)
{
	m_spList = spList;
	Reset();
}

LONG CyNodeList::GetCount()
{
	LONG nRet = 0;
	if(m_spList){	
		for(MSXML2::IXMLDOMNodePtr spNode = m_spList->nextNode(); spNode; spNode = m_spList->nextNode()){
			if(spNode->nodeType == MSXML2::NODE_ELEMENT){
				if(m_TagName.length()>0){
					if(m_TagName == (const wchar_t*)spNode->nodeName){
						nRet ++;
					}
				} else {
					nRet ++;
				}			
			}
		}
	}	
	Reset();
	return nRet;
}

CyNode CyNodeList::Next() const
{
	MSXML2::IXMLDOMNodePtr spNodeRet;
	
	if(m_spList){	
		for(MSXML2::IXMLDOMNodePtr spNode = m_spList->nextNode(); spNode; spNode = m_spList->nextNode()){			
			if(spNode->nodeType == MSXML2::NODE_ELEMENT){
				if(m_TagName.length()>0){
					if(m_TagName == (const wchar_t*)spNode->nodeName){
						spNodeRet = spNode;
						break;
					}
				} else {
					spNodeRet = spNode;
					break;
				}	
				
			}
		}
	}
	return spNodeRet;
}

void CyNodeList::Reset()
{
	if(m_spList){
		m_spList->reset();
	}
}
