#ifndef WINDOW_H
#define WINDOW_H
/////////////////////////////////////////////////////////////////////////////
// Window.h - Builds a c++/CLI WPF application							   //
// ver 1.0                                                                 //
// ----------------------------------------------------------------------- //
// copyright © Ankit Kumar, 2015                                           //
// All rights granted provided that this notice is retained                //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2013                             //
// Platform:    Asus ROG GL551JM-EH71, Core i7, Windows 8.1 64-bit         //
// Application: OOD Projects, 2015	                                       //
// Author:      Ankit Kumar, Syracuse University						   //
//              (315) 728-8955, akumar15@syr.edu	                       //
/////////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package demonstrates how to build a C++\CLI WPF application.  It 
*  provides one class, ClientView, derived from System::Windows::Window
*  that is compiled with the /clr option to run in the Common Language
*  Runtime, and another class MockChannel written in native C++ and compiled
*  as a DLL with no Common Language Runtime support.
*
*  The window class hosts, in its window, a tab control with three views, two
*  of which are provided with functionality that you may need for Project #4.
*  It loads the DLL holding MockChannel.  MockChannel hosts a send queue, a
*  receive queue, and a C++11 thread that reads from the send queue and writes
*  the deQ'd message to the receive queue.
*
*  The Client can post a message to the MockChannel's send queue.  It hosts
*  a receive thread that reads the receive queue and dispatches any message
*  read to a ListBox in the Client's FileList tab.  So this Demo simulates
*  operations you will need to execute for Project #4.
*
*  Required Files:
*  ---------------
*  Window.h, Window.cpp, MochChannel.h, MochChannel.cpp,
*  BlockingQueue.h, BlockingQueue.cpp
*
*  Build Command:
*  --------------
*  devenv CppCli-WPF-App.sln
*  - this builds C++\CLI client application and native mock channel DLL
*
*  Maintenance History:
*  --------------------
*  ver 2.1 : 28 Apr 2015
* - created complete client view
*  ver 2.0 : 15 Apr 2015
*  - completed message passing demo with moch channel
*  - added FileBrowserDialog to show files in a selected directory
*  ver 1.0 : 13 Apr 2015
*  - incomplete demo with GUI but not connected to mock channel
*/
using namespace System;
using namespace System::Text;
using namespace System::Windows;
using namespace System::Windows::Input;
using namespace System::Windows::Markup;
using namespace System::Windows::Media;                   // TextBlock formatting
using namespace System::Windows::Controls;                // TabControl
using namespace System::Windows::Controls::Primitives;    // StatusBar
using namespace System::Threading;
using namespace System::Threading::Tasks;
using namespace System::Windows::Threading;
using namespace System::ComponentModel;

#include "../MockChannel/MockChannel.h"
#include <iostream>
//#include "../Peer/Peer.h"

namespace CppCliWindows
{
  ref class ClientView : Window
  {
    ISendr* pSendr_;
    IRecvr* pRecvr_;
    IMockChannel* pChann_;

  public:
    ClientView();
    ~ClientView();

    
  private:
    std::string toStdString(String^ pStr);
    String^ toSystemString(std::string& str);
    void setTextBlockProperties();
	size_t srcPort;
	size_t destPort;
	String^ srcAdd;
	String^ destAdd;
	static array<size_t^>^ servers;
	static size_t serverSize = 0;
	array<String^>^ fileList;
	Boolean^ tryNext = true;

	// Controls for Window

	DockPanel^ hDockPanel = gcnew DockPanel();      // support docking statusbar at bottom
	Grid^ hGrid = gcnew Grid();
	TabControl^ hTabControl = gcnew TabControl();

	TabItem^ hSendMessageTab = gcnew TabItem();
	TabItem^ hFileListTab = gcnew TabItem();

	TabItem^ hConnectTab = gcnew TabItem();
	TabItem^ hFileUploadTab = gcnew TabItem();
	TabItem^ hFileDownloadTab = gcnew TabItem();
	TabItem^ hFileSearchTab = gcnew TabItem();
	TabItem^ hTextSearchTab = gcnew TabItem();
	StatusBar^ hStatusBar = gcnew StatusBar();
	StatusBarItem^ hStatusBarItem = gcnew StatusBarItem();
	TextBlock^ hStatus = gcnew TextBlock();

	// Controls for SendMessage View

	Grid^ hSendMessageGrid = gcnew Grid();
	Button^ hSendButton = gcnew Button();
	Button^ hClearButton = gcnew Button();
	TextBlock^ hTextBlock1 = gcnew TextBlock();
	ScrollViewer^ hScrollViewer1 = gcnew ScrollViewer();
	StackPanel^ hStackPanel1 = gcnew StackPanel();

	String^ msgText
		= "Command:ShowMessage\n"   // command
		+ "Sendr:localhost@8080\n"  // send address
		+ "Recvr:localhost@8090\n"  // receive address
		+ "Content-length:44\n"     // body length attribute
		+ "\n"                      // end header
		+ "Hello World\nCSE687 - Object Oriented Design";  // message body

	// Controls for Connect View
	void connect(Object^ sender, RoutedEventArgs^ args);
	Grid^ hConnectGrid = gcnew Grid();
	Button^ hConnectButton = gcnew Button();
	Button^ hListenButton = gcnew Button();
	Label^ hConnectionSrcIPLbl = gcnew Label();
	TextBox^ hConnectionSrcIP = gcnew TextBox();
	Label^ hConnectionSrcPortLbl = gcnew Label();
	TextBox^ hConnectionSrcPort = gcnew TextBox();
	Label^ hConnectionDestIPLbl = gcnew Label();
	TextBox^ hConnectionDestIP = gcnew TextBox();
	Label^ hConnectionDestPortLbl = gcnew Label();
	TextBox^ hConnectionDestPort = gcnew TextBox();
	Button^ hConnectionClearButton = gcnew Button();
	StackPanel^ hConnectStackPanel1 = gcnew StackPanel();
	StackPanel^ hConnectStackPanel2 = gcnew StackPanel();


	// Controls for UplFileList View
	Grid^ hUplGrid = gcnew Grid();
	Forms::FolderBrowserDialog^ hUplFolderBrowserDialog = gcnew Forms::FolderBrowserDialog();
	ListBox^ hUplListBox = gcnew ListBox();
	Button^ hUplFolderBrowseButton = gcnew Button();
	Button^ hUplUploadButton = gcnew Button();
	Grid^ hUplGrid2 = gcnew Grid();

	// Controls for DlListView View
	Grid^ hDlGrid = gcnew Grid();
	Forms::FolderBrowserDialog^ hDlFolderBrowserDialog = gcnew Forms::FolderBrowserDialog();
	ListBox^ hDlListBox = gcnew ListBox();
	ListBox^ hDlServerListBox = gcnew ListBox();
	Button^ hDlFolderBrowseButton = gcnew Button();
	Button^ hDlDloadButton = gcnew Button();
	Label^ hServerLabel = gcnew Label();
	Label^ hFileListLabel = gcnew Label();
	Grid^ hDlGrid2 = gcnew Grid();

	// Controls for Text Search View
	Grid^ hTextSearchViewGrid = gcnew Grid();
	Label^ hTextSearchWildLbl = gcnew Label();
	TextBox^ hTextSearchWild = gcnew TextBox();
	Button^ hTextSearchAdd = gcnew Button();
	Button^ hTextSearchClear = gcnew Button();
	Label^ hTextSearchSelectedLbl = gcnew Label();
	ListBox^ hTextSearchSelectionLB = gcnew ListBox();
	Label^ hTextSearchExpr = gcnew Label();
	TextBox^ hTextSearchExprBox = gcnew TextBox();
	Button^ hTextSearchSearch = gcnew Button();
	StackPanel^ hTextSearchSP1 = gcnew StackPanel();
	StackPanel^ hTextSearchSP2 = gcnew StackPanel();
	StackPanel^ hTextSearchSP3 = gcnew StackPanel();
	Label^ hTextSearchOutputLbl = gcnew Label();
	ListBox^ hTextSearchOutputLB = gcnew ListBox();
	ListBox^ hServersLBTS = gcnew ListBox();
	Label^ hServerLabelTS = gcnew Label();


	// Controls for File Search View
	Grid^ hFileSearchViewGrid = gcnew Grid();
	Label^ hFileSearchWildLbl = gcnew Label();
	TextBox^ hFileSearchWild = gcnew TextBox();
	Button^ hFileSearchAdd = gcnew Button();
	Button^ hFileSearchClear = gcnew Button();
	Button^ hDlShowFilesButton = gcnew Button();
	Label^ hFileSearchSelectedLbl = gcnew Label();
	ListBox^ hFileSearchSelectionLB = gcnew ListBox();
	ListBox^ hServersLBFS = gcnew ListBox();
	Label^ hServerLabelFS = gcnew Label();
	Button^ hFileSearchSearch = gcnew Button();
	StackPanel^ hFileSearchSP1 = gcnew StackPanel();
	StackPanel^ hFileSearchSP2 = gcnew StackPanel();
	StackPanel^ hFileSearchSP3 = gcnew StackPanel();
	Label^ hFileSearchOutputLbl = gcnew Label();
	ListBox^ hFileSearchOutputLB = gcnew ListBox();

	Thread^ recvThread;//MANAGED C++ Thread

	//Various UI properties
	void setUpStatusBar();
	void setUpTextSearchView();
	void setUpTextSearchButtons();
	void setUpFileSearchView();
	void setUpFileSearchButtons();
	void setUpUplView();
	void setUpDlView();
	void setUpDlView2();
	void setUpTabControl();
	void setUpSendMessageView();
	void setUpConnectionView();
	void setUpConnectionButtons();
	void setUpConnectionButtons2();
	array<String^>^ getFileListfromMsg(Message msg);
	array<String^>^ getFileListfromMsgforTS(Message msg);;
	void fileDownloaded();
	void addToServerList(String^ msg);
	void addToFileSearchList(String^ msg);
	void addToTextSearchList(String^ msg);
	void fileListFetched();
	void searchFileListFetched();
	void searchTextListFetched();
	void addText(String^ msg);
	void getMessage();
	void fileSent();
	void eventHandlers();
	void setupReadMe();
	void clear(Object^ sender, RoutedEventArgs^ args);
	void browseForFolder2(Object^ sender, RoutedEventArgs^ args);
	void getServerFileList(Object^ sender, RoutedEventArgs^ args);
	void getServersList(Object^ sender, RoutedEventArgs^ args);
	void OnLoaded(Object^ sender, RoutedEventArgs^ args);
	void Uploadfile(Object^ sender, RoutedEventArgs^ args);
	void downloadFile(Object^ sender, RoutedEventArgs^ args);
	void addPattern(Object^ sender, RoutedEventArgs^ args);
	void clearPatterns(Object^ sender, RoutedEventArgs^ args);
	void addWildCard(Object^ sender, RoutedEventArgs^ args);
	void clearWildCard(Object^ sender, RoutedEventArgs^ args);
	void patternFileSearch(Object^ sender, RoutedEventArgs^ args);
	void patternTextSearch(Object^ sender, RoutedEventArgs^ args);
	void Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args);
	//Getters and setters for the input taken from user for port and address
	void setSrcPort(String^ port)
	{
		srcPort = atoi(toStdString(port).c_str());
	}
	size_t getSrcPort()
	{
		return srcPort;
	}
	void setDestPort(String^ port)
	{
		destPort = atoi(toStdString(port).c_str());
	}
	size_t getDestPort()
	{
		return destPort;
	}
	std::string getSrcAdd()
	{
		return toStdString(srcAdd);
	}
	std::string getDestAdd()
	{
		return toStdString(destAdd);
	}
	void setDestAdd(String^ param)
	{
		destAdd = param;
	}

	void setSrcAdd(String^ param)
	{
		destAdd = param;
	}
  };
}


#endif
