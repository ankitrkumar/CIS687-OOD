#ifndef RECEIVER_H
#define RECEIVER_H
/////////////////////////////////////////////////////////////////////////////
// Receiver.h - Receiver class for Peer, receives on listener socket and   //
//				delgates to client handler which works on the request	   //
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
* Receives msg on the listener socket and uses a Client handler to create a 
* worker thread for processing
*
* Public Interface:
* =================
* Receiver(Socket::IpVer ipVer = Socket::IP4, int port=9090);
* Accepts port number and ipversion
* 
* Receiver::getSenderQ();
* To get the recvrQ for the Receiver
*
* Required Files:
* ===============
* Message.h, Message.cpp, BlockingQueue.h,
* ClientHandler.h, ClientHandler.cpp
*
* Build Command:
* ==============
* cl /EHa /DTEST_RECEIVER RECEIVER.cpp
*
* Maintenance History:
* ====================
* ver 1.0 : 9 April 2015
* - first release
*/


#include "../BlockingQueue/BlockingQueue.h"
#include "../Message/Message.h"
#include "ClientHandler.h"
#include "Dispatcher.h"

class Receiver
{
public:
	Receiver(Socket::IpVer ipVer = Socket::IP6, int port = 9000);
	static Message rmRecvrQ();
	static void addRecvrQ(Message msg);
	static BlockingQueue<Message>& getRecvrQ(){ return recvrQ; }

private:
	Dispatcher *dispatcher;
	static BlockingQueue<Message> recvrQ;
	SocketSystem socketSystem;
	SocketListener *socketListener;
	Message msg;
	ClientHandler clientHandler;
	
};

#endif