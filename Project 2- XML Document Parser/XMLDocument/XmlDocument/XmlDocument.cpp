/////////////////////////////////////////////////////////////////////////////
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

#include <iostream>
#include <ctype.h>
#include <stack>
#include <algorithm>
#include <fstream>
#include "XmlDocument.h"
#include "../Display/Display.h"

using namespace XmlProcessing;

//------<void Constructor>------------------------
XmlProcessing::XmlDocument::XmlDocument() : found_(std::vector<sPtr>())
{
	sPtr pDoc = makeDocElement(nullptr);
	pDocElement_ = pDoc;
}

//------<parametrized Constructor>------------------------
XmlProcessing::XmlDocument::XmlDocument(const std::string& src, sourceType srcType)
{
	createElem(src, srcType);
}

//------<move Constructor>------------------------
XmlProcessing::XmlDocument::XmlDocument(XmlDocument&& s) : pDocElement_(s.pDocElement_), found_(s.found_)
{
	s.pDocElement_ = nullptr;  // must do this to avoid delete in s dtor
	for (size_t i = 0; i < found_.size(); i++)
		s.found_[i] = nullptr;
};
//------<move assignment>--------------------------
XmlDocument& XmlProcessing::XmlDocument::operator=(XmlDocument&& s) 
{
	if (this == &s) 
		return *this;
	for (size_t i = 0; i < found_.size(); i++)
	{
		found_[i] = nullptr;
	}
	found_.clear();
	pDocElement_ = nullptr;
	found_ = s.found_;
	pDocElement_ = s.pDocElement_;
	s.pDocElement_ = nullptr;  // must do this to avoid delete in s dtor
	for (size_t i = 0; i < found_.size(); i++)
		s.found_[i] = nullptr;
	s.found_.clear();
	return *this;
}

//------<Identify type of Element and create tree>------------------------
void XmlProcessing::XmlDocument::createElem(const std::string input, sourceType srcType){
	DocElement doc(pDocElement_);
	try{
		sPtr declare, comment, tagged, text, procinstr; bool typeO = false;
		if (srcType == string) typeO = false;
		else typeO = true;
		Toker toker(input, typeO);
		toker.setMode(Toker::xml);
		XmlParts parts(&toker);
		std::stack <sPtr> NodeStack;
		while (parts.get()){
			if (parts[0] == "<" && parts[1] == "?" && parts[2] == "xml" ){// This is for declaration tag
				if (parts[3] != "-"){
					declare = declarIdentify(parts);
					pDocElement_ = makeDocElement(declare);
				}
				else{
				procinstr = procInstrIdentify(parts);
				pDocElement_->addChild(procinstr);
				}
			}
			else if (parts[0] == "<" && parts[1] == "?"){//change this logic, it will only work for procinstr that start with xml, others will fail
				procinstr = procInstrIdentify(parts);
				pDocElement_->addChild(procinstr);
			}
			else  if (parts[0] == "<" && parts[1] == "!"){// This is for comment			
				comment = commentIdentify(parts);
				treeHelper(NodeStack, comment);
				createStackforTree(parts, NodeStack, comment, tagged);
			}
			else if (parts[0] == "<" && parts[1] != "/"){// This is for a regular tag
				tagged = tagIdentify(parts);
				treeHelper(NodeStack, tagged);
				createStackforTree(parts, NodeStack, comment, tagged);
			}
			else if (parts[0] != "<"){// This is for text element
				text = textIdentify(parts);
				treeHelperforText(NodeStack, text);
			}
			else if (parts[0] == "<" && parts[1] == "/")
				treeHelperNodeStack(NodeStack);
		}
	}
	catch (std::exception ex){
		std::cout << "\n  " << ex.what() << "\n\n";
	}
}
//------<Node Stack pop for tree>------------------------
void XmlDocument::treeHelperNodeStack(std::stack <sPtr>& NodeStack_)
{
	if (!NodeStack_.empty())
		NodeStack_.pop();
}

//----<helper for creating tree>------------------------------
void XmlDocument::treeHelper(std::stack <sPtr>& NodeStack_, sPtr& element)
{
	if (!NodeStack_.empty())
		NodeStack_.top()->addChild(element);
	else if (pDocElement_)
		pDocElement_->addChild(element);
}

//----<helper for creating tree, terminal node, text>------------------------------
void XmlDocument::treeHelperforText(std::stack <sPtr>& NodeStack_, sPtr& element)
{
	if (!NodeStack_.empty() && pDocElement_)
		NodeStack_.top()->addChild(element);
}
//--------<Create a stack to maintain the structire of XML file>---------------
void XmlDocument::createStackforTree(XmlParts& parts_, std::stack <sPtr>& NodeStack_, sPtr& comment_, sPtr& tagged_)
{
if (parts_[0] == "<" && parts_[1] != "/" && parts_[1] != "?") 
{// To create a stack
	if (parts_[1] == "!")
		NodeStack_.push(comment_);
	else
		NodeStack_.push(tagged_);
}
if (parts_.length()>3 && (parts_[1] == "/" || parts_[parts_.length() - 2] == "--" || parts_[parts_.length() - 2] == "/"))
{// To pop from stack for tree
	if (!NodeStack_.empty())
		NodeStack_.pop();
}
}

//----<procInstr identification >--------------------------------------

std::shared_ptr <AbstractXmlElement> XmlDocument::declarIdentify(XmlParts& parts_)
{
		//std::cout << "declaration" << std::endl;
		sPtr decElem = makeXmlDeclarElement();
		for (int i = 1; i < parts_.length() - 1; ++i)
		{
			if (parts_[i] == "=")
			{
				decElem->addAttrib(parts_[i - 1], extractValue(parts_[i + 1]));
			}
		}
		return decElem;
}

//----<procInstr identification >--------------------------------------

std::shared_ptr <AbstractXmlElement> XmlDocument::procInstrIdentify(XmlParts& parts_)
{
	//std::cout << "procinstrelement" << std::endl;
	
	std::string temp = "";
	int i = 2;
	int pos = parts_.find("=");
	for (i = 2; i <= pos - 2;++i)
		temp += parts_[i];
	sPtr procElem = makeProcInstrElement(temp);
	for (int i = 1; i < parts_.length() - 1; ++i)
	{
		if (parts_[i] == "=")
		{
			procElem->addAttrib(parts_[i - 1], extractValue(parts_[i + 1]));
		}
	}
	return procElem;
}
//----<comment identification >--------------------------------------

std::shared_ptr <AbstractXmlElement> XmlDocument::commentIdentify(XmlParts& parts_)
{
	std::string temp = "";
	
	for (int i = 3; i < parts_.length() - 2; ++i)
	{
		temp += parts_[i] + " ";
	}
	//std::cout << "comment  " << temp << std::endl;
	sPtr commElem = makeCommentElement(temp);
	return commElem;
}

//----<text element identification >--------------------------------------

std::shared_ptr <AbstractXmlElement> XmlDocument::textIdentify(XmlParts& parts_)
{
	//std::cout << "text element" << std::endl;
	
	sPtr textElem = makeTextElement(body(parts_.show()));

	return textElem;
}

//----<tag element identification >--------------------------------------
std::shared_ptr <AbstractXmlElement> XmlDocument::tagIdentify(XmlParts& parts_)
{
	//std::cout << "regular tagged element" << std::endl;
	sPtr tagElem = makeTaggedElement(parts_[1]);;
	for (int i = 1; i < parts_.length() - 1; ++i)
	{
		if (parts_[i] == "=")
		{
			tagElem->addAttrib(parts_[i - 1], extractValue(parts_[i + 1]));
		}
	}
	return tagElem;
}

//--------<DFS search for tag returns a vector>---------------
std::vector<std::shared_ptr<AbstractXmlElement>> XmlDocument::findElementsByTag(std::string tag)
{
	sPtr pNode = getAST();
	std::vector<std::shared_ptr<AbstractXmlElement>> suo;
	std::stack<sPtr> nodeStack;
	nodeStack.push(pNode);
	while (nodeStack.size() > 0)
	{
		sPtr pCurrentNode = nodeStack.top();
		nodeStack.pop();
		for (auto pChild : pCurrentNode->getChild())
		{
			if (pChild!=nullptr && pChild->value() == tag)
			{
				suo.push_back(pChild);
			}
			nodeStack.push(pChild);
		}
	}
	return suo;
}


//--------<DFS search for tag returns first element>---------------
std::shared_ptr<AbstractXmlElement> XmlDocument::findElementByTag(std::string tag)
{
	sPtr pNode = getAST();
	std::stack<sPtr> nodeStack;
	nodeStack.push(pNode);
	while (nodeStack.size() > 0)
	{
		sPtr pCurrentNode = nodeStack.top();
		nodeStack.pop();
		for (auto pChild : pCurrentNode->getChild())
		{
			if (pChild->value() == tag)
			{
				return pChild;
			}
			nodeStack.push(pChild);
		}
	}
	return nullptr;
}

//--------<DFS search for id>---------------------------------------
std::vector<std::shared_ptr<AbstractXmlElement>> XmlDocument::findElementsById(std::string id)
{	
	sPtr pNode = getAST();
	std::vector<std::shared_ptr<AbstractXmlElement>> suo;
	std::stack<sPtr> nodeStack;
	nodeStack.push(pNode);
	while (nodeStack.size() > 0)
	{
		sPtr pCurrentNode = nodeStack.top();
		if (!nodeStack.empty())
			nodeStack.pop();
		if (pCurrentNode != nullptr)
		for (auto pChild : pCurrentNode->getChild())
		{
			std::vector<std::pair<std::string, std::string>> temp;
			if (pChild != nullptr)
			temp = pChild->getAttribs();
			for (auto iter = temp.begin(); iter != temp.end(); ++iter)
			{
				if (iter->second == id && iter->first == "id")
				{
					suo.push_back(pChild);
				}
			}	
			nodeStack.push(pChild);
		}
	}
	return suo;
}

//--------<DFS search for id returns first element>---------------
std::shared_ptr<AbstractXmlElement> XmlDocument::findElementById(std::string id)
{
	sPtr pNode = getAST();
	std::stack<sPtr> nodeStack;
	nodeStack.push(pNode);
	while (nodeStack.size() > 0)
	{
		sPtr pCurrentNode = nodeStack.top();
		nodeStack.pop();
		for (auto pChild : pCurrentNode->getChild())
		{
			if (pChild->getAttribs()[0].second == id && pChild->getAttribs()[0].first == "id")
			{
				return pChild;
			}
			nodeStack.push(pChild);
		}
	}
	return nullptr;
}


//-----<Add child to input tag to XML>---------------------------------
bool XmlDocument::addChildbyTag(std::string tag, std::string child)
{
	found_ = findElementsByTag(tag);
	if (found_.size() == 0)
		return false;
	sPtr childPtr = makeTaggedElement(child);
	for (auto pChild : found_)
	{
		pChild->addChild(childPtr);
	}
	return true;
}

//-----<Add child to input ids tag to XML>---------------------------------
bool XmlDocument::addChildbyId(std::string id, std::string child)
{
	found_= findElementsById(id);
	if (found_.size() == 0)
		return false;
	sPtr childPtr = makeTaggedElement(child);
	for (auto pChild : found_)
	{
		pChild->addChild(childPtr);
	}
	return true;
}

//-----<Remove child with input tag from XML>---------------------------------
bool XmlDocument::removeChildbyTag(std::string tag)
{
	found_ = findElementsByTag(tag);
	if (found_.size() == 0)
		return false;
	for (auto pChild : found_)
	{
		std::vector<std::shared_ptr<AbstractXmlElement>> cVector = pChild->getChild();
		for (auto child : cVector)
			pChild->removeChild(child);
	}
	return true;
}


//-----<Remove child with input id tag from XML>---------------------------------
bool XmlDocument::removeChildbyId(std::string id)
{
	found_ = findElementsById(id);
	if (found_.size() == 0)
		return false;
	for (auto pChild : found_)
	{
		std::vector<std::shared_ptr<AbstractXmlElement>> cVector = pChild->getChild();
		for (auto child : cVector)
			pChild->removeChild(child);
	}
	return true;
}

//-----<Expose the Elements attributes>---------------------------------
std::vector<std::pair<std::string, std::string>> XmlDocument::getElementAttribs(std::shared_ptr<AbstractXmlElement> element)
{
	std::vector<std::pair<std::string, std::string>> nullVec;
	if (element != nullptr)
		return element->getAttribs();
	return nullVec;
}

//-----<Expose the Elements children>---------------------------------
std::vector<std::shared_ptr<AbstractXmlElement>> XmlDocument::getElementChildren(std::shared_ptr<AbstractXmlElement> element)
{
	std::vector<std::shared_ptr<AbstractXmlElement>> nullVec;
	if (element!=nullptr)
		return element->getChild();
	return nullVec;
}

//-----<addRoot to empty file>---------------------------------
void XmlDocument::addRootElem(std::string elem)
{
	getAST()->addChild(makeXmlDeclarElement());
	vector<sPtr> declare = getAST()->getChild();
	declare.front()->addAttrib("version", "1.0");
	declare.front()->addAttrib("encoding", "utf-8");
	getAST()->addChild(makeTaggedElement(elem));
}

//-----<Capability to write to XML file>---------------------------------
bool XmlDocument::writeXML(std::string fileName)
{
	try{
		std::ofstream myfile;
		myfile.open(fileName);

		if (myfile.is_open())
		{
			myfile << getAST()->toString();;
			myfile.close();
			return true;
		}
	}
	catch (exception ex)
	{
		cout << ex.what();
	}

	//Do file write here with root->toString()
	return false;
}

//-----<Capability to write to XML string>---------------------------------
bool XmlDocument::writeXMLString(std::string xmlString)
{
	try{
		xmlString = getAST()->toString();
		if (xmlString.size() > 0)
			return true;
	}
	catch (exception ex)
	{
		cout << ex.what();
	}

	//Do file write here with root->toString()
	return false;
}


//----< helper identifies markup chars >------------------------------

bool specialChar(char ch)
{
	bool test = isspace(ch) || /*ch == '/' ||*/ ch == '>' || ch == '<' || ch == '='; //temporarily removed '/', may need to add later
	test = test || ch == '\'' || ch == '"';
	return test;
}

// ----< return body string >------------------------------------------

std::string XmlDocument::body(std::string input)
{
	size_t pos1 = input.find(">");
	size_t pos2 = input.find_last_of("</");
	return input.substr(pos1 + 1, pos2 - pos1 - 2);
}
//-----<Helpr function for getting values from within quotes>----------
std::string XmlDocument::extractValue(std::string token)
{
	std::string temp = "";
	int i = 0;
	while (true)
	{
		if (specialChar(token[++i]))
			break;
		temp += token[i];
	}
	//std::cout << temp;
	return temp;
}

#ifdef TEST_XMLDOCUMENT
int main()
{ 
  //Display dp;
  title("Testing XmlDocument class");
  std::string temp = "../XMLParsing/XmlElementParts/LectureNote.xml";
  std::string temp1 = "../XMLParsing/XmlElementParts/LectureNote1.xml";
  XmlDocument doc(temp);
  std::vector<std::shared_ptr<AbstractXmlElement>> ids_found = doc.findElementsById("Wintellect");
  std::vector<std::shared_ptr<AbstractXmlElement>> tags_found = doc.findElementsByTag("title");
  if (doc.addChildbyTag("note", "hello"))
	  std::cout << doc.getAST()->toString();
  if (doc.addChildbyId("CSE681", "hello"))
	  std::cout << doc.getAST()->toString();
  if (doc.removeChildbyTag("title"))
	  std::cout << "\n\nRemoving by tag \n\n"<<doc.getAST()->toString();
  if (doc.removeChildbyId("Wintellect"))
	  std::cout << "\n\nRemoving by id\n\n" << doc.getAST()->toString();
  std::vector<std::pair<std::string, std::string>> attrVec = doc.getElementAttribs(doc.findElementByTag("note"));
  std::vector<std::shared_ptr<AbstractXmlElement>> childVec = doc.getElementChildren(doc.findElementByTag("LectureNote"));
  std::vector<std::shared_ptr<AbstractXmlElement>>tagVec = doc.findElementsByTag("title");
  for (auto t : tagVec){
	  t->addAttrib("ankits", "kumars");
	  if (t->addAttrib("ankit", "kumar"))
		  std::cout << "\n\n\nankit kumar added";
  }
  std::cout << "\n\nAdded attribs\n\n" << doc.getAST()->toString();
  for (auto t : tagVec){
	  if (t->removeAttrib("ankits"))
		  std::cout << "\n\n\nankits removed";
  }
  std::cout << "\n\nRemoved attribs\n\n" << doc.getAST()->toString();
  if (doc.writeXML(temp1))
	  std::cout << "\n\n XML file written";
  std::cout << "\n\n";
}
#endif