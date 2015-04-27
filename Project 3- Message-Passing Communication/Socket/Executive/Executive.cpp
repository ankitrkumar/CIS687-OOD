/////////////////////////////////////////////////////////////////////////////
// Executive.cpp - Executive Module for Socket Communication for Messages  //
// ver 1.0                                                                 //
// ----------------------------------------------------------------------- //
// copyright © Ankit Kumar, 2014                                           //
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
#include "../Display/Display.h"

int main(int argc, char** args)
{
	Display dp;
	ApplicationHelpers::Verbose v(true);
	if (argc<5)
	{
		ApplicationHelpers::Verbose::show("Arguments absent\n");
		ApplicationHelpers::Verbose::show("Enter correct arguments\n");
		ApplicationHelpers::Verbose::show("\nExecutive.exe <srcPort> <destAddress> <destPort> <filePath>");
	}
	else
	{
		dp.displayRequirement("\nRequirements that can't be displayed will be found in the Project submission folder.");
		dp.printline(60);
		dp.displayRequirement("\nApplicationHelpers::Verbose is used along with Display for Display package.");
		dp.printline(60);
		dp.displayRequirement("\nRequirements that can be displayed are as follows, 3-5-7,");
		try
		{
			std::string srcAdd = args[1];
			std::string srcPort = args[2];
			std::string destAdd = args[3];
			std::string destPort = args[4];			
			std::string srcFile = args[5];
			Peer peer_(srcAdd, std::stoi(srcPort));
			peer_.enQMessage(srcFile, srcAdd, destAdd, std::stoi(destPort));
			peer_.sendOverThread();
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