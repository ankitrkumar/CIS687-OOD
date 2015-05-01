/////////////////////////////////////////////////////////////////////////////
// Sender.cpp - Sender class											   //
// ver 1.0                                                                 //
// ----------------------------------------------------------------------- //
// copyright © Ankit Kumar, 2015                                           //
// All rights granted provided that this notice is retained                //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2013                             //
// Platform:    Asus ROG GL551JM-EH71, Core i7, Windows 8.1 64-bit         //
// Application: OOD Projects, 2015	                                       //
// Author:      Ankit Kumar, Syracuse University						   //
//              (315) 728-8955, akumar15@syr.edu	                       //
/////////////////////////////////////////////////////////////////////////////

#include "Sender.h"

//BlockingQueue<Message> Sender::senderQ;

//-----<Wrapper to remove from Sender queue>----------------------------
Message Sender::rmSenderQ()
{
	Message msg;
	msg = senderQ.deQ();
	return msg;
}

//-----<Wrapper to add to Sender queue>----------------------------
void Sender::addSenderQ(Message msg)
{
	getSenderQ().enQ(msg);
}

//-----<Connect to peer using given ip and port>--------------------------------
bool Sender::connectPeer(std::string ip, size_t port)
{
	int i = 0;
	while (!socketConnector.connect(ip, (int)port))
	{
		if (i > 5)
			return false;
		::Sleep(100);
		i++;
	}
	return true;
}

//-----<Convert message to a string to send it across Sender Thread>--------------------------------
void Sender::parseAndSendHeader(Message msgSend)
{
	std::string strMessage;
	std::unordered_map<std::string, std::string> msgHeader = msgSend.getMsgAttribs();
	for (auto att : msgHeader)
	{
		strMessage += att.first + "=" + att.second+"--";
	}
	socketConnector.sendString(strMessage,'\n');
}

//-----<Send File over the Sender thread>--------------------------------
bool Sender::sendFile(Message msg)
{
	std::string filename = msg.getFileName();
	FileSystem::FileInfo fn(filename);
	if (fn.good())
		filename = fn.name();
	else
	{
		return false;
	}
	Socket::byte buffer[1000];
	std::streamoff bytesRead;
	std::ifstream file(msg.getFileName(), std::ios::in | std::ios::binary);
	if (file.is_open())
	{
		file.seekg(0, std::ios::end);
		std::streamoff remaining = file.tellg();
		file.seekg(0, std::ios::beg);
		while (file.good())
		{
			file.read(buffer, min(1000, remaining));
			bytesRead = min(1000, remaining);
			msg.setContentLength((size_t)bytesRead);
			msg.setFileName(filename);
			msg.createMessage();
			std::unordered_map<std::string, std::string> map = msg.getMsgAttribs();
			parseAndSendHeader(msg);
			socketConnector.send((size_t)bytesRead, buffer);
			remaining -= bytesRead;
			if (bytesRead < 1000)
				break;
		}
		file.close();
		return true;
	}
	return false;
}
//-----<test_stub>--------------------------------
#ifdef TEST_SENDER
int main()
{
	socketConnector.connectPeer("localhost",9090);		
	Sender::getSenderQ();
	Message msg;
	parseAndSendHeader(msg);
	//Sender sender;
	//sender.sendFile(msg);
}

#endif