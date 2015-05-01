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

			if (msg.getCmd() == "FILE_LIST"){
				getFileList(msg, sock);
			}
			if (msg.getCmd() == "FILE_SEARCH"){
				lookupFiles(msg, sock);
			}
			if (msg.getCmd() == "TEXT_SEARCH"){
				lookupFileswithText(msg, sock);
			}
			if (msg.getCmd() == "UPLOAD_FILE"){
				if (!getFile(msg, sock))
					ApplicationHelpers::Verbose::show("\nFile upload failed\n");
			}
			if (msg.getCmd() == "DL_FILE"){
				sendFile(msg, sock);
			}
			if (msg.getCmd() == "DL_FILE_S" || msg.getCmd() == "FILE_LIST_S" || msg.getCmd() == "FILE_SENT"
				|| msg.getCmd() == "FILE_SEARCH_S" || msg.getCmd() == "TEXT_SEARCH_S"){
				bQ.enQ(msg);
			}	
		}
		catch (std::exception ex){
			//break;
		}
	}
	try{
		sock.shutDown();
		sock.close();
	}
	catch (std::exception ex){
		ApplicationHelpers::Verbose::show("\nClientHandler connection close error"+ (std::string)ex.what() + "\n");
	}
}
void ClientHandler::lookupFileswithText(Message msg, Socket& sock)
{
	std::string sl = "\\";
	std::string opDirectory = ".\\DestFolder_" + to_string(msg.getDestPort());
	if (!FileSystem::Directory::exists(opDirectory)){
		FileSystem::Directory::create(opDirectory);
	}
	std::string filePats = msg.getFileName();
	DataStore ds;
	std::string strMessage;
	FileMgr fm(opDirectory, ds);
	std::string delimiter = "!@!";
	size_t pos = 0;
	std::string token;
	std::string inpText;
	while ((pos = filePats.find(delimiter)) != std::string::npos) {
		token = filePats.substr(0, pos);
		fm.addPattern(token);
		filePats.erase(0, pos + delimiter.length());
	}
	inpText = filePats;
	fm.search(true);
	set<string> founds = fm.textSearch(inpText, ds);
	for (set<string>::iterator i = founds.begin(); i != founds.end(); ++i)
			strMessage += *i + "@!@";
	Message msg_("TEXT_SEARCH_S", strMessage, msg.getDestAdd(), msg.getDestPort(), msg.getSrcAdd(), msg.getSrcPort());
	msg_.createMessage();
	bQ.enQ(msg_);

}
void ClientHandler::lookupFiles(Message msg, Socket& sock)
{
	std::string sl = "\\";
	std::string opDirectory = ".\\DestFolder_" + to_string(msg.getDestPort());
	if (!FileSystem::Directory::exists(opDirectory)){
		FileSystem::Directory::create(opDirectory);
	}
	std::string filePats = msg.getFileName();
	DataStore ds;
	std::string strMessage;
	FileMgr fm(opDirectory, ds);
	std::string delimiter = "#";
	size_t pos = 0;
	std::string token;
	while ((pos = filePats.find(delimiter)) != std::string::npos) {
		token = filePats.substr(0, pos);
		fm.addPattern(token);
		filePats.erase(0, pos + delimiter.length());
	}
	fm.search(true);
	for (DataStore::Store::iterator iter = ds.begin(); iter != ds.end(); ++iter)
	{
		for (auto l : iter->second)
			strMessage += l->c_str() + sl + iter->first + "*";
	}
	Message msg_("FILE_SEARCH_S", strMessage, msg.getDestAdd(), msg.getDestPort(), msg.getSrcAdd(), msg.getSrcPort());
	msg_.createMessage();
	bQ.enQ(msg_);

}

//-----<Method to get and parse the reeived message from the socket>--------------------------------
Message ClientHandler::getMsgHeader(Socket& sock)
{
	std::string messageHeader = sock.recvString('\n');
	Message msg = msg.parseHeader(messageHeader);
	msg.createMessage();
	return msg;
}

void ClientHandler::getFileList(Message msg, Socket& sock)
{
	std::string sl = "\\";
	std::string opDirectory = ".\\DestFolder_" + to_string(msg.getDestPort());
	if (!FileSystem::Directory::exists(opDirectory)){
		FileSystem::Directory::create(opDirectory);
	}
	DataStore ds;
	std::string strMessage;
	FileMgr fm(opDirectory, ds);
	fm.search(true);
	
	for (DataStore::Store::iterator iter = ds.begin(); iter != ds.end(); ++iter)
	{
		for (auto l : iter->second)
			strMessage += l->c_str() + sl + iter->first + "*";
	}
	Message msg_("FILE_LIST_S", strMessage, msg.getDestAdd(), msg.getDestPort(), msg.getSrcAdd(), msg.getSrcPort());
	msg_.createMessage();
	
	bQ.enQ(msg_);
}


bool ClientHandler::sendFile(Message msg, Socket& sock)
{
	Message m("UPLOAD_FILE", msg.getFileName(), msg.getDestAdd(), msg.getDestPort(), msg.getSrcAdd(), msg.getSrcPort());
	m.createMessage();
	bQ.enQ(m);
	return true;
}


//-----<Method to get complete File from the socket>--------------------------------
bool ClientHandler::getFile(Message msg, Socket& sock)
{
	std::string sl = "\\";
	std::string opDirectory = ".\\DestFolder_" + to_string(msg.getDestPort())+sl;
	if (!FileSystem::Directory::exists(opDirectory)){
		FileSystem::Directory::create(opDirectory);
	}
	opDirectory=FileSystem::Path::getFullFileSpec(opDirectory);
	std::ofstream opFile(opDirectory + msg.getFileName(), std::ios::binary);
	size_t bufferContent = 1000;
	Socket::byte buffer[1000];
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

		if (bufferContent < 1000)
		{
			msg.setCmd("FU_COMPLETE");
			msg.createMessage();
			opFile.close();
			bQ.enQ(msg);
			return true;
		}
		else if (sock.bytesWaiting() != 0)
		{
			msg = getMsgHeader(sock);
			bufferContent = msg.getContLength();
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