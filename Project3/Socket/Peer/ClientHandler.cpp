/////////////////////////////////////////////////////////////////////////////
// ClientHandler.cpp-Gets the request on a thread and releases the receiver// 
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


#include "ClientHandler.h"
#include "Receiver.h"

//-----<Client Handler starts by running this method>--------------------------------
void ClientHandler::operator()(Socket& sock)
{
	while (true)
	{
		try
		{
			Message msg = getMsgHeader(sock);
			msg.createMessage();
			if (msg.getCmd()== "SEND_ACK")
			{
				string s = "\n  Acknowledgement received from IP: " + msg.getSrcAdd() + " and port:" + to_string(msg.getSrcPort());
				Verbose::show(s);
			}			
			//For strings alone
			else if (msg.getCmd() == "SEND_STRING")
				ApplicationHelpers::Verbose::show("\nString received: " + msg.getCmd() + "\n");

			else if (msg.getCmd() == "UPLOAD_FILE")
			{
				if (!getFile(msg, sock))
					ApplicationHelpers::Verbose::show("\nFile upload failed\n");
			}
			else
				break;
		}
		catch (std::exception ex)
		{
			break;
		}
	}
	try{
		sock.shutDown();
		sock.close();
	}
	catch (std::exception ex)
	{
		ApplicationHelpers::Verbose::show("\nClientHandler connection close error"+ (std::string)ex.what() + "\n");
	}
}
//-----<Method to get and parse the reeived message from the socket>--------------------------------
Message ClientHandler::getMsgHeader(Socket& sock)
{
	std::string messageHeader = sock.recvString('\n');
	Message msg = msg.parseHeader(messageHeader);
	msg.createMessage();
	return msg;
}

//-----<Method to get complete File from the socket>--------------------------------
bool ClientHandler::getFile(Message msg, Socket& sock)
{
	std::string opDirectory = "./DestFolder/";
	opDirectory=FileSystem::Path::getFullFileSpec(opDirectory);
	std::ofstream opFile(opDirectory + msg.getFileName(), std::ios::binary);
	size_t bufferContent = 100;
	Socket::byte buffer[100];
	bufferContent = msg.getContLength();
	bool ok;
	if (sock.bytesWaiting() == 0)
		return false;
	while (true)
	{
		ok = sock.recv(bufferContent, buffer);
		if (sock == INVALID_SOCKET || !ok)
			return false;
		
		opFile.write(buffer, bufferContent);

		if (bufferContent < 100)
		{
			opFile.close();
			Receiver::addRecvrQ(msg);
			return true;
		}
		else if (sock.bytesWaiting() != 0)
		{
			msg = getMsgHeader(sock);
			bufferContent = msg.getContLength();
		}
		else
		{
			Receiver::addRecvrQ(msg);
			return true;
		}
	}
	return false;
}

#ifdef TEST_CLIENTHANDLER

int main()
{
	return 0;
	//No test_stub for client handler tested with Receiver
}

#endif
