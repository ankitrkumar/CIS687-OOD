/////////////////////////////////////////////////////////////////////////////
// Executive.cpp - Uses DataStore, FileManager, FileSystem and Display to  //
//					complete File Catalogging  							   //
// ver 1.0                                                                 //
// ----------------------------------------------------------------------- //
// copyright © Ankit Kumar, 2014                                           //
// All rights granted provided that this notice is retained                //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2010                             //
// Platform:    Asus ROG GL551JM-EH71, Core i7, Windows 8.1 64-bit         //
// Application: OOD Projects, 2014	                                       //
// Author:      Ankit Kumar, Syracuse University						   //
//              (315) 728-8955, akumar15@syr.edu	                       //
/////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <sstream>
#include "../FileSystem/FileSystem.h"
#include "../FileMgr/FileMgr.h"
#include "../DataStore/DataStore.h"
#include "../Display/Display.h"
using namespace std;

set<string> inputParser(string input);

//#ifdef TEST_EXEC

int main(int argc, char* argv[])
{
	string userInputText = "ankit";
	string userInputPatt = "*.*";
	char userReq = 't';
	bool recurse = 0, duplicate = 0, textSearch = 0, run = 0;
	DataStore ds;
	Display dp;
	string inputText;
	string path, pattern;
		if (argc < 2)//
		{
			path = ".";					//default path if no arguments are provided
			path = FileSystem::Path::getFullFileSpec(path);
			pattern = "*.*";			//default pattern, if no arguments are provided
			recurse = 0;
			FileMgr fm(path, ds);
			fm.search(recurse);
			dp.printSummary(ds);
		}
		else {
			path = argv[1];
			path = FileSystem::Path::getFullFileSpec(path);
			if (!FileSystem::Directory::exists(path))				//check if input path is valid or not
			{
				dp.printPathError();
				path = ".";
				path = FileSystem::Path::getFullFileSpec(path);
				recurse = 1;
			}

			FileMgr fm(path, ds);
			for (int i = 2; i < argc; i++)
			{
				string arg = argv[i];
				if (arg == "/s" || arg == "/S")
				{
					recurse = 1;
				}
				else if (arg == "/d" || arg == "/D")
				{
					duplicate = 1;
				}
				else if (arg == "/f" || arg == "/F")
				{
					textSearch = 1;
					string input = argv[++i];
					inputText = input;
				}
				else
				{
					fm.addPattern(arg);
				}
			}//end of for

			fm.search(recurse);
			if (recurse && !duplicate && !textSearch)
				dp.printDataStore(ds);

			if (duplicate && recurse)
				dp.printDuplicates(ds);
			else if (!recurse && duplicate)
				dp.printDuplicatesError();
			else if (!recurse && !duplicate && !textSearch)
				dp.printDataStore(ds);

			if (textSearch)
			{
				set<string> inputSet = fm.textSearch(inputText, ds);		
				dp.printSearchTextFound(inputText, inputSet.size());
				dp.printSearchTextFoundPaths(inputSet);
			}

		}//end of else
		
		do
		{
			FileMgr fm(path, ds);
			cout << "\n\nPress ENTER to Exit OR any key to Continue  ";					//continue to accept input after ds is created
			getline(cin, userInputText);
			if (userInputText.empty())
				break;
			else
			{
			cout << "Enter Search Text: ";
			getline(cin, userInputText);
			cout << "Enter patterns: ";
			getline(cin, userInputPatt);
			set<string> inputSet;
			if (inputParser(userInputPatt).size() > 0)
			{
				if ((string)*inputParser(userInputPatt).begin() == "*.*")
					inputSet = fm.textSearch(userInputText, ds);
				else
					inputSet = fm.textSearch(userInputText, ds, inputParser(userInputPatt));
			}
			else
				inputSet = fm.textSearch(userInputText, ds);

			dp.printSearchTextFound(userInputText, inputSet.size());
			dp.printSearchTextFoundPaths(inputSet);
			}

		} while (1);

		return 0;
}
//----< Parse the input from user>-------------------------
set<string> inputParser(string input)
{
	set<string> inputs;
	stringstream mySstream(input);
	string temp;
	while (getline(mySstream, temp, ' '))
		{
		inputs.insert(temp);
		}
	return inputs;
}
//#endif TEST_EXEC
