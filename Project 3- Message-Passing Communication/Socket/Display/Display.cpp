/////////////////////////////////////////////////////////////////////////////
// Display.cpp - Display user prompts and output				           //
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


#include "Display.h"
#include <iostream>
#include <string>

using namespace std;

//----< To print given number of lines onto the screen>-------------------------
void Display::printline(int number)
{
	cout << "\n";
	for (int i = 0; i < number; i++)
	{
		cout << "-";
	}
	cout << "\n";
}

void Display::displayRequirement(std::string input)
{
	ApplicationHelpers::Verbose::show(input);
}

#ifdef TEST_DISPLAY

int main()
{
	Display dp;
	dp.printline(50);


	return 0;
}

#endif 