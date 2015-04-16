#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H
/////////////////////////////////////////////////////////////////////////////
// ClientHandler.h - Gets the request on a thread and releases the receiver// 
//					from the work so that it can continue to listen to	   //
//					incoming messages									   //
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
* This module creates a worker thread so that the reciever can listen to 
* other incoming messages from other peers
*
* Public Interface:
* =================
* 
* bool getFile(msg, socket);
* To get the file specified in the message header
* 
* Message getMsgHeader(Socket& socket);
* Get the header file specified
* 
* Required Files:
* ===============
* FileSystem.h, FileSystem.cpp,Sockets.h,Sockets.cpp
* Message.h, Message.cpp
*
* Build Command:
* ==============
* cl /EHa /DTEST_CLIENTHANDLER CLIENTHANDLER.cpp
*
* Maintenance History:
* ====================
* ver 1.0 : 9 April 2015
* - first release
*/


#include "../Sockets/Sockets.h"
#include <string>
#include "../Message/Message.h"
#include "FileSystem.h"

class ClientHandler
{
public:
	void operator()(Socket& socket_);				
	bool getFile(Message msg, Socket& socket);	
	Message getMsgHeader(Socket& socket);		

private:

};

#endif