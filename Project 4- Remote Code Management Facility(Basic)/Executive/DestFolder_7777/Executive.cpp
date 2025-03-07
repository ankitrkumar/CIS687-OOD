/////////////////////////////////////////////////////////////////////////////
// Executive.cpp - Executive Module for Socket Communication for Messages  //
// ver 1.0                                                                 //
// ----------------------------------------------------------------------- //
// copyright � Ankit Kumar, 2014                                           //
// All rights granted provided that this notice is retained                //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2013                             //
// Platform:    Asus ROG GL551JM-EH71, Core i7, Windows 8.1 64-bit         //
// Application: OOD Projects, 2015	                                       //
// Author:      Ankit Kumar, Syracuse University						   //
//              (315) 728-8955, akumar15@syr.edu	                       //
/////////////////////////////////////////////////////////////////////////////

#ifdef TEST_EXECUTIVE

#include "../Peer/Peer.h"


int main(int argc, char** args)
{

	ApplicationHelpers::Verbose v(true);
	if (argc<4)
	{
		ApplicationHelpers::Verbose::show("Arguments absent\n");
		ApplicationHelpers::Verbose::show("Enter correct arguments\n");
		ApplicationHelpers::Verbose::show("\nExecutive.exe <srcPort> <destAddress> <destPort> <filePath>");
	}
	else
	{
		try
		{
			std::string srcAdd = args[1];
			std::string srcPort = args[2];
			std::string destAdd = args[3];
			std::string destPort = args[4];			
			//std::string srcFile = args[5];
			Peer peer_(srcAdd, std::stoi(srcPort));
			//peer_.enQMessage(srcFile, srcAdd, destAdd, std::stoi(destPort));
			peer_.start2();
//			Sleep(5000);
			peer_.finishSending();
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