/////////////////////////////////////////////////////////////////////////////
// Window.cpp - Builds a c++/CLI WPF application						   //
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
#include "Window.h"
using namespace CppCliWindows;


//-----<Constructor for the view>--------------------------------
ClientView::ClientView()
{
	// set up channel
	ObjectFactory* pObjFact = new ObjectFactory;
	pSendr_ = pObjFact->createSendr();
	pRecvr_ = pObjFact->createRecvr();
	pChann_ = pObjFact->createMockChannel(pSendr_, pRecvr_);//creates thread
	delete pObjFact;
	servers = gcnew array<size_t^>(10);
	fileList = gcnew array<String^>(3000);
	// client's receive thread

	recvThread = gcnew Thread(gcnew ThreadStart(this, &ClientView::getMessage));//Thread start is a callable object which calls another callabale object
	recvThread->Start();

	eventHandlers();
	// set Window properties

	this->Title = "Remote Code Management Facility";
	this->Width = 800;
	this->Height = 600;

	// attach dock panel to Window

	this->Content = hDockPanel;
	hDockPanel->Children->Add(hStatusBar);
	hDockPanel->SetDock(hStatusBar, Dock::Bottom);
	hDockPanel->Children->Add(hGrid);

	// setup Window controls and views

	setUpTabControl();
	setUpStatusBar();
	setUpSendMessageView();
	setUpTextSearchView();
	setUpFileSearchView();
	setUpDlView();
	setUpUplView();
	setUpConnectionView();
}

// set event handlers
void ClientView::eventHandlers()
{
	hListenButton->Click += gcnew RoutedEventHandler(this, &ClientView::connect);
	hListenButton->Click += gcnew RoutedEventHandler(this, &ClientView::getServersList);
	this->Loaded += gcnew System::Windows::RoutedEventHandler(this, &ClientView::OnLoaded);
	this->Closing += gcnew CancelEventHandler(this, &ClientView::Unloading);
	hClearButton->Click += gcnew RoutedEventHandler(this, &ClientView::clear);//"
	hDlShowFilesButton->Click += gcnew RoutedEventHandler(this, &ClientView::getServerFileList);//"
	hUplFolderBrowseButton->Click += gcnew RoutedEventHandler(this, &ClientView::browseForFolder2);//"
	hUplUploadButton->Click += gcnew RoutedEventHandler(this, &ClientView::Uploadfile);//"
	hDlDloadButton->Click += gcnew RoutedEventHandler(this, &ClientView::downloadFile);//"

	hFileSearchAdd->Click += gcnew RoutedEventHandler(this, &ClientView::addPattern);
	hFileSearchClear->Click += gcnew RoutedEventHandler(this, &ClientView::clearPatterns);

	hTextSearchAdd->Click += gcnew RoutedEventHandler(this, &ClientView::addWildCard);
	hTextSearchClear->Click += gcnew RoutedEventHandler(this, &ClientView::clearWildCard);

	hFileSearchSearch->Click += gcnew RoutedEventHandler(this, &ClientView::patternFileSearch);
	hTextSearchSearch->Click += gcnew RoutedEventHandler(this, &ClientView::patternTextSearch);
}

//-----<Destructor for window>--------------------------------
ClientView::~ClientView()
{
	delete pChann_;
	delete pSendr_;
	delete pRecvr_;
}

//-----<Connect button event handler>--------------------------------
void ClientView::connect(Object^ sender, RoutedEventArgs^ args){

	srcPort = atoi(toStdString(hConnectionSrcPort->Text).c_str());
	destPort = atoi(toStdString(hConnectionDestPort->Text).c_str());

	pChann_->getSrc(toStdString(hConnectionSrcIP->Text), srcPort);
	pChann_->getdest(toStdString(hConnectionDestIP->Text), destPort);
	if (pChann_->start())
		hStatus->Text = "Listening at " + srcPort + " Connected to " + destPort;
	else
		hStatus->Text = "Listening at " + srcPort + " but problem with " + destPort;
	servers[serverSize] = destPort;
	serverSize++;

}

//-----<File search event handler>--------------------------------
void ClientView::patternFileSearch(Object^ sender, RoutedEventArgs^ args)
{
	Console::Write("  \n Search for files");
	if (hServersLBFS->Items->Count > 0)
	{
		for each(UInt32^ port in hServersLBFS->SelectedItems)
		{
			if (hFileSearchSelectionLB->Items->Count > 0)
			{
				String^ strMessage = "";
				for each(String^ value in hFileSearchSelectionLB->SelectedItems)
				{
					strMessage += value + "#";
				}
				Message msg("FILE_SEARCH", toStdString(strMessage), "localhost", srcPort, "localhost", (size_t)port);
				msg.createMessage();
				pChann_->postMessage(msg);
			}
			else
				hStatus->Text = "Select patterns and search else *.* is added by default.";
		}
	}
	else
		hStatus->Text = "Select a server!";
}
//-----<Text search event handler>--------------------------------
void ClientView::patternTextSearch(Object^ sender, RoutedEventArgs^ args)
{
	if (hServersLBTS->Items->Count > 0)
	{
		for each(UInt32^ port in hServersLBTS->SelectedItems)
		{
			if (hTextSearchSelectionLB->Items->Count > 0)
			{
				String^ strMessage = "";
				for each(String^ value in hTextSearchSelectionLB->SelectedItems)
				{
					strMessage += value + "!@!";
				}
				strMessage += hTextSearchExprBox->Text;
				Message msg("TEXT_SEARCH", toStdString(strMessage), "localhost", srcPort, "localhost", (size_t)port);
				msg.createMessage();
				pChann_->postMessage(msg);
			}
			else
				hStatus->Text = "Select patterns and search for text!";
		}
	}
	else
		hStatus->Text = "Select a server!";
}
//-----<Add a pattern event handler>--------------------------------
void ClientView::addPattern(Object^ sender, RoutedEventArgs^ args){
	if (hFileSearchWild->Text != ""){
		hFileSearchSelectionLB->Items->Add(hFileSearchWild->Text);
		hFileSearchWild->Text = "";
	}
	else{
		std::cout << "\nEnter value to add";
	}
}
//-----<Delete a pattern event handler>--------------------------------
void ClientView::clearPatterns(Object^ sender, RoutedEventArgs^ args){
	if (hFileSearchSelectionLB->SelectedItems->Count > 0){
		for (int i = hFileSearchSelectionLB->SelectedItems->Count - 1; i >= 0; i--)
		{
			hFileSearchSelectionLB->Items->RemoveAt(hFileSearchSelectionLB->SelectedIndex);
		}
	}
	else{
		std::cout << "\nSelect Items to Clear";
	}
}
//-----<Add a pattern event handler>--------------------------------
void ClientView::addWildCard(Object^ sender, RoutedEventArgs^ args){
	if (hTextSearchWild->Text != ""){
		hTextSearchSelectionLB->Items->Add(hTextSearchWild->Text);
		hTextSearchWild->Text = "";
	}
	else{
		std::cout << "\nEnter value to add";
	}
}
//-----<Delete a pattern event handler>--------------------------------
void ClientView::clearWildCard(Object^ sender, RoutedEventArgs^ args){
	if (hTextSearchSelectionLB->SelectedItems->Count > 0){
		for (int i = hTextSearchSelectionLB->SelectedItems->Count - 1; i >= 0; i--)
		{
			hTextSearchSelectionLB->Items->RemoveAt(hTextSearchSelectionLB->SelectedIndex);
		}
	}
	else{
		std::cout << "\nSelect Items to Clear";
	}
}
//-----<Upload event handler>--------------------------------
void ClientView::Uploadfile(Object^ sender, RoutedEventArgs^ args)
{
	if (hUplListBox->SelectedItems->Count > 0)
	{
		for each(String^ value in hUplListBox->SelectedItems)
		{
			Message msg("UPLOAD_FILE", toStdString(value), toStdString(hConnectionSrcIP->Text), srcPort, toStdString(hConnectionDestIP->Text), destPort);
			msg.createMessage();
			pChann_->postMessage(msg);
		}
	}
}
//-----<Download event handler>--------------------------------
void ClientView::downloadFile(Object^ sender, RoutedEventArgs^ args)
{
	if (hDlListBox->SelectedItems->Count > 0)
	{
		for each(String^ value in hDlListBox->SelectedItems)
		{
			Message msg("DL_FILE", toStdString(value), toStdString(hConnectionSrcIP->Text), srcPort, toStdString(hConnectionDestIP->Text), destPort);
			msg.createMessage();
			pChann_->postMessage(msg);
		}
	}
}

//-----<setupStatus bar>--------------------------------
void ClientView::setUpStatusBar()
{
	hStatusBar->Items->Add(hStatusBarItem);
	hStatus->Text = "WATCH THIS PLACE FOR IMPORTANT INFORMATION!!!!";
	//status->FontWeight = FontWeights::Bold;
	hStatusBarItem->Content = hStatus;
	hStatusBar->Padding = Thickness(10, 2, 10, 2);
}

//-----<setUp TabControl>--------------------------------
void ClientView::setUpTabControl()
{
	hGrid->Children->Add(hTabControl);
	hSendMessageTab->Header = "Read Me";
	hConnectTab->Header = "Connect";
	hFileUploadTab->Header = "File Upload";
	hFileDownloadTab->Header = "File Download";
	hFileSearchTab->Header = "File Search";
	hTextSearchTab->Header = "Text Search";
	hTabControl->Items->Add(hSendMessageTab);
	hTabControl->Items->Add(hConnectTab);
	hTabControl->Items->Add(hFileUploadTab);
	hTabControl->Items->Add(hFileDownloadTab);
	hTabControl->Items->Add(hFileSearchTab);
	hTabControl->Items->Add(hTextSearchTab);

}

//-----<setup TextBlock properties>--------------------------------
void ClientView::setTextBlockProperties()
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hSendMessageGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hTextBlock1;
	hTextBlock1->Padding = Thickness(15);
	hTextBlock1->Text = "";
	hTextBlock1->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hTextBlock1->FontWeight = FontWeights::Bold;
	hTextBlock1->FontSize = 16;
	hScrollViewer1->VerticalScrollBarVisibility = ScrollBarVisibility::Auto;
	hScrollViewer1->Content = hBorder1;
	hSendMessageGrid->SetRow(hScrollViewer1, 0);
	hSendMessageGrid->Children->Add(hScrollViewer1);
}

//-----<setUp sendMessage View>--------------------------------
void ClientView::setUpSendMessageView()
{
	hSendMessageGrid->Margin = Thickness(20);
	hSendMessageTab->Content = hSendMessageGrid;

	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hSendMessageGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hTextBlock1;
	hTextBlock1->Padding = Thickness(15);
	hTextBlock1->FontWeight = FontWeights::Bold;
	hTextBlock1->FontSize = 14;
	setupReadMe();
	hScrollViewer1->VerticalScrollBarVisibility = ScrollBarVisibility::Auto;
	hScrollViewer1->Content = hBorder1;
	hSendMessageGrid->SetRow(hScrollViewer1, 0);
	hSendMessageGrid->Children->Add(hScrollViewer1);

	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(75);
	hSendMessageGrid->RowDefinitions->Add(hRow2Def);
	hSendMessageGrid->Children->Add(hStackPanel1);
}

//-----<setUp Read Me>--------------------------------
void ClientView::setupReadMe()
{
	hTextBlock1->Text = "READ ME.....\n\nTo test the project " +
		"	\n\n"
		+ "Connect :\n"
		+ "1. Start the servers first and then connect to \n the server by entering the port value of that server\n"
		+ "2. Default values are Client port is 8888 and \n Server ports are started on ports 7777 and 9999.\n"
		+ "3. Create connection establishes the connection\n(Wait for button to turn grey to confirm connection complete)\n\n"

		+ "File Upload :\n"
		+ "For File Upload select one more files and they \n will be uploaded to the server to which you just established connection to.\n"
		+ "You can see the files in . / DestFolder_<serverPort> / \n\n"

		+ "File Download :\n"
		+ "1. From the list of servers select a server and \n click show files on server to show all the files on that server.\n"
		+ "PLEASE WAIT FOR SERVER TO RESPOND.STATUS BAR IS UPDATED WHEN REQUEST IS COMPLETE\n"
		+ "2. From the list of files shown select one or more to download to the folder . / DestFolder_<clientPort> / . \n\n"

		+ "File Search :\n"

		+ "1. Enter or more file patterns, ex. *.png	and click add to list.\n"
		+ "2. Select server and patterns from the list of patterns(it is multiselect)\n"
		+ "3. Click search files.\n"
		+ "STATUS BAR WILL UPDATE WHEN RESULTS ARE READY OR IF INPUT IS INCORRECT.\n"

		+ "Text Search :\n"
		+ "1. Enter or more file patterns, ex. *.png	and click add to list.\n"
		+ "2. Select server and patterns from the list of patterns(it is multiselect)\n"
		+ "3. Input a text string to search.\n"
		+ "4. Click Search Text.\n"
		+ "STATUS BAR WILL UPDATE WHEN RESULTS ARE READY OR IF INPUT IS INCORRECT.\n";
}
//-----<setUp Connection view>--------------------------------

void ClientView::setUpConnectionView()
{


	hConnectTab->Content = hConnectGrid;
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hRow1Def->Height = GridLength(80);
	RowDefinition^ hRow3Def = gcnew RowDefinition();
	hRow3Def->Height = GridLength(80);
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(80);
	hConnectGrid->RowDefinitions->Add(hRow1Def);
	hConnectGrid->RowDefinitions->Add(hRow2Def);
	hConnectGrid->RowDefinitions->Add(hRow3Def);
	hConnectGrid->Margin = Thickness(20);


	hListenButton->Content = "Create connection";
	hListenButton->BorderThickness = Thickness(2);
	hListenButton->BorderBrush = Brushes::Azure;
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Width = 120;
	hBorder1->Height = 30;
	hBorder1->Child = hListenButton;
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 10;
	TextBlock^ hSpacer1 = gcnew TextBlock();
	hSpacer1->Width = 10;

	hConnectStackPanel1->Children->Add(hSpacer1);
	hConnectStackPanel1->Children->Add(hBorder1);
	hConnectStackPanel1->Orientation = Orientation::Horizontal;
	hConnectStackPanel1->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;

	setUpConnectionButtons();
	hConnectGrid->SetRow(hConnectStackPanel1, 1);
	hConnectGrid->Children->Add(hConnectStackPanel1);
}

//-----<setUp Connection buttons>--------------------------------
void ClientView::setUpConnectionButtons()
{
	hConnectionSrcIPLbl->Content = "Source IP";
	hConnectionSrcIPLbl->Height = 30;
	hConnectionSrcIP->Height = 20;
	hConnectionSrcIP->Text = "localhost";
	hConnectionSrcIP->Width = 80;

	hConnectionSrcPortLbl->Content = "Source port";
	hConnectionSrcPortLbl->Height = 30;
	hConnectionSrcPort->Height = 20;
	hConnectionSrcPort->Text = "8888";
	hConnectionSrcPort->Width = 80;

	hConnectionDestIPLbl->Content = "Destination IP";
	hConnectionDestIPLbl->Height = 30;
	hConnectionDestIP->Height = 20;
	hConnectionDestIP->Text = "localhost";
	hConnectionDestIP->Width = 80;

	hConnectionDestPortLbl->Content = "Destinaton Port";
	hConnectionDestPortLbl->Height = 30;
	hConnectionDestPortLbl->Width = 95;
	hConnectionDestPort->Text = "9999";
	hConnectionDestPort->Height = 20;
	hConnectionDestPort->Width = 80;

	Border^ hBorder2 = gcnew Border();
	hBorder2->Width = 120;
	hBorder2->Height = 30;
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	/*hBorder2->Child = hConnectButton;
	hConnectButton->Content = "Connect";*/
	hConnectButton->BorderThickness = Thickness(2);
	hConnectButton->BorderBrush = Brushes::Black;

	setUpConnectionButtons2();

}

//-----<setUp Connection buttons2>--------------------------------
void ClientView::setUpConnectionButtons2()
{
	TextBlock^ hSpacer2 = gcnew TextBlock();
	hSpacer2->Width = 10;
	TextBlock^ hSpacer3 = gcnew TextBlock();
	hSpacer3->Width = 10;
	TextBlock^ hSpacer4 = gcnew TextBlock();
	hSpacer4->Width = 10;
	TextBlock^ hSpacer5 = gcnew TextBlock();
	hSpacer5->Width = 10;
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 10;
	TextBlock^ hSpacer1 = gcnew TextBlock();
	hSpacer1->Width = 10;
	TextBlock^ hSpacer0 = gcnew TextBlock();
	hSpacer1->Width = 10;
	hConnectStackPanel2->Children->Add(hConnectionSrcIPLbl);
	hConnectStackPanel2->Children->Add(hConnectionSrcIP);
	hConnectStackPanel2->Children->Add(hSpacer0);
	hConnectStackPanel2->Children->Add(hConnectionSrcPortLbl);
	hConnectStackPanel2->Children->Add(hSpacer);
	hConnectStackPanel2->Children->Add(hConnectionSrcPort);
	hConnectStackPanel2->Children->Add(hSpacer1);

	hConnectStackPanel2->Children->Add(hConnectionDestIPLbl);
	hConnectStackPanel2->Children->Add(hSpacer2);
	hConnectStackPanel2->Children->Add(hConnectionDestIP);
	hConnectStackPanel2->Children->Add(hSpacer3);
	hConnectStackPanel2->Children->Add(hConnectionDestPortLbl);
	hConnectStackPanel2->Children->Add(hSpacer4);
	hConnectStackPanel2->Children->Add(hConnectionDestPort);
	hConnectStackPanel2->Children->Add(hSpacer5);
	hConnectStackPanel2->Orientation = Orientation::Horizontal;
	hConnectStackPanel2->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hConnectGrid->SetRow(hConnectStackPanel2, 0);
	hConnectGrid->SetColumn(hConnectStackPanel2, 2);
	hConnectGrid->Children->Add(hConnectStackPanel2);
	hConnectGrid->Margin = Thickness(20);
}
//-----<System to string>--------------------------------
std::string ClientView::toStdString(String^ pStr)
{
	std::string dst;
	for (int i = 0; i < pStr->Length; ++i)
		dst += (char)pStr[i];
	return dst;
}
//-----<std to system string>--------------------------------
String^ ClientView::toSystemString(std::string& str)
{
	StringBuilder^ pStr = gcnew StringBuilder();
	for (size_t i = 0; i < str.size(); ++i)
		pStr->Append((Char)str[i]);
	return pStr->ToString();
}
//-----<add text to status bar>--------------------------------
void ClientView::addText(String^ msg)
{
	hStatus->Text = msg;
	Console::Write("\n" + msg);
}

//-----<update server list>--------------------------------
void ClientView::addToServerList(String^ msg)
{
	hDlListBox->Items->Clear();
	for (int i = 0; i < fileList->Length; ++i)
		hDlListBox->Items->Add(fileList[i]);
}

//-----<update File search list>--------------------------------
void ClientView::addToFileSearchList(String^ msg)
{
	hFileSearchOutputLB->Items->Clear();
	for (int i = 0; i < fileList->Length; ++i)
		hFileSearchOutputLB->Items->Add(fileList[i]);
}

//-----<update Text Search list>--------------------------------
void ClientView::addToTextSearchList(String^ msg)
{
	hTextSearchOutputLB->Items->Clear();
	for (int i = 0; i < fileList->Length; ++i)
		hTextSearchOutputLB->Items->Add(fileList[i]);
}

//-----<getchild thread to pass infor to ain thread using DISPATCHER(last line)>--------------------------------

void ClientView::getMessage()
{
	// recvThread runs this function

	while (true)
	{
		std::cout << "\n  receive thread calling getMessage()";
		Message msg = pRecvr_->rmRecvrQ();
		if (msg.getCmd() == "FILE_SENT")
			fileSent();
		if (msg.getCmd() == "DL_FILE_S")
			fileDownloaded();
		if (msg.getCmd() == "FILE_LIST_S")
		{
			fileList = getFileListfromMsg(msg);
			fileListFetched();
		}
		if (msg.getCmd() == "FILE_SEARCH_S")
		{
			fileList = getFileListfromMsg(msg);
			searchFileListFetched();
		}
		if (msg.getCmd() == "TEXT_SEARCH_S")
		{
			fileList = getFileListfromMsgforTS(msg);
			searchTextListFetched();
		}
	}
}

//-----<Helper Function for file message>--------------------------------
array<String^>^ ClientView::getFileListfromMsg(Message msg)
{
	//Console::Write(toSystemString(msg.getFileName()));
	array<String^>^ strarray = gcnew array<String^>(3000);
	std::string s = msg.getFileName();
	std::string delimiter = "*";
	size_t pos = 0;
	int i = 0;
	std::string token;
	while ((pos = s.find(delimiter)) != std::string::npos) {
		token = s.substr(0, pos);
		strarray[i] = toSystemString(token);
		i++;
		s.erase(0, pos + delimiter.length());
	}
	return strarray;
}

//-----<Helper function for Text search>--------------------------------
array<String^>^ ClientView::getFileListfromMsgforTS(Message msg)
{
	//Console::Write(toSystemString(msg.getFileName()));
	array<String^>^ strarray = gcnew array<String^>(3000);
	std::string s = msg.getFileName();
	std::string delimiter = "@!@";
	size_t pos = 0;
	int i = 0;
	std::string token;
	while ((pos = s.find(delimiter)) != std::string::npos) {
		token = s.substr(0, pos);
		strarray[i] = toSystemString(token);
		i++;
		s.erase(0, pos + delimiter.length());
	}
	return strarray;
}

//-----<update files from server>--------------------------------
void ClientView::fileListFetched()
{
	String^ sMsg = "Files fetched from server";
	array<String^>^ args = gcnew array<String^>(1);
	args[0] = sMsg;
	Action<String^>^ act = gcnew Action<String^>(this, &ClientView::addText);//binding delegate to a particular function--act is the delegate
	Dispatcher->Invoke(act, args);  // must call addText on main UI thread
	Action<String^>^ act2 = gcnew Action<String^>(this, &ClientView::addToServerList);//binding delegate to a particular function--act is the delegate
	Dispatcher->Invoke(act2, args);  // must call addText on main UI thread
}

//-----<update File Search list>--------------------------------
void ClientView::searchFileListFetched()
{
	String^ sMsg = "File(s) on server obtained";
	array<String^>^ args = gcnew array<String^>(1);
	args[0] = sMsg;
	Action<String^>^ act = gcnew Action<String^>(this, &ClientView::addText);//binding delegate to a particular function--act is the delegate
	Dispatcher->Invoke(act, args);  // must call addText on main UI thread
	Action<String^>^ act2 = gcnew Action<String^>(this, &ClientView::addToFileSearchList);//binding delegate to a particular function--act is the delegate
	Dispatcher->Invoke(act2, args);  // must call addText on main UI thread
}

//-----<update File Text list>--------------------------------
void ClientView::searchTextListFetched()
{
	String^ sMsg = "File(s) on server wit given text obtained";
	array<String^>^ args = gcnew array<String^>(1);
	args[0] = sMsg;
	Action<String^>^ act = gcnew Action<String^>(this, &ClientView::addText);//binding delegate to a particular function--act is the delegate
	Dispatcher->Invoke(act, args);  // must call addText on main UI thread
	Action<String^>^ act2 = gcnew Action<String^>(this, &ClientView::addToTextSearchList);//binding delegate to a particular function--act is the delegate
	Dispatcher->Invoke(act2, args);  // must call addText on main UI thread
}

//-----<update File sent>--------------------------------
void ClientView::fileSent()
{
	String^ sMsg = "File(s) Uploaded Successfully!";
	array<String^>^ args = gcnew array<String^>(1);
	args[0] = sMsg;

	Action<String^>^ act = gcnew Action<String^>(this, &ClientView::addText);//binding delegate to a particular function--act is the delegate
	Dispatcher->Invoke(act, args);  // must call addText on main UI thread
}

//-----<update File downloaded>--------------------------------
void ClientView::fileDownloaded()
{
	String^ sMsg = "File(s) Downloaded Successfully!";
	array<String^>^ args = gcnew array<String^>(1);
	args[0] = sMsg;

	Action<String^>^ act = gcnew Action<String^>(this, &ClientView::addText);//binding delegate to a particular function--act is the delegate
	Dispatcher->Invoke(act, args);  // must call addText on main UI thread
}
//-----<clear event handler>--------------------------------
void ClientView::clear(Object^ sender, RoutedEventArgs^ args)
{
	Console::Write("\n  cleared message text");
	hStatus->Text = "Cleared message";
	hTextBlock1->Text = "";
}
//-----<setup upload view>--------------------------------
void ClientView::setUpUplView()
{

	hUplGrid->Margin = Thickness(20);
	hFileUploadTab->Content = hUplGrid;
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	//hRow1Def->Height = GridLength(75);
	hUplGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hUplListBox->SelectionMode = SelectionMode::Single;
	hBorder1->Child = hUplListBox;
	hUplGrid->SetRow(hBorder1, 0);
	hUplGrid->Children->Add(hBorder1);

	RowDefinition^ hRow2Def = gcnew RowDefinition();
	RowDefinition^ hRow3Def = gcnew RowDefinition();
	hRow3Def->Height = GridLength(75);
	hRow2Def->Height = GridLength(75);
	hUplGrid->RowDefinitions->Add(hRow2Def);
	hUplGrid->RowDefinitions->Add(hRow3Def);
	hUplUploadButton->Content = "Upload File(s)";
	hUplUploadButton->Height = 30;
	hUplUploadButton->Width = 120;
	hUplUploadButton->BorderThickness = Thickness(2);
	hUplUploadButton->BorderBrush = Brushes::Black;
	hUplFolderBrowseButton->Content = "Select Directory";
	hUplFolderBrowseButton->Height = 30;
	hUplFolderBrowseButton->Width = 120;
	hUplFolderBrowseButton->BorderThickness = Thickness(2);
	hUplFolderBrowseButton->BorderBrush = Brushes::Black;
	hUplGrid->SetRow(hUplFolderBrowseButton, 1);
	hUplGrid->SetRow(hUplUploadButton, 2);
	hUplGrid->Children->Add(hUplFolderBrowseButton);
	hUplGrid->Children->Add(hUplUploadButton);

	hUplFolderBrowserDialog->ShowNewFolderButton = false;
	hUplFolderBrowserDialog->SelectedPath = System::IO::Directory::GetCurrentDirectory();
}
//-----<setup download view>--------------------------------
void ClientView::setUpDlView()
{
	
	hDlGrid->Margin = Thickness(20);
	hFileDownloadTab->Content = hDlGrid;
	RowDefinition^ hRow0Def = gcnew RowDefinition();
	hDlGrid->RowDefinitions->Add(hRow0Def);
	hRow0Def->Height = GridLength(40);
	ColumnDefinition^ hCol1Def = gcnew ColumnDefinition();
	hCol1Def->Width = GridLength(80);
	hDlGrid->ColumnDefinitions->Add(hCol1Def);
	ColumnDefinition^ hCol2Def = gcnew ColumnDefinition();
	hDlGrid->ColumnDefinitions->Add(hCol2Def);
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hServerLabel->Content = "Servers:";
	hServerLabel->Width = 50;
	hServerLabel->Height = 30;
	hFileListLabel->Content = "Files on selected servers:";
	hFileListLabel->Width = 150;
	hFileListLabel->Height = 30;
	//hRow1Def->Height = GridLength(75);
	hDlGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hDlListBox->SelectionMode = SelectionMode::Single;
	hDlServerListBox->SelectionMode = SelectionMode::Single;
	hDlServerListBox->BorderBrush = Brushes::Black;
	hDlServerListBox->BorderThickness = Thickness(1);
	hBorder1->Child = hDlListBox;
	hDlGrid->SetRow(hDlServerListBox, 1);
	hDlGrid->SetRow(hBorder1, 1);
	hDlGrid->SetRow(hServerLabel, 0);
	hDlGrid->SetColumn(hServerLabel, 0);
	hDlGrid->SetRow(hFileListLabel, 0);
	hDlGrid->SetColumn(hFileListLabel, 1);
	hDlGrid->SetColumn(hDlServerListBox, 0);
	hDlGrid->SetColumn(hBorder1, 1);
	hDlGrid->Children->Add(hFileListLabel);
	hDlGrid->Children->Add(hServerLabel);
	hDlGrid->Children->Add(hDlServerListBox);
	hDlGrid->Children->Add(hBorder1);

	setUpDlView2();
}
//-----<setup download view>--------------------------------
void ClientView::setUpDlView2()
{
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	RowDefinition^ hRow3Def = gcnew RowDefinition();
	hRow3Def->Height = GridLength(75);
	hRow2Def->Height = GridLength(75);
	hDlGrid->RowDefinitions->Add(hRow2Def);
	hDlGrid->RowDefinitions->Add(hRow3Def);
	hDlDloadButton->Content = "Download File(s)";
	hDlDloadButton->Height = 30;
	hDlDloadButton->Width = 120;
	hDlDloadButton->BorderThickness = Thickness(2);
	hDlDloadButton->BorderBrush = Brushes::Black;
	hDlShowFilesButton->Content = "Show Files on Server";
	hDlShowFilesButton->Height = 30;
	hDlShowFilesButton->Width = 120;
	hDlShowFilesButton->BorderThickness = Thickness(2);
	hDlShowFilesButton->BorderBrush = Brushes::Black;
	hDlGrid->SetRow(hDlShowFilesButton, 2);
	hDlGrid->SetRow(hDlDloadButton, 3);
	hDlGrid->SetColumn(hDlShowFilesButton, 1);
	hDlGrid->SetColumn(hDlDloadButton, 1);
	hDlGrid->Children->Add(hDlShowFilesButton);
	hDlGrid->Children->Add(hDlDloadButton);

	hDlFolderBrowserDialog->ShowNewFolderButton = false;
	hDlFolderBrowserDialog->SelectedPath = System::IO::Directory::GetCurrentDirectory();
}

//-----<setup text search view>--------------------------------
void ClientView::setUpTextSearchView(){

	hTextSearchViewGrid->Margin = Thickness(1);
	hTextSearchWildLbl->Content = "Enter Wild Card :";
	hTextSearchWildLbl->Height = 30;
	hTextSearchWild->Height = 30;
	hTextSearchWild->Width = 70;
	hTextSearchWild->Text = "*.cpp";
	TextBlock^ hSpacer2 = gcnew TextBlock();
	hSpacer2->Width = 10;
	hTextSearchAdd->Content = "Add to List";
	hTextSearchAdd->Width = 60;
	hTextSearchAdd->Height = 30;
	hTextSearchAdd->BorderThickness = Thickness(1);
	hTextSearchAdd->BorderBrush = Brushes::Black;
	TextBlock^ hSpacer3 = gcnew TextBlock();
	hSpacer3->Width = 10;
	hTextSearchClear->Content = "Delete Selected";
	hTextSearchClear->Width = 90;
	hTextSearchClear->Height = 30;
	hServerLabelTS->Content = "Servers:";
	hServerLabelTS->Width = 50;
	hServerLabelTS->Height = 30;
	hServersLBTS->Height = 80;
	hServersLBTS->Width = 50;
	hServersLBTS->SelectionMode = SelectionMode::Single;
	hTextSearchClear->BorderThickness = Thickness(1);
	hTextSearchClear->BorderBrush = Brushes::Black;
	TextBlock^ hSpacer4 = gcnew TextBlock();
	hSpacer4->Width = 10;
	TextBlock^ hSpacer5 = gcnew TextBlock();
	hSpacer5->Width = 10;

	hTextSearchSP1->Children->Add(hTextSearchWildLbl);
	hTextSearchSP1->Children->Add(hTextSearchWild);
	hTextSearchSP1->Children->Add(hSpacer2);
	hTextSearchSP1->Children->Add(hTextSearchAdd);
	hTextSearchSP1->Children->Add(hSpacer3);
	hTextSearchSP1->Children->Add(hTextSearchClear);
	hTextSearchSP1->Children->Add(hSpacer4);
	hTextSearchSP1->Children->Add(hServerLabelTS);
	hTextSearchSP1->Children->Add(hServersLBTS);
	hTextSearchSP1->Orientation = Orientation::Horizontal;
	hTextSearchSP1->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hTextSearchSP3->Children->Add(hTextSearchSP1);

	setUpTextSearchButtons();
}
//-----<setup Text Search view buttons>--------------------------------
void ClientView::setUpTextSearchButtons(){

	hTextSearchSelectedLbl->Content = "WildCard List : ";
	hTextSearchSelectedLbl->Height = 30;
	hTextSearchSelectionLB->Height = 80;
	hTextSearchSelectionLB->Width = 60;
	hTextSearchSelectionLB->SelectionMode = SelectionMode::Multiple;

	hTextSearchExpr->Content = "Text to Search :";
	hTextSearchExpr->Height = 30;
	hTextSearchExprBox->Text = "ankit";
	hTextSearchExprBox->Height = 30;
	hTextSearchExprBox->Width = 90;
	TextBlock^ hSpacer2 = gcnew TextBlock();
	hSpacer2->Width = 20;
	hTextSearchSearch->Content = "Search Text";
	hTextSearchSearch->Width = 120;
	hTextSearchSearch->Height = 30;
	hTextSearchSearch->BorderThickness = Thickness(1);
	hTextSearchSearch->BorderBrush = Brushes::Black;
	hTextSearchSP2->Children->Add(hTextSearchSelectedLbl);
	hTextSearchSP2->Children->Add(hTextSearchSelectionLB);
	hTextSearchSP2->Children->Add(hTextSearchExpr);
	hTextSearchSP2->Children->Add(hTextSearchExprBox);
	hTextSearchSP2->Children->Add(hSpacer2);
	hTextSearchSP2->Children->Add(hTextSearchSearch);
	hTextSearchSP2->Margin = Thickness(10);
	hTextSearchSP2->Orientation = Orientation::Horizontal;
	hTextSearchSP2->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hTextSearchSP3->Children->Add(hTextSearchSP2);
	ScrollViewer^ scrollTxt = gcnew ScrollViewer();
	scrollTxt->Content = hTextSearchOutputLB;
	scrollTxt->BorderThickness = Thickness(1);
	scrollTxt->BorderBrush = Brushes::Black;
	scrollTxt->Height = 300;
	scrollTxt->Width = 400;

	hTextSearchOutputLbl->Content = "   Files from servers: ";
	hTextSearchSP3->Children->Add(hTextSearchOutputLbl);
	hTextSearchSP3->Children->Add(scrollTxt);
	hTextSearchSP3->Orientation = Orientation::Vertical;
	hTextSearchSP3->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hTextSearchTab->Content = hTextSearchSP3;
}



//-----<setup File search view buttons>--------------------------------
void ClientView::setUpFileSearchView(){

	hFileSearchViewGrid->Margin = Thickness(1);
	hFileSearchWildLbl->Content = "Enter File Pattern :";
	hFileSearchWildLbl->Height = 30;
	hFileSearchWild->Height = 30;
	hFileSearchWild->Width = 70;
	hFileSearchWild->Text = "*.cpp";
	TextBlock^ hSpacer2 = gcnew TextBlock();
	hSpacer2->Width = 10;
	hFileSearchAdd->Content = "Add to List";
	hFileSearchAdd->Width = 60;
	hFileSearchAdd->Height = 30;
	hFileSearchAdd->BorderThickness = Thickness(1);
	hFileSearchAdd->BorderBrush = Brushes::Black;
	TextBlock^ hSpacer3 = gcnew TextBlock();
	hSpacer3->Width = 10;
	hFileSearchClear->Content = "Delete Selected";
	hFileSearchClear->Width = 90;
	hFileSearchClear->Height = 30;
	hServerLabelFS->Content = "Servers:";
	hServerLabelFS->Width = 50;
	hServerLabelFS->Height = 30;
	hServersLBFS->Height = 80;
	hServersLBFS->Width = 50;
	hServersLBFS->SelectionMode = SelectionMode::Single;
	hFileSearchClear->BorderThickness = Thickness(1);
	hFileSearchClear->BorderBrush = Brushes::Black;
	TextBlock^ hSpacer4 = gcnew TextBlock();
	hSpacer4->Width = 10;
	TextBlock^ hSpacer5 = gcnew TextBlock();
	hSpacer5->Width = 10;

	hFileSearchSP1->Children->Add(hFileSearchWildLbl);
	hFileSearchSP1->Children->Add(hFileSearchWild);
	hFileSearchSP1->Children->Add(hSpacer2);
	hFileSearchSP1->Children->Add(hFileSearchAdd);
	hFileSearchSP1->Children->Add(hSpacer3);
	hFileSearchSP1->Children->Add(hFileSearchClear);
	hFileSearchSP1->Children->Add(hSpacer4);
	hFileSearchSP1->Orientation = Orientation::Horizontal;
	hFileSearchSP1->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hFileSearchSP3->Children->Add(hFileSearchSP1);

	setUpFileSearchButtons();
}

//-----<setup file search buttons view>--------------------------------
void ClientView::setUpFileSearchButtons(){
	TextBlock^ hSpacer0 = gcnew TextBlock();
	hSpacer0->Width = 10;
	hFileSearchSelectedLbl->Content = "Patterns : ";
	hFileSearchSelectedLbl->Height = 30;
	hFileSearchSelectionLB->Height = 100;
	hFileSearchSelectionLB->Width = 70;
	hFileSearchSelectionLB->SelectionMode = SelectionMode::Multiple;

	TextBlock^ hSpacer2 = gcnew TextBlock();
	hSpacer2->Width = 20;

	hFileSearchSearch->Content = "Search Files";
	hFileSearchSearch->Width = 90;
	hFileSearchSearch->Height = 25;
	hFileSearchSearch->BorderThickness = Thickness(1);
	hFileSearchSearch->BorderBrush = Brushes::Black;
	hFileSearchSP2->Children->Add(hFileSearchSelectedLbl);
	hFileSearchSP2->Children->Add(hSpacer0);
	hFileSearchSP2->Children->Add(hFileSearchSelectionLB);
	hFileSearchSP2->Children->Add(hServerLabelFS);
	hFileSearchSP2->Children->Add(hServersLBFS);
	hFileSearchSP2->Children->Add(hSpacer2);
	hFileSearchSP2->Children->Add(hFileSearchSearch);
	hFileSearchSP2->Margin = Thickness(10);
	hFileSearchSP2->Orientation = Orientation::Horizontal;
	hFileSearchSP2->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hFileSearchSP3->Children->Add(hFileSearchSP2);
	ScrollViewer^ scrollTxt = gcnew ScrollViewer();
	scrollTxt->Content = hFileSearchOutputLB;
	scrollTxt->BorderThickness = Thickness(1);
	scrollTxt->BorderBrush = Brushes::Black;
	scrollTxt->Height = 300;
	scrollTxt->Width = 400;

	hFileSearchOutputLbl->Content = "   Files from servers: ";
	hFileSearchSP3->Children->Add(hFileSearchOutputLbl);
	hFileSearchSP3->Children->Add(scrollTxt);
	hFileSearchSP3->Orientation = Orientation::Vertical;
	hFileSearchSP3->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hFileSearchTab->Content = hFileSearchSP3;
}

//-----<setup browser for folder>--------------------------------
void ClientView::browseForFolder2(Object^ sender, RoutedEventArgs^ args)
{
	std::cout << "\n  Browsing for folder";
	hUplListBox->Items->Clear();
	System::Windows::Forms::DialogResult result;
	result = hUplFolderBrowserDialog->ShowDialog();
	if (result == System::Windows::Forms::DialogResult::OK)
	{
		String^ path = hUplFolderBrowserDialog->SelectedPath;
		std::cout << "\n  opening folder \"" << toStdString(path) << "\"";
		array<String^>^ files = System::IO::Directory::GetFiles(path, L"*.*");
		for (int i = 0; i < files->Length; ++i)
			hUplListBox->Items->Add(files[i]);
		array<String^>^ dirs = System::IO::Directory::GetDirectories(path);
		for (int i = 0; i < dirs->Length; ++i)
			hUplListBox->Items->Add(L"<> " + dirs[i]);
	}
}

//-----<server list event handler>--------------------------------
void ClientView::getServersList(Object ^ sender, RoutedEventArgs^ args)
{
	std::cout << "\n  Get server list ";
	hDlServerListBox->Items->Clear();
	hServersLBFS->Items->Clear();
	hServersLBTS->Items->Clear();
	for (int i = 0; i < servers->Length; ++i)
	{
		hDlServerListBox->Items->Add(servers[i]);
		hServersLBTS->Items->Add(servers[i]);
		hServersLBFS->Items->Add(servers[i]);
	}
}
//-----<server file list event handler>--------------------------------
void ClientView::getServerFileList(Object ^ sender, RoutedEventArgs^ args)
{
	if (hDlServerListBox->SelectedItems->Count > 0){
		for each(UInt32^ value in hDlServerListBox->SelectedItems)
		{
			Message msg("FILE_LIST", "", toStdString(hConnectionSrcIP->Text), srcPort, toStdString(hConnectionDestIP->Text), (size_t)value);
			msg.createMessage();
			pChann_->postMessage(msg);
		}
	}
}
//-----<event handler>--------------------------------
void ClientView::OnLoaded(Object^ sender, RoutedEventArgs^ args)
{
	Console::Write("\n  Window loaded");
}
//-----<event handler>--------------------------------
void ClientView::Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args)
{
	Console::Write("\n  Window closing");
}

[STAThread]
//int _stdcall WinMain()
int main(array<System::String^>^ args)
{
	Console::WriteLine(L"\n Starting ClientView");
	Application^ app = gcnew Application();
	app->Run(gcnew ClientView());
	Console::WriteLine(L"\n\n");
}