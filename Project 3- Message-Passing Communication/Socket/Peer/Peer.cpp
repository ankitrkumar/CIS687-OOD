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

//-----<Parametrized constructor>--------------------------------
Peer::Peer(std::string srcAdd_, size_t srcPort_)
{
	this->srcPort = srcPort_;
	this->srcAdd = srcAdd_;
	sender = new Sender();
	recvr = new Receiver(Socket::IP4, srcPort);
	dispatcher = new Dispatcher(sender->getSenderQ(), recvr->getRecvrQ());
	dispatcher->start();
	::Sleep(1000);
	ApplicationHelpers::Verbose::show("\n Receiver initiated at port number: "+std::to_string(srcPort));
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
			else if (msg.getCmd() == "SEND_ACK")
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
			Peer peer_(srcAdd, std::stoi(srcPort));
			peer_.enQMessage(srcAdd, srcFile, destAdd, std::stoi(destPort));
			peer_.sendOverThread();
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