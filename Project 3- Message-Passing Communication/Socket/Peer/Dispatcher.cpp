/////////////////////////////////////////////////////////////////////////////
// Dispatcher.cpp - Perform dispatch function depending on the received	   //
//					command.											   //
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

#include "Dispatcher.h"
#include "Receiver.h"
#include "Sender.h"

void Dispatcher::start()
{
	thread dispatcherT(&Dispatcher::dispatcherFunc,this);
	dispatcherT.detach();
}

void Dispatcher::dispatcherFunc()
{
	while (true)
	{
		try
		{
			Message msg = Receiver::rmRecvrQ();
			if (msg.getCmd() == "CLOSE_DISPATCHER")
				break;
			else
				fileUpload(msg);
		}
		catch (exception ex)
		{
			cout << "\n In Dispatcher Thread: " << ex.what() << endl;
		}
	}
}
void fileUpload(Message msg)
{
	ApplicationHelpers::Verbose::show("\nMessage from Dispatcher--File "+msg.getFileName()+" received!");
	Message msg_("SEND_ACK", msg.getFileName(), msg.getSrcAdd(), msg.getSrcPort(), msg.getDestAdd(), msg.getDestPort());
	msg_.createMessage();
	Sender::addSenderQ(msg_);
}

#ifdef TEST_DISPATCHER

int main()
{
	return 0;
}

#endif