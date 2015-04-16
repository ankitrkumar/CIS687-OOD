#ifndef XMLDOCUMENT_H
#define XMLDOCUMENT_H
//////////////////////////////////////////////////////////////////////////////
// XmlDocument.cpp - a container of XmlElement nodes that creates a parse  //
//					 and also provides functionality for adding and 	   //
//					 removing elements and their properties				   //
// ver 1.3                                                                 //
// ----------------------------------------------------------------------- //
// copyright © Ankit Kumar, 2014                                           //
// All rights granted provided that this notice is retained                //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2010                             //
// Platform:    Asus ROG GL551JM-EH71, Core i7, Windows 8.1 64-bit         //
// Application: OOD Projects, 2014	                                       //
// Author:      Ankit Kumar, Syracuse University						   //
//              (315) 728-8955, akumar15@syr.edu	                       //
// Source:		Jim Fawcett, CST 4-187, 443-3948						   //
//              jfawcett@twcny.rr.com									   //
/////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package allows for the reation of a XML parse tree and this will 
* allow us to search for XML element by tag or by id, to add an element,
* or to remove an element or to get a list of all the children of a given element
* it will also facilitate the writing of an XML document.
* It allows the client to read the XML document, programmatically make changes
* to the document and then write the document back.
* Abstract Syntax Trees, defined in this package, are unordered trees with 
* two types of nodes:
*   Terminal nodes     - nodes with no children
*   Non-Terminal nodes - nodes which may have a finite number of children
* They are often used to contain the results of parsing some language.
*lElement, will be used in
* the AST defined in this package.  They are:
*   AbstractXmlElement - base for all the XML Eleme
* The elements defined in the companion package, Xmnt types
*   DocElement         - XML element with children designed to hold prologue, Xml root, and epilogue
*   TaggedElement      - XML element with tag, attributes, child elements
*   TextElement        - XML element with only text, no markup
*   CommentElement     - XML element with comment markup and text
*   ProcInstrElement   - XML element with markup and attributes but no children
*   XmlDeclarElement   - XML declaration element with attributes
*
* Public Interface:
* =================
* XMLDocument doc(file);
* doc.createElem("FileName");
* doc.findElementsByTag("title");	//returns a vector of elements
* doc.findElementsById("CSE681");	//returns a vector of elements
* doc.findElementByTag("title");	//returns the first Element with that tag
* doc.findElementById("CSE681");	//returns the first element with this id
* if(doc.addChildbyTag("title","testTag"))
*		return true;
* if(doc.addChildbyId("CSE681","testTag"))
*		return true;
* if(doc.removeChildbyTag("title"))
*		return true;
* if(doc.removeChildbyId("CSE681"))
*		return true;
* std::vector<std::pair<std::string, std::string>> attribs = getElementAttribs(ptrElement);
* std::vector<std::shared_ptr<AbstractXmlElement>> childresn = getElementChildren(ptrElement);
* sPtr getAST()
* if(writeXML(inputFile))
*		return true;
* if(writeXMLString(inputString))
*		return true;
*
* Required Files:
* ---------------
*   - XmlDocument.h, XmlDocument.cpp, 
*     XmlElement.h, XmlElement.cpp
*
* Build Process:
* --------------
*   devenv AST.sln /debug rebuild
*
* Maintenance History:
* --------------------
* ver 1.3 : 19 Mar 15
* - addChildBy Tag, adChildById ethods added
* - findById and findByTag implemented
* - add and remove children of a given element using tag or id
* - get the attributes or children of a given element
* - helper function for different elements o create a tree are written
* ver 1.2 : 21 Feb 15
* - modified these comments
* ver 1.1 : 14 Feb 15
* - minor changes to comments, method arguments
* Ver 1.0 : 11 Feb 15		
* - first release
*
*/

#include <memory>
#include <string>
#include <stack>
#include "../XmlElement/XmlElement.h"
#include "../XMLParsing/XmlElementParts/Tokenizer.h"
#include "../XMLParsing/XmlElementParts/xmlElementParts.h"

namespace XmlProcessing
{
  class XmlDocument
  {
  public:
    using sPtr = std::shared_ptr < AbstractXmlElement > ;
    enum sourceType { string, filename };
	
    XmlDocument(const std::string& src, sourceType srcType=filename);
	XmlDocument();
	XmlDocument& operator=(XmlDocument&& s);
	XmlDocument(XmlDocument&& s);

	void createElem(std::string input, sourceType srcType);
	std::vector<std::shared_ptr<AbstractXmlElement>>findElementsByTag(std::string tag);
	std::vector<std::shared_ptr<AbstractXmlElement>>findElementsById(std::string id);
	std::shared_ptr<AbstractXmlElement>findElementByTag(std::string tag);
	std::shared_ptr<AbstractXmlElement>findElementById(std::string id);
	bool addChildbyTag(std::string tag, std::string child);
	bool addChildbyId(std::string id, std::string child);
	bool removeChildbyTag(std::string tag);
	bool removeChildbyId(std::string id);

	void addRootElem(std::string elem);

	std::vector<std::pair<std::string, std::string>> getElementAttribs(std::shared_ptr<AbstractXmlElement> element);
	std::vector<std::shared_ptr<AbstractXmlElement>> getElementChildren(std::shared_ptr<AbstractXmlElement> element);
	sPtr getAST(){ return pDocElement_; }
	bool writeXML(std::string fileName);
	bool writeXMLString(std::string xmlString);

  private:
    sPtr pDocElement_;         // AST that holds procInstr, comments, XML root, and more comments
    std::vector<sPtr> found_;  // query results

	std::string extractValue(std::string token);
	std::string body(std::string input);
	void treeHelper(std::stack <sPtr>& NodeStack_, sPtr& element);
	void treeHelperforText(std::stack <sPtr>& NodeStack_, sPtr& element);
	void treeHelperNodeStack(std::stack <sPtr>& NodeStack_);

	std::shared_ptr <AbstractXmlElement> commentIdentify(XmlParts& parts_);
	std::shared_ptr <AbstractXmlElement> tagIdentify(XmlParts& parts_);
	std::shared_ptr <AbstractXmlElement> textIdentify(XmlParts& parts_);
	std::shared_ptr <AbstractXmlElement> procInstrIdentify(XmlParts& parts_);
	std::shared_ptr <AbstractXmlElement> declarIdentify(XmlParts& parts_);
	
	void createStackforTree(XmlParts& parts_, std::stack <sPtr>& NodeStack_, sPtr& comment_, sPtr& tagged_);//private method to create a tree
  };
}
#endif
