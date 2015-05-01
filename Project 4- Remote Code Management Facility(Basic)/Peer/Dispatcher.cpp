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
			Message msg = rBQ.deQ();
			/*if (msg.getCmd() == "CLOSE_DISPATCHER")
				break;
			else*/ 
			if(msg.getCmd() == "FU_COMPLETE")
				fileUpload(msg);
			if(msg.getCmd() == "FILE_LIST_S")
				fileListSend(msg);
			if (msg.getCmd() == "DL_FILE_S")
				fileDlSuccess(msg);
			if (msg.getCmd() == "FILE_SEARCH_S")
				fileSearchSuccess(msg);
			if (msg.getCmd() == "TEXT_SEARCH_S")
				textSearchSuccess(msg);
			if (msg.getCmd() == "UPLOAD_FILE")
				sendFile(msg);
		}
		catch (exception ex)
		{
			cout << "\n In Dispatcher Thread: " << ex.what() << endl;
		}
	}
}
void Dispatcher::fileUpload(Message msg)
{
	ApplicationHelpers::Verbose::show("\nMessage from Dispatcher--File "+msg.getFileName()+" received!");
	Message msg_("FILE_SENT", msg.getFileName(), msg.getDestAdd(), msg.getDestPort(), msg.getSrcAdd(), msg.getSrcPort());
	msg_.createMessage();
	sBQ.enQ(msg_);
}

void Dispatcher::fileListSend(Message msg)
{
		Message m("FILE_LIST_S", msg.getFileName(), msg.getDestAdd(), msg.getDestPort(), msg.getSrcAdd(), msg.getSrcPort());
		m.createMessage();
		sBQ.enQ(m);
}

void Dispatcher::fileDlSuccess(Message msg)
{
	Message m("DL_FILE_S", msg.getFileName(), msg.getSrcAdd(), msg.getSrcPort(), msg.getDestAdd(), msg.getDestPort());
	m.createMessage();
	sBQ.enQ(m);
}

void Dispatcher::fileSearchSuccess(Message msg)
{
	Message m("FILE_SEARCH_S", msg.getFileName(), msg.getSrcAdd(), msg.getSrcPort(), msg.getDestAdd(), msg.getDestPort());
	m.createMessage();
	sBQ.enQ(m);
}

void Dispatcher::textSearchSuccess(Message msg)
{
	Message m("TEXT_SEARCH_S", msg.getFileName(), msg.getSrcAdd(), msg.getSrcPort(), msg.getDestAdd(), msg.getDestPort());
	m.createMessage();
	sBQ.enQ(m);
}

void Dispatcher::sendFile(Message msg)
{
	sBQ.enQ(msg);
	Message m("DL_FILE_S", msg.getFileName(), msg.getDestAdd(), msg.getDestPort(), msg.getSrcAdd(), msg.getSrcPort());
	m.createMessage();
	sBQ.enQ(m);
}
#ifdef TEST_DISPATCHER

int main()
{
	return 0;
}

#endif