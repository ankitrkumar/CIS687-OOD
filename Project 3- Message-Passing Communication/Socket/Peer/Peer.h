#ifndef PEER_H
#define PEER_H
/////////////////////////////////////////////////////////////////////////////
// Peer.h - Provide receiver and Sender capabilities to peers			   //
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
* This module provides peer to peer communication. The peers have a sender 
* and receiver module.
*
* Public Interface:
* =================
* Peer peer(listenPort);
* peer.sendOverThread()
* peer.enQMessage(path, destAdd, destPort);//enqs message into senderQ
* finishSending();	//enqs stop message to terminate sender thread
*
* Required Files:
* ===============
* Sender.h, Sender.cpp,
* Receiver.h, Receiver.cpp
*
* Build Command:
* ==============
* cl /EHa /DTEST_PEER PEER.cpp
*
* Maintenance History:
* ====================
* ver 1.0 : 9 April 2015
* - first release
*/


#include <iostream>
#include "Sender.h"
#include "Receiver.h"
#include <thread>

class Peer{

public:	
	Peer(std::string srcAdd_, size_t srcPort_);

	void sendOverThread();		//initiates peers sender thread
	
	void enQMessage(std::string path, std::string srcAdd, std::string destAdd, size_t destPort);//enqs message into senderQ

	void finishSending();	//enqs stop message to terminate sender thread

private:
	Sender sender;	
	Receiver *recvr;
	void sendThreadFunc();
	int srcPort;
	std::string srcAdd;

};

#endif