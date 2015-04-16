/////////////////////////////////////////////////////////////////////////////
// Display.cpp - Display user prompts and output				           //
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


#include "Display.h"
#include "../DataStore/DataStore.h"

#include <iostream>
#include <string>

using namespace std;

//----< print the duplicate files, if they exist >-------------------------
void Display::printDuplicates(DataStore& store)
{
	cout << "\nDisplaying duplicate files.\n\n";
	printline(50);
	cout << "Filename" << "\t \t" << "Paths \n";
	printline(50);
	for(DataStore::iterator mapIt = store.begin(); mapIt != store.end(); ++mapIt)
	{
		if (mapIt->second.size() > 1)
		{
			cout << mapIt->first.c_str(); 
			for (auto listIt :mapIt->second)
				cout << "\t \t" << listIt->c_str() << "\n";
			printline(50);
		}
	}
}

//----< Print Search text found or not>-------------------------
void Display::printSearchTextFound(string inputText, int size)
{
	if (size > 0)
		cout << "\n" << "Input text \"" << inputText << "\" found in, " << endl;
	else
		cout << "\n" << "Input text \"" << inputText << "\" not found in any file\n" << endl;

}

//----< Print Searh text found location>-------------------------
void Display::printSearchTextFoundPaths(set<string> inputSet)
{
	for (set<string>::iterator i = inputSet.begin(); i != inputSet.end(); ++i)
		cout << "\t \t" << *i <<"\n";
}

//----< Print  that input path is invalid>-------------------------
void Display::printPathError()
{
	cout << "\nInput path is invalid. Setting defult path as current directory\n";
}

//----< Prompt user for correct input to display duplicates>-------------------------
void Display::printDuplicatesError()
{
	cout << "\n/s option not provided.\n";
	cout << "\nNo duplicate files can exist in a single directory, use /s option along with /d to get duplicates if they exist.\n\n";
}

//----< Display the datastore>-------------------------
void Display::printDataStore(DataStore& ds)
{
	printline(50);
	cout << "Filename" << "\t \t" << "Paths \n";
	printline(50);	
	for (DataStore::Store::iterator iter = ds.begin(); iter != ds.end(); ++iter)
	{
		cout << iter->first.c_str();
		for (auto l : iter->second)
			cout << "\t \t" << l->c_str() << "\n";
		printline(50);
	}
}

//----< To print given number of lines onto the screen>-------------------------
void Display::printline(int number)
{
	for (int i = 0; i < number; i++)
	{
		cout << "-";
	}
	cout << "\n";
}

//----< Print the summary of datastore>-------------------------
void Display::printSummary(DataStore& ds)
{
	cout << "\nNo options provided. \nNon-recursive search implemented at current directory.\n";
	cout << ds.noOfFiles() << " found in " << ds.noOfDirectories() << " directories\n\n";

}

#ifdef TEST_DISPLAY

int main()
{
	DataStore ds;
	Display d;

	set<string> testSet;

	d.printDataStore(ds);
	d.printDuplicates(ds);
	d.printDuplicatesError();
	d.printline(50);
	d.printPathError();

	d.printSearchTextFoundPaths(testSet);
	d.printSearchTextFound("test", 10);

	return 0;
}

#endif TEST_DISPLAY