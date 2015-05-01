#ifndef RECEIVER_H
#define RECEIVER_H
/////////////////////////////////////////////////////////////////////////////
// Receiver.h - Receiver class for Peer, receives on listener socket and   //
//				delgates to client handler which works on the request. Also//
//				provides a client handler to handle incoming requests on a seperate thead//
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
* clienthandler -	gets the request on a thread and releases the receiver
*					from the work so that it can continue to listen to	   
*					incoming messages
*
* Public Interface:
* =================
* Receiver(Socket::IpVer ipVer = Socket::IP4, int port=9090);
* Accepts port number and ipversion
* 
* Receiver::getSenderQ();
* To get the recvrQ for the Receiver
*
*
* bool getfile(msg, socket);
* to get the file specified in the message header
*
* message getmsgheader(socket& socket);
* get the header file specified
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
* ver 1.1 : 29 April 2015
* - Client Handler added to Receiver
* ver 1.0 : 9 April 2015
* - first release
*/


#include "../Sockets/Sockets.h"
#include "../BlockingQueue/BlockingQueue.h"
#include "../Message/Message.h"
#include "Dispatcher.h"
#include "../MockChannel/MockChannel.h"
#include "../FileSystem/FileSystem.h"
#include "../FileMgr/FileMgr.h"
#include "../DataStore/DataStore.h"


class Receiver;
class ClientHandler;

class ClientHandler
{
public:
	ClientHandler(BlockingQueue<Message>& bQ_) : bQ(bQ_){}
	void operator()(Socket& socket_);
	bool getFile(Message msg, Socket& socket);
	Message getMsgHeader(Socket& socket);
	void getFileList(Message msg, Socket& sock);
	bool sendFile(Message msg, Socket& sock);
	void lookupFiles(Message msg, Socket& sock);
	void lookupFileswithText(Message msg, Socket& sock);
private:
	BlockingQueue<Message>& bQ;
};

class Receiver : public IRecvr
{
public:
	Receiver(Socket::IpVer ipVer = Socket::IP6, int port = 9000);
	Message rmRecvrQ();
	void addRecvrQ(Message msg);
	BlockingQueue<Message>& getRecvrQ(){ return recvrQ; }
	void listen(int port);
	void connect();

private:
	BlockingQueue<Message> recvrQ;
	SocketSystem socketSystem;
	SocketListener *socketListener;
	Message msg;
	ClientHandler clientHandler;
	
};


#endif