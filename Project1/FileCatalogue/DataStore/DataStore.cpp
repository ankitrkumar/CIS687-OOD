/////////////////////////////////////////////////////////////////////////////
// DataStore.cpp - Create the DataStore after file manager processes inputs//
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

#include "DataStore.h"
#include <iostream>
#include<string>

//----< Insert the incoming path to the set of paths>-------------------------
void DataStore::savePaths(const string& filespec){	
	paths.insert(filespec);
}

//----< Create datastore with filename and list of iterators>-------------------------
void DataStore::save(const string& filename, const string& path)		
{ 
	PathIter pathIter;
	ListOfIters listOfIters;
	savePaths(path);
	for (pathIter = paths.begin(); pathIter != paths.end(); ++pathIter)
	{
		if (path == *pathIter)					//check if path exists in set
		{
			if (store.count(filename) < 1)		//check if key exists
			{
				listOfIters.push_back(pathIter);
				store[filename] = listOfIters;
			}
			else
			{
				listOfIters = store[filename];
				listOfIters.push_back(pathIter);
				store[filename] = listOfIters;			//update map
			}
		}
		else
			continue;
	}	
}
#ifdef TEST_DATASTORE
int main()
{
  std::cout << "\n  Testing DataStore";

  DataStore ds;
  ds.savePaths("one");
  ds.savePaths("two");
  ds.savePaths("five");

  ds.save("key","value");
  ds.save("key1", "value");
  ds.save("key2", "value");

  for (auto item : ds)
  {
    cout << "\n  " << item.first;
  }
	cout << "\n\n";
}

#endif
