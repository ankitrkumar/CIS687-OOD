#ifndef SENDER_H
#define SENDER_H
/////////////////////////////////////////////////////////////////////////////
// Sender.h - Sender class to send messages over socket	for its Peer	   //
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
/*
* Module Operations:
* ==================
* This module defines the Sender for the Peer to use to send  messages and 
* content over the communication channel
*
* Public Interface:
* =================
* SockerConnector sc;
* sc.connectPeer("localhost",9090);		
* To connect to peer on given ip and address
* 
* Sender::getSenderQ();
* To get the senderQ for the Sender
* 
* Message msg;
* parseAndSendHeader(msg);
* Parses the header and sends it to the peer at given port
* 
* Sender sender;
* sender.sendFile(msg);
* Sends file block by block to peer at given port number
* 
* Required Files:
* ===============
* FileSystem.h, FileSystem.cpp,Sockets.h,Sockets.cpp
* AppHelpers.h, AppHelpers.cpp, BlockingQueue.h,
* Message.h, Message.cpp
*
* Build Command:
* ==============
* cl /EHa /DTEST_SENDER SENDER.cpp
*
* Maintenance History:
* ====================
* ver 1.0 : 9 April 2015
* - first release
*/

#include <fstream>
#include <iostream>
#include <string>
#include "../ApplicationHelpers/AppHelpers.h"
#include "../Sockets/Sockets.h"
#include "../BlockingQueue/BlockingQueue.h"
#include "../Message/Message.h"
#include "FileSystem.h"

class Sender{

public:
	//Create a thread
	//The thread will pull from blocking queue
	//Need an instance of the blocking queue to add to queue and remove from queue.
	//sender will remove from queue
	Sender(){ };
	bool connectPeer(std::string ip, size_t port);

	static Message rmSenderQ();
	static void addSenderQ(Message msg);
	static BlockingQueue<Message>& getSenderQ(){ return senderQ; }

	void parseAndSendHeader(Message msg);
	bool sendFile(Message msg);

private:
	static BlockingQueue<Message> senderQ;
	SocketSystem socketSystem;
	SocketConnecter socketConnector;
	Message msg;

};

#endif