
/////////////////////////////////////////////////////////////////////////////
// Executive.cpp - Executive Module for XML Document Model				   //
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

#include <iostream>
#include "../XmlDocument/XmlDocument.h"
#include "../Display/Display.h"


using namespace std;
using namespace XmlProcessing;
//-----<Demo func for Req 3>---------------------
void demoReq3(Display& dp_, XmlDocument& doc_, std::string& temp_)
{
	dp_.printReq("\n\n\nDemo of Req-3");
	dp_.displayXML(doc_);
}


//-----<Demo func for Req 4>---------------------
void demoReq4(Display& dp_, XmlDocument& doc_)
{
	dp_.printReq("\n\n\nDemo of Req-4\n\n");
	std::string temp = "<?xml version=\"1.0\" encoding=\"utf - 8\"?><? xml - stylesheet type = \"text/css\" href = \"style.css\" ?><!--XML test case --><title><note/><world></world></title>";
	
	XmlDocument doc1(temp, XmlDocument::string);
	dp_.printReq("\n\nObject before use of move constrcutor,\n\n");
	dp_.displayXML(doc1);
	XmlDocument xmld = std::move(doc1);
	dp_.printReq("\nNew Object created using move counstructor\n");
	dp_.displayXML(xmld);
	dp_.printReq("\n\nOLD OBJECT\n\n");
	dp_.displayXML(doc1);
	cin.get();
	dp_.printReq("\n\nObject before use of move assignment,\n\n");
	dp_.displayXML(xmld);
	XmlDocument doc3(temp, XmlDocument::string);
	doc3 = std::move(xmld);
	dp_.printReq("\nNew Object created using move assignment\n");
	dp_.displayXML(doc3);
	dp_.printReq("\n\nOLD OBJECT\n\n");
	dp_.displayXML(xmld);
}

//-----<Demo func for Req 5>---------------------
void demoReq5(Display& dp_, XmlDocument& doc_)
{
	dp_.printReq("\n\n\nDemo of Req-5");
	dp_.printReq("\n\nLooking for Element by id= CSE681");
	std::vector<std::shared_ptr<AbstractXmlElement>> ids_found = doc_.findElementsById("CSE681");
	//can use findElementbyId to return only one element with that id, returns a shared_pointer to that element
	dp_.displaySearchResults(doc_, ids_found);
}

//-----<Demo func for Req 6>---------------------
void demoReq6(Display& dp_, XmlDocument& doc_)
{
	dp_.printReq("\n\n\nDemo of Req-6");
	dp_.printReq("\n\nLooking for Element by tag = title");
	std::vector<std::shared_ptr<AbstractXmlElement>> tags_found = doc_.findElementsByTag("title");
	//can use findElementbyTag to return only one element with that tag, returns a shred_pointer to that element
	dp_.displaySearchResults(doc_, tags_found);

}

//-----<Demo func for Req 7>---------------------
void demoReq7(Display& dp_, XmlDocument& doc_)
{
	XmlDocument doc2;
	dp_.printReq("\n\n\nDemo of Req-7");
	dp_.printReq("\n\nAdding tag = hello to note tag");
	if (doc_.addChildbyTag("note", "hello"))
	{
		dp_.printReq("\nAdding hello tag to note tag");
		dp_.displayXML(doc_);
	}
	else
		dp_.printFailure("\nInput tag not found, element not added.");
	cin.get();
	dp_.printReq("\n\nAdding tag = hello to element with id=CSE681");
	if (doc_.addChildbyId("CSE681", "hello"))
	{
		dp_.printReq("\nAdding hello tag to tag that contains CSE681");
		dp_.displayXML(doc_);
	}
	else
		dp_.printFailure("\nInput id not found, element not added.");
	cin.get();
	dp_.printReq("\n\nRemoving children by tag, removing title tags children \n\n");
	if (doc_.removeChildbyTag("title"))
	{
		dp_.displayXML(doc_);
	}
	else
		dp_.printFailure("\nInput tag not found, element not removed.");
	cin.get();
	dp_.printReq("\n\nRemoving children by id, removing tags of element with id=refId\n\n");
	if (doc_.removeChildbyId("refId"))
	{
		dp_.printReq("\n\nRemoving children by id, removing tag with id refId\n\n");
		dp_.displayXML(doc_);
	}
	else
		dp_.printFailure("\nInput id not found, element not removed.");
	cin.get();
	dp_.printReq("\n\nCreating root element for empty doc element and adding newRootElem Element");
	doc2.addRootElem("newRootElem");
	dp_.printReq("\n\nRoot element addition to create new XML\n\n");
	dp_.displayXML(doc2);
}

//-----<Demo func for Req 8>---------------------
void demoReq8(Display& dp_, XmlDocument& doc_)
{
	dp_.printReq("\n\n\nDemo of Req-8");
	dp_.printReq("\n\nFetching the attriutes for element with tag=note");
	std::vector<std::pair<std::string, std::string>> attrVec = doc_.getElementAttribs(doc_.findElementByTag("note"));
	dp_.displayAttribs(doc_, attrVec);
	dp_.printReq("\n\nFetching the children for element with tag=LectureNote");
	std::vector<std::shared_ptr<AbstractXmlElement>> childVec = doc_.getElementChildren(doc_.findElementByTag("LectureNote"));
	dp_.displayChildrenSearchResults(doc_, childVec);
}

//-----<Demo func for Req 9>---------------------
void demoReq9(Display& dp_, XmlDocument& doc_)
{
	dp_.printReq("\n\n\nDemo of Req-9");
	dp_.printReq("\nAdding attributes to tags with title\n");
	std::vector<std::shared_ptr<AbstractXmlElement>>tagVec = doc_.findElementsByTag("title");

	for (auto t : tagVec)
	{
		if(t->addAttrib("ankits", "kumars"))
		{
			dp_.printReq("\n\n\nankits=kumars added");
		}
		else
		{
			dp_.printReq("\n\n\nankits=kumars not added");
		}
		if (t->addAttrib("ankit", "kumar"))
		{
			dp_.printReq("\n\n\nankit=kumar added");
		}
		else
		{
			dp_.printReq("\n\n\nankit=kumar not added");
		}
	}
	dp_.printReq("\n\nAdded attributes\n\n");
	dp_.displayXML(doc_);
	cin.get();
	dp_.printReq("\n\n\nDemo of Req-9-removal");
	dp_.printReq("\nRemoving attributes to tags with title\n");
	for (auto t : tagVec)
	{
		if (t->removeAttrib("ankits"))
			dp_.printReq("\n\n\nankits removed");
		else
			dp_.printReq("\n\n\nankits not removed");
	}
	dp_.printReq("\n\nRemoved attributes that were found\n\n");
	dp_.displayXML(doc_);
}

//-----<Demo func for Req 10>---------------------
void demoReq10(Display& dp_, XmlDocument& doc_, std::string& temp_)
{
	dp_.printReq("\n\n\nDemo of Req-10");
	if (__argc < 2)
	{
		if (doc_.writeXMLString(temp_))
		{
			dp_.printReq("\n\n XML string written");
			dp_.displayXML(doc_);
		}
		else
			dp_.printReq("XML string not written");
	}
	else
	{
		if (doc_.writeXML("newXML.xml"))
		{
			dp_.printReq("\n\n XML string written to File newXML");
		}
		else
			dp_.printReq("XML file not written");
	}
}

void doDemo(XmlDocument & doc_, Display& dp_, std::string temp)
{
	demoReq3(dp_, doc_, temp);
	cin.get();

	demoReq4(dp_, doc_);
	cin.get();

	demoReq5(dp_, doc_);
	cin.get();

	demoReq6(dp_, doc_);
	cin.get();

	demoReq7(dp_, doc_);
	cin.get();

	demoReq8(dp_, doc_);
	cin.get();

	demoReq9(dp_, doc_);
	cin.get();

	demoReq10(dp_, doc_, temp);
	cin.get();

	std::cout << "\n\n";
}

#ifdef TEST_EXECUTIVE
int main(int argc, char* argv[])
{
	try{
		std::string temp;
		if (argc < 2)
		{
			temp = "<?xml version=\"1.0\" encoding=\"utf - 8\"?><? xml - stylesheet type = \"text/css\" href = \"style.css\" ?><!--XML test case --><LectureNote><title id=\"CSE681\"><note hello=\"world\" /><world></world></title><note id =\"refId\"></LectureNote>";
			Display dp;
			XmlDocument doc(temp, XmlDocument::string);
			doDemo(doc, dp, temp);
		}
		else
		{
			temp = argv[1];
			Display dp;
			XmlDocument doc(temp, XmlDocument::filename);
			doDemo(doc, dp, temp);
		}
	}
	catch (std::exception ex){
		std::cout << "\n  " << ex.what() << "\n\n";
	}
	return 0;
}

#endif
