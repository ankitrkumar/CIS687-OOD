/////////////////////////////////////////////////////////////////////////////
// Peer.cpp - Provide receiver and Sender capabilities to peers			   //
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
#include "Peer.h"

Peer::Peer(std::string srcAdd_, size_t srcPort_)
{
	this->srcPort = srcPort_;
	this->srcAdd = srcAdd_;
	sender = new Sender();
	recvr = new Receiver(Socket::IP4, srcPort);
	dispatcher = new Dispatcher(sender->getSenderQ(), recvr->getRecvrQ());
	dispatcher->start();
	ApplicationHelpers::Verbose::show("\n Receiver initiated at port number: " + std::to_string(srcPort));
}
//-----<Parametrized constructor>--------------------------------
Peer::Peer(ISendr* pISendr, IRecvr* pIRecvr)
{
	sender = dynamic_cast<Sender*>(pISendr);
	recvr = dynamic_cast<Receiver*>(pIRecvr);
	if (sender == nullptr || recvr  == nullptr)
	{
		std::cout << "\n  failed to start Mock Channel\n\n";
		return;
	}
	std::cout << "\n  started Mock Channel\n\n";
}

//-----<Get src port and address>--------------------------------
void Peer::getSrc(std::string srcAdd_, size_t srcPort_)
{
	this->srcPort = srcPort_;
	this->srcAdd = srcAdd_;
}

//-----<Get dest port and address>--------------------------------
void Peer::getdest(std::string destAdd_, size_t destPort_)
{
	this->destPort = destPort_;
	this->destAdd = destAdd_;
}

//-----<Init sender and receiver>--------------------------------
bool Peer::initAll()
{

	bool o= sender->connectPeer(destAdd,destPort);
	sendOverThread();

	recvr->listen(srcPort);
	recvr->connect();
	
	ApplicationHelpers::Verbose::show("\n Receiver initiated at port number: " + std::to_string(srcPort));
	return o;
}
//-----<start overridden>--------------------------------
bool Peer::start()
{
	return initAll();
}

//-----<start for server>--------------------------------
void Peer::start2()
{
	sendOverThread();

	recvr->listen(srcPort);
	recvr->connect();
	dispatcher = new Dispatcher(sender->getSenderQ(), recvr->getRecvrQ());
	dispatcher->start();
}

//-----<overridden overridden>--------------------------------
Message Peer::getMessage()
{
	return recvr->rmRecvrQ();
}

bool Peer::stop()
{
	return true;
}
//-----<Threading function to send message over thread>--------------------------------
void Peer::sendOverThread()
{
	std::thread sendT(&Peer::sendThreadFunc, this);
	sendT.detach();
}
//-----<Stop message enqueued after the entire file is sent>--------------------------------
void Peer::finishSending()
{
	//Message msg("SEND_STOP", "", "", 0, "", srcPort);
	//msg.createMessage();
	//sender->addSenderQ(msg);
}

//-----<Thread Method::Send the file>--------------------------------
void Peer::sendThreadFunc()
{
	do{
		try{
			Message msg;
			msg = sender->rmSenderQ();
			std::unordered_map<std::string, std::string> msgHeader = msg.getMsgAttribs();
			
			if (msg.getCmd()== "SEND_STOP")
			{
				break;
			}
			else if (msg.getCmd() == "UPLOAD_FILE")
			{
				if (sender->connectPeer(msg.getDestAdd(), (int)msg.getDestPort()))
				{
					if (sender->sendFile(msg))
					{
					//	ApplicationHelpers::Verbose::show("\n Uploading File " + msg.getFileName());
					}
				}
				else
					ApplicationHelpers::Verbose::show("\n Sender connection returned false.");
			}
			else if (msg.getCmd() == "FILE_SENT" || msg.getCmd() == "FILE_LIST" || msg.getCmd() == "FILE_LIST_S"
				|| msg.getCmd() == "DL_FILE_S" || msg.getCmd() == "DL_FILE" || msg.getCmd() == "FILE_SEARCH"
				|| msg.getCmd() == "FILE_SEARCH_S" || msg.getCmd() == "TEXT_SEARCH_S" || msg.getCmd() == "TEXT_SEARCH")
			{
				if (sender->connectPeer(msg.getDestAdd(),(int)msg.getDestPort()))
					sender->parseAndSendHeader(msg);
			}

		}
		catch (std::exception ex)
		{
			ApplicationHelpers::Verbose::show("\n Thread sender function: ");
			ApplicationHelpers::Verbose::show(ex.what());
		}
	} while (true);
}

//-----<Enqueue the msg in the senders queue>--------------------------------

void Peer::enQMessage(std::string filepath, std::string srcAdd,std::string destAdd, size_t destPort)
{
	Message msg("UPLOAD_FILE", filepath, this->srcAdd, srcPort, destAdd, destPort);
	msg.createMessage();
	ApplicationHelpers::Verbose::show("\nSending File: " + filepath + " to " + to_string(destPort));
	sender->addSenderQ(msg);
}

//-----<adds message to senders>--------------------------------
void Peer::postMessage(const Message& msg)
{
	sender->addSenderQ(msg);
}

//-----<test_stub>--------------------------------
#ifdef TEST_PEER

int main(int argc, char** args)
{
	ApplicationHelpers::Verbose v(true);
	if (argc<5)
	{
		ApplicationHelpers::Verbose::show("Arguments absent\n");
	}
	else
	{
		try
		{
			std::string srcAdd = args[1];
			std::string srcPort = args[2];
			std::string destAdd = args[3];
			std::string destPort = args[4];
			std::string srcFile = args[5];
			int i = 0;
			Peer peer_;
			std::cout.flush();
			std::cin.get();
		}
		catch (std::exception& ex)
		{
			ApplicationHelpers::Verbose::show("Exception caught:", true);
			ApplicationHelpers::Verbose::show(std::string("\n  ") + ex.what() + "\n\n");
		}
	}
	return 0;
}

#endif