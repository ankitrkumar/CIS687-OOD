#ifndef MESSAGE_H
#define MESSAGE_H
/////////////////////////////////////////////////////////////////////////////
// Message.h - Construct and Interpret Messages							   //
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
/*
* Module Operations:
* ==================
* Module to create and interpret messages. This helps in parsing incoming
* messages and constructing/creating outgoing messages and provides 
* datastructures to fetch the command and the associatedattribute from the
* message object.
*
* Public Interface:
* =================
* Message msg;
* Message  msg("SEND_STOP", "", "", 0, "", 0);
* msg.createMessage();
* 
* find(std::string key, std::string value);
* -Private function to create and construct a message
* 
* std::string header;
* parseHeader(header);
* 
* getters and setters for all the private members of the class as shown in the header file.
* 
* msg.getMsgAttribs();
* 
* Required Files:
* ===============
* Message.cpp
*
* Build Command:
* ==============
* cl /EHa /DTEST_MESSAGE MESSAGE.cpp
*
* Maintenance History:
* ====================
* ver 1.0 : 9 April 2015
* - first release
*/

#include <iostream>
#include <string>
#include <unordered_map>

class Message{

	using MessageParts = std::string;

private:
	MessageParts cmd;
	MessageParts filePath;
	MessageParts srcAdd;
	size_t srcPort;
	MessageParts destAdd;
	size_t destPort;
	size_t contentLength;
	std::string find(std::string key, std::string value);

public:

	std::unordered_map<std::string, std::string> msgAttribs;
	Message(){};
	void createMessage();
	Message parseHeader(std::string header);
	std::unordered_map<std::string, std::string> getMsgAttribs(){ return msgAttribs; }
	//command, filename, source address, source port, destination address, destination port
	Message(std::string cmd_, std::string fileName_, std::string srcAdd_, size_t srcPort, std::string destAdd, size_t destPort);

	//inline setters and getters
	inline void setAttrib(std::string key, std::string value)
	{
		msgAttribs[key] = value;
	}

	inline MessageParts Message::getCmd()
	{
		return cmd;
	}

	inline MessageParts Message::getFileName()
	{
		return filePath;
	}

	inline MessageParts Message::getSrcAdd()
	{
		return srcAdd;
	}

	inline MessageParts Message::getDestAdd()
	{
		return destAdd;
	}

	inline size_t Message::getSrcPort()
	{
		return srcPort;
	}

	inline size_t Message::getDestPort()
	{
		return destPort;
	}

	inline size_t Message::getContLength()
	{
		return contentLength;
	}
	inline void Message::setCmd(MessageParts cmd_)
	{
		cmd = cmd_;
	}

	inline void Message::setFileName(MessageParts fileName_)
	{
		filePath = fileName_;
	}

	inline void Message::setSrcAdd(MessageParts srcAdd_)
	{
		srcAdd = srcAdd_;
	}

	inline void Message::setDestAdd(MessageParts destAdd_)
	{
		destAdd = destAdd_;
	}

	inline void Message::setSrcPort(size_t srcPort_)
	{
		srcPort = srcPort_;
	}

	inline void Message::setDestPort(size_t destPort_)
	{
		destPort = destPort_;
	}

	inline void Message::setContentLength(size_t contentLength_)
	{
		contentLength = contentLength_;
	}

};

#endif	