/////////////////////////////////////////////////////////////////////////////
// Message.cpp - Construct and Interpret Messages to be used by Peers      //
// ver 1.0                                                                 //
// ----------------------------------------------------------------------- //
// copyright © Ankit Kumar, 2014                                           //
// All rights granted provided that this notice is retained                //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2013                             //
// Platform:    Asus ROG GL551JM-EH71, Core i7, Windows 8.1 64-bit         //
// Application: OOD Projects, 2015	                                       //
// Author:      Ankit Kumar, Syracuse University						   //
//              (315) 728-8955, akumar15@syr.edu	                       //
/////////////////////////////////////////////////////////////////////////////

#include "Message.h"

//-----<Parametrized constructor>--------------------------------
Message::Message(std::string cmd_, std::string fileName_, std::string srcAdd_, size_t srcPort, std::string destAdd, size_t destPort)
{
	setCmd(cmd_);
	setFileName(fileName_);
	setSrcAdd(srcAdd_);
	setSrcPort(srcPort);
	setDestAdd(destAdd);
	setDestPort(destPort);
	setContentLength(0);
}

//-----<create an unordered map with the given details>--------------------------------
void Message::createMessage()
{
	setAttrib("cmd", getCmd());
	setAttrib("fileName", getFileName());
	setAttrib("srcAdd", getSrcAdd());
	setAttrib("srcPort", std::to_string(getSrcPort()));
	setAttrib("destAdd", getDestAdd());
	setAttrib("destPort", std::to_string(getDestPort()));	
	setAttrib("contLength", std::to_string(getContLength()));
}

//-----<Method to interpret and parse the message once received from the socket>--------------------------------
Message Message::parseHeader(std::string header)
{
	Message msg;
	msg.setCmd(find("cmd=", header));
	msg.setFileName(find("fileName=", header));
	msg.setSrcAdd(find("srcAdd=", header));
	msg.setSrcPort(std::stoi(find("srcPort=", header)));////////////////////////////
	msg.setDestAdd(find("destAdd=", header));
	msg.setDestPort(std::stoi(find("destPort=", header)));////////////////////////
	msg.setContentLength(std::stoi(find("contLength=", header)));////////////////
	return msg;
}

//-----<Helper function for message parser>--------------------------------
std::string Message::find(std::string key, std::string header){
	std::string cmd = "";
	try{
		std::size_t cmd_hdr_start = header.find(key);
		if (cmd_hdr_start != std::string::npos){
			std::size_t cmd_hdr_end = cmd_hdr_start + key.length();
			std::size_t cmd_end = header.find("--", cmd_hdr_end);
			if (cmd_end != std::string::npos){
				cmd = header.substr(cmd_hdr_end, (cmd_end - cmd_hdr_end));		
				return cmd;
			}
		}
	}
	catch (std::exception e){
		std::cout << "\n  " << e.what();
	}
	return cmd;
}

//-----<test_Stub>--------------------------------
#ifdef TEST_MESSAGE

int main()
{
	Message msg("SEND_STOP", "", "", 0, "", 0);
	msg.createMessage();
	std::string header="something=hu--oiki=ikjkj--kd=sjeuej--";
	std::cout <<"\n\n   "<<header;
	Message msg1 = msg.parseHeader(header);
	msg1.createMessage();
}

#endif 