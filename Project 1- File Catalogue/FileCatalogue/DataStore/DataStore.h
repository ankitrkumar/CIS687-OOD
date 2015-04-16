#ifndef DATASTORE_H
#define DATASTORE_H

/////////////////////////////////////////////////////////////////////////////
// DataStore.h - Create the DataStore after file manager processes inputs  //
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
* This module is responsible for creating the Set of Paths as well as 
* for creating the map data store, which will store the key and 
* list of pointers to paths.
*
* Public Interface:
* =================
*
* DataStore d;
* d.save(key, listofIters);
* d.savePaths(path);
*
* Required Files:
* ===============
* DataStore.h DataStore.cpp
*
* Build Command:
* ==============
* cl /EHa /DTEST_DATASTORE DataStore.cpp
*
* Maintenance History:
* ====================
* ver 1.0 : 10 Feb 15
* - first release
*/

#include <vector>
#include <set>
#include <list>
#include <map>
#include <algorithm>

using namespace std;

class DataStore
{
public:
	using Path = string;
	using Paths = set < Path >;
	using PathIter = Paths::iterator;
	using ListOfIters = list < PathIter > ;
	using File = string;
	using Store = map < File, ListOfIters > ;
	using iterator = Store::iterator;

	void savePaths(const string& filespec);
	void save(const string& filename, const string& path);
	iterator begin() { return store.begin(); }
	iterator end() { return store.end(); }
	int noOfFiles(){ return store.size(); }
	int noOfDirectories(){ return paths.size(); }

private:
	Store store;
	Paths paths;
};

#endif