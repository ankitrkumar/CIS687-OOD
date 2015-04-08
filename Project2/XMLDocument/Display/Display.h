#ifndef DISPLAY_H
#define DISPLAY_H
/////////////////////////////////////////////////////////////////////////////
// Display.h - Display user prompts and output				           //
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
/*
* Module Operations:
* ==================
* This module provides the Display class wthich supports, printing out
* duplicate files, summary and prompt for user, by simply providing the
* output datastore as an input to most of the print methods
*
* Public Interface:
* =================
* Display dp;
* dp.printline(50);
* dp.displayXML(doc);
* dp.displaySearchResults(doc,opVec);
* dp.displayAttribs(doc,attrVec);
* dp.printFailure(msg);
* dp.printReq(msg);
*
*
* Required Files:
* ===============
* Display.cpp Display.h
* XmlDocument.cpp, XmlDocument.h
*
* Build Command:
* ==============
* cl /EHa /DTEST_DISPLAY Display.cpp
*
* Maintenance History:
* ====================
* ver 1.0 : 19 Mar 15
* - first release
*/

#include "../XmlDocument/XmlDocument.h"

using namespace std;
using namespace XmlProcessing;
using sPtr = std::shared_ptr < AbstractXmlElement >;
class Display{
public:

	void printline(int number);
	void displayXML(XmlDocument& doc_);
	void displaySearchResults(XmlDocument& doc, vector<sPtr> opVec);
	void displayChildrenSearchResults(XmlDocument& doc, vector<sPtr> opVec);
	void displayAttribs(XmlDocument& doc, vector<std::pair<std::string, std::string>> attrVec);
	void printFailure(string msg);
	void printReq(string msg);
};

#endif DISPLAY_H