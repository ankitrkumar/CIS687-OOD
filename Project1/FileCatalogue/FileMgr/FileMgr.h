#ifndef FILEMGR_H
#define FILEMGR_H
/////////////////////////////////////////////////////////////////////////////
// FileManager.cpp - Process files and directories for datastore to use    //
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
* This module is responsible for processing all the files in a given path
* and provides the directories and files, to be added to the datastore.
* It also provides the capability to do a textsearch and to add the pattern
* to the container.
* 
* Public Interface:
* =================
*
* FileManager fm(path, store);
* fm.addPattern("*.oo");
* fm.textSearch(inputText, store);
* fm.find(path, recurse)
* fm.search(true);
* 
* Required Files:
* ===============
* FileMgr.h FileMgr.cpp
*
* Build Command:
* ==============
* cl /EHa /DTEST_FILEMGR FileMgr.cpp
*
* Maintenance History:
* ====================
* ver 1.0 : 10 Feb 15
* - first release
*/

#include "../FileSystem/FileSystem.h"
#include "../DataStore/DataStore.h"

using namespace std;

class FileMgr
{
public:
  using iterator = DataStore::iterator;
  using patterns = vector <string>;

  FileMgr(const string& path, DataStore& ds) : path_(path), store_(ds){ patterns_.push_back("*.*"); }
  void addPattern(const string& patt);
  void search(bool esOpt);
  set<string> textSearch(string input, DataStore& store);
  set<string> textSearch(string input, DataStore& store, set<string> patters);
  void find(const string& path, bool recurse);

private:
  string path_;
  DataStore& store_;
  patterns patterns_;
};

#endif
