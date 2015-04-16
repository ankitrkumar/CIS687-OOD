#ifndef DISPLAY_H
#define DISPLAY_H
/////////////////////////////////////////////////////////////////////////////
// Display.h - Display user prompts and output							   //
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
/*
* Module Operations:
* ==================
* This module provides the Display class which supports, printing out
* to the console, relevant information about the current processes.
* 
*
* Public Interface:
* =================
* Display dp;
* dp.printline(50);
* dp.displayRequirement("Req 1");
*
*
* Required Files:
* ===============
* Display.cpp Display.h
* ApplicationHelpers.h, ApplicationHelpers.cpp
*
* Build Command:
* ==============
* cl /EHa /DTEST_DISPLAY Display.cpp
*
* Maintenance History:
* ====================
* ver 1.0 : 9 Apr 15
* - first release
*/

#include "../ApplicationHelpers/AppHelpers.h"
using namespace std;

class Display{
public:
	void printline(int number);
	void displayRequirement(std::string input);
};

#endif DISPLAY_H