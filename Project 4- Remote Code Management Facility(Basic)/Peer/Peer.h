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
* and receiver module. We use this module to generate a sender and receiver
* for the mock channel on ehich we communicate with the UI
*
* Public Interface:
* =================
* Peer peer(listenPort);
* peer.sendOverThread()
* peer.enQMessage(path, destAdd, destPort);//enqs message into senderQ
* finishSending();	//enqs stop message to terminate sender thread
*
* Peer peer_("localhost",8080);
* Peer peer_(pISendr, pIRecvr);
* peer.getSrc(srcAdd_, srcPort_);
* peer_.postMessage(msg);//add to senders Q
* peer_.getMessage();//get from recvrQ
* finishSending();	//enqs stop message to terminate sender thread
* peer_.start2();
* peer_.getdest(destAdd_, destPort_);
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
* ver 1.1 : 28 April 2015
* - Added postMessage
* - Added getMessage
* - Modified the existing constructor and added more parametrized constructor 
*   to ensure server and client capabilities
* ver 1.0 : 9 April 2015
* - first release
*/


#include <iostream>
#include "Sender.h"
#include "Receiver.h"
#include <thread>
#include "../MockChannel/MockChannel.h"
class Peer : public IMockChannel
{
public:	
	Peer(std::string srcAdd_, size_t srcPort_);

	Peer(){}
	Peer(ISendr* pISendr, IRecvr* pIRecvr);
	void getSrc(std::string srcAdd_, size_t srcPort_);
	void getdest(std::string destAdd_, size_t destPort_);

	void enQMessage(std::string path, std::string srcAdd, std::string destAdd, size_t destPort);//enqs message into senderQ
	void postMessage(const Message& msg);
	Message getMessage();
	
	void finishSending();	//enqs stop message to terminate sender thread
	void start2();
	
private:
	Sender *sender;	
	Receiver *recvr;
	Dispatcher *dispatcher;
	void sendThreadFunc();
	int srcPort;
	std::string srcAdd;
	int destPort;
	std::string destAdd;
	void sendOverThread();		//initiates peers sender thread
	bool start();
	bool stop();
	bool initAll();

};

#endif