#ifndef DISPLAY_H
#define DISPLAY_H
/////////////////////////////////////////////////////////////////////////////
// Display.h - Display user prompts and output				           //
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
/*
* Module Operations:
* ==================
* This module provides the Display class wthich supports, printing out
* duplicate files, summary and prompt for user, by simply providing the
* output datastore as an input to most of the print methods
*
* Public Interface:
* =================
* Display d;
* d.printline(50);
* d.printSummary(store);
*
* d.printDataStore(store);
* d.printDuplicates(store);
*
* d.printPathError();
* d.printDuplicatesError();
* d.printSearchTextFoundPaths(inputSet);
* d.printSearchTextFound(inputText , size);
*
* Required Files:
* ===============
* Display.cpp Display.h
*
* Build Command:
* ==============
* cl /EHa /DTEST_DISPLAY Display.cpp
*
* Maintenance History:
* ====================
* ver 1.0 : 10 Feb 15
* - first release
*/

# include "../DataStore/DataStore.h"
using namespace std;

class Display{
public:

	void printline(int number);
	void printSummary(DataStore& ds);

	void printDataStore(DataStore& ds);
	void printDuplicates(DataStore& store);

	void printPathError();
	void printDuplicatesError();
	void printSearchTextFoundPaths(set<string> inputSet);
	void printSearchTextFound(string inputText , int size);

};
 
#endif DISPLAY_H