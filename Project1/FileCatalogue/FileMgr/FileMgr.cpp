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

#include "FileMgr.h"
#include "../FileSystem/FileSystem.h"
#include "../DataStore/DataStore.h"
#include <iostream>
#include <string>
#include <iterator>

using namespace std;

//----< add incoming pattern to container>-------------------------
void FileMgr::addPattern(const std::string& patt)
{
	if (patterns_.size() == 1 && patterns_[0] == "*.*")
		patterns_.pop_back();
	if(std::find(patterns_.begin(), patterns_.end(), patt) == patterns_.end())
		patterns_.push_back(patt);
}
//----< acept search from executve to fetch from directories>-------
void FileMgr::search(bool esOpt)
{
		find(path_, esOpt);
}
//----< Add the path and pattern to the datastore>-------------------------
void FileMgr::find(const string& path, bool recurse)		
{
	for (auto patt : patterns_)
	{
		vector<string> files = FileSystem::Directory::getFiles(path, patt);//file fullname
		for (auto f : files)
		{
			store_.save(f, path);
		}
	}
	if (!recurse)
		return;

	vector<string> dirs = FileSystem::Directory::getDirectories(path);
	for (auto d : dirs)
	{
		if (d == "." || d == "..")
			continue;
		d = path + "\\" + d;
		find(d, 1);
	}

}

//----< Search all the files in the datastore to see if a given text exists>-------------------------
set<string> FileMgr::textSearch(string input, DataStore& store)
{
	set<string> founds;
	for (DataStore::iterator mapIt = store.begin(); mapIt != store.end(); mapIt++)
	{
		for (DataStore::ListOfIters::iterator fileIter = mapIt->second.begin(); fileIter != mapIt->second.end(); ++fileIter)
		{
			FileSystem::File file(**fileIter + "\\" + mapIt->first);
			file.open(FileSystem::File::in);
			if (file.isGood())
			{
				std::string all = file.readAll(true);
				if (all.find(input) != string::npos) 
				{
					founds.insert(**fileIter + "\\" + mapIt->first);
				}
			}
			file.close();
		}
	}
	return founds;
}
//----< Search all the files in the datastore to see if a given text exists, accept pattern types>-------------------------
set<string> FileMgr::textSearch(string input, DataStore& store, set<string> patters)
{
	set<string> founds;
	for (DataStore::iterator mapIt = store.begin(); mapIt != store.end(); mapIt++)
	{
			for (DataStore::ListOfIters::iterator fileIter = mapIt->second.begin(); fileIter != mapIt->second.end(); ++fileIter)
			{
			FileSystem::File file(**fileIter + "\\" + mapIt->first);
			file.open(FileSystem::File::in);
			if (file.isGood())
			{
				for (auto p : patters)
				{
					if ("*."+FileSystem::Path::getExt(**fileIter + "\\" + mapIt->first) == p)//check for matching extension 
					{

						std::string all = file.readAll(true);
						if (all.find(input) != string::npos)
						{
							founds.insert(**fileIter + "\\" + mapIt->first);
						}
					}
				}
			}
			file.close();
		}
		

}
	return founds;
}



#ifdef TEST_FILEMGR
int main()
{
	DataStore ds;
	FileMgr fm(".", ds);
	fm.addPattern("*.partial");
	fm.addPattern("*.*");
	fm.search(1);
	fm.textSearch(".cpp", ds);
//	ds.displayFiles();
	for (auto fs : ds)
	{
		cout << "\n  " << fs.first;
	}
	cout << "\n\n";
	return 0;
}
#endif