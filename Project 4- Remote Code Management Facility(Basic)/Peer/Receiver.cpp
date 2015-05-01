/////////////////////////////////////////////////////////////////////////////
// Receiver.cpp - Receiver class for Peer, receives on listener socket and //
//				  delgates to client handler which works on the request	   //
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

#include "Receiver.h"
#include "../ApplicationHelpers/AppHelpers.h"

//-----<Parametrized constructor>--------------------------------
Receiver::Receiver(Socket::IpVer ipVer, int portNo) : clientHandler(recvrQ)
{
	socketListener = new SocketListener(portNo, ipVer);
	socketListener->start(clientHandler);
}

void Receiver::listen(int port)
{
	socketListener = new SocketListener(port, Socket::IP4);
}
//connect to the port
void Receiver::connect()
{
	try{
		//Verbose v(true);
		socketListener->start(clientHandler);
	}
	catch (std::exception ex){
	}
}


//-----<Wrapper to remove from Receiver queue>----------------------------
Message Receiver::rmRecvrQ()
{
	Message msg;
	msg = recvrQ.deQ();
	return msg;
}

//-----<Wrapper to add to Receiver queue>----------------------------
void Receiver::addRecvrQ(Message msg)
{
	getRecvrQ().enQ(msg);
}


#ifdef TEST_RECEIVER

int main()
{
	return 0;
	//No test_stub for Receiver. Tested with peer to peer communication
}

#endif