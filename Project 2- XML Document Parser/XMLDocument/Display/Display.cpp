/////////////////////////////////////////////////////////////////////////////
// Display.cpp - Display user prompts and output				           //
// ver 1.0                                                                 //
// ----------------------------------------------------------------------- //
// copyright © Ankit Kumar, 2014                                           //
// All rights granted provided that this notice is retained                //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2010                             //
// Platform:    Asus ROG GL551JM-EH71, Core i7, Windows 8.1 64-bit         //
// Application: OOD Projects, 2014	                                       //
// Author:      Ankit Kumar, Syracuse University						   //
//              (315) 728-8955, akumar15@syr.edu	                       //
/////////////////////////////////////////////////////////////////////////////


#include "Display.h"
#include "../XmlDocument/XmlDocument.h"

#include <iostream>
#include <string>

using namespace std;

//----< To print given number of lines onto the screen>-------------------------
void Display::printline(int number)
{
	cout << "\n";
	for (int i = 0; i < number; i++)
	{
		cout << "-";
	}
	cout << "\n";
}

//----< TO display the Parse tree>-------------------------
void Display::displayXML(XmlDocument& doc_)
{
	if (doc_.getAST())
	{
		cout << "\n Operation successful, XML tree:: ";
		cout << "\n\n" << doc_.getAST()->toString();
	}
	else
	{
		cout << "\n\n" << "No XML found\n";
	}
	cout << "\n\n";
	printline(50);
}

//----< Displays query to console>-------------------------
void Display::displaySearchResults(XmlDocument& doc, vector<sPtr> opVec)
{
	if (opVec.size()>0)
	{
		cout << "\n Operation successful, elements found ";
		for (auto t : opVec)
		{
			std::cout << "\n" << t->toString();
		}
		cout << "\n\n";
	}
	else
		cout << "\n Operation unsuccessful, elements not found.";
	printline(50);
}

void Display::displayChildrenSearchResults(XmlDocument& doc, vector<sPtr> opVec)
{
	if (opVec.size()>0)
	{
		cout << "\n Children for given input element.\n";
		for (auto t : opVec)
		{
			std::cout << "\n" << t->value();
		}
		cout << "\n Children for given input element, with grandchildren.\n";
		for (auto t : opVec)
		{
			std::cout << "\n" << t->toString();
		}
		cout << "\n\n";
	}
	else
		cout << "\n Operation unsuccessful, elements not found.";
	printline(50);
}


//----< Print failure messages>-------------------------
void Display::printFailure(string msg)
{
	cout << msg;
}

//----< Print requirements messages>-------------------------
void Display::printReq(string msg)
{
	cout << msg;
}

//----< Display the attributes of tag>------------------------
void Display::displayAttribs(XmlDocument& doc, vector<std::pair<std::string, std::string>> attrVec)
{
	if (attrVec.size() > 0)
	{
		std::cout << "\n" << "Name" << "                        " << "Value";
		printline(40);
		for (auto t : attrVec)
		{
			std::cout << "\n" << t.first << "                 " << t.second;
		}
	}
	else
		cout << "\n Operation unsuccessful, No attributes found.";
	printline(50);
}

#ifdef TEST_DISPLAY

int main()
{
	std::string temp = "../XMLParsing/XmlElementParts/LectureNote.xml";
	XmlDocument doc(temp);
	Display dp;
	dp.printline(50);
	
	dp.displayXML(doc);
	
	std::vector<std::shared_ptr<AbstractXmlElement>> ids_found = doc.findElementsById("Wintellect");
	dp.displaySearchResults(doc, ids_found);
	
	std::vector<std::pair<std::string, std::string>> attrVec = doc.getElementAttribs(doc.findElementByTag("note"));
	dp.displayAttribs(doc, attrVec);

	dp.printFailure("\n\nPrint operation failed test");
	dp.printReq("\n\nPrint Requirement number test");
	return 0;
}

#endif TEST_DISPLAY