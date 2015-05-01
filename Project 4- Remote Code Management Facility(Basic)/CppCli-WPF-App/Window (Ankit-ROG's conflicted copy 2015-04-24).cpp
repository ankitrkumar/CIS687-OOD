///////////////////////////////////////////////////////////////////////////
// Window.cpp - C++\CLI implementation of WPF Application                //
//          - Demo for CSE 687 Project #4                                //
// ver 2.0                                                               //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2015             //
///////////////////////////////////////////////////////////////////////////
/*
*  To run as a Windows Application:
*  - Set Project Properties > Linker > System > Subsystem to Windows
*  - Comment out int main(...) at bottom
*  - Uncomment int _stdcall WinMain() at bottom
*  To run as a Console Application:
*  - Set Project Properties > Linker > System > Subsytem to Console
*  - Uncomment int main(...) at bottom
*  - Comment out int _stdcall WinMain() at bottom
*/
#include "Window.h"
using namespace CppCliWindows;

WPFCppCliDemo::WPFCppCliDemo()
{
  // set up channel

  ObjectFactory* pObjFact = new ObjectFactory;
  pSendr_ = pObjFact->createSendr();
  pRecvr_ = pObjFact->createRecvr();
  pChann_ = pObjFact->createMockChannel(pSendr_, pRecvr_);//creates thread
  pChann_->start();//chanel starts
  delete pObjFact;

  // client's receive thread

  recvThread = gcnew Thread(gcnew ThreadStart(this, &WPFCppCliDemo::getMessage));//Thread start is a callable object which calls another callabale object
  recvThread->Start();

  // set event handlers

  this->Loaded += 
    gcnew System::Windows::RoutedEventHandler(this, &WPFCppCliDemo::OnLoaded);
  this->Closing += 
    gcnew CancelEventHandler(this, &WPFCppCliDemo::Unloading);
  hSendButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::sendMessage);//binfing th their respective functions
  hClearButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::clear);//"
  hFolderBrowseButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::browseForFolder);//"

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
  setUpFileListView();
  setUpConnectionView();
}

WPFCppCliDemo::~WPFCppCliDemo()
{
  delete pChann_;
  delete pSendr_;
  delete pRecvr_;
}

void WPFCppCliDemo::setUpStatusBar()
{
  hStatusBar->Items->Add(hStatusBarItem);
  hStatus->Text = "very important messages will appear here";
  //status->FontWeight = FontWeights::Bold;
  hStatusBarItem->Content = hStatus;
  hStatusBar->Padding = Thickness(10, 2, 10, 2);
}

void WPFCppCliDemo::setUpTabControl()
{
  hGrid->Children->Add(hTabControl);
  hSendMessageTab->Header = "Send Message";
  hFileListTab->Header = "File List";
  hConnectTab->Header = "Connect";
  hFileUploadTab->Header = "File Upload";
  hFileDownloadTab->Header = "File Download";
  hTabControl->Items->Add(hSendMessageTab);
  hTabControl->Items->Add(hFileListTab);
  hTabControl->Items->Add(hConnectTab);
  hTabControl->Items->Add(hFileUploadTab);
  hTabControl->Items->Add(hFileDownloadTab);
}

void WPFCppCliDemo::setTextBlockProperties()
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


void WPFCppCliDemo::setButtonsProperties()
{
  RowDefinition^ hRow2Def = gcnew RowDefinition();
  hRow2Def->Height = GridLength(75);
  hSendMessageGrid->RowDefinitions->Add(hRow2Def);
  hSendButton->Content = "Send Message";
  Border^ hBorder2 = gcnew Border();
  hBorder2->Width = 120;
  hBorder2->Height = 30;
  hBorder2->BorderThickness = Thickness(1);
  hBorder2->BorderBrush = Brushes::Black;
  hClearButton->Content = "Clear";
  hBorder2->Child = hSendButton;
  Border^ hBorder3 = gcnew Border();
  hBorder3->Width = 120;
  hBorder3->Height = 30;
  hBorder3->BorderThickness = Thickness(1);
  hBorder3->BorderBrush = Brushes::Black;
  hBorder3->Child = hClearButton;
  hStackPanel1->Children->Add(hBorder2);
  TextBlock^ hSpacer = gcnew TextBlock();
  hSpacer->Width = 10;
  hStackPanel1->Children->Add(hSpacer);
  hStackPanel1->Children->Add(hBorder3);
  hStackPanel1->Orientation = Orientation::Horizontal;
  hStackPanel1->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
  hSendMessageGrid->SetRow(hStackPanel1, 1);
  hSendMessageGrid->Children->Add(hStackPanel1);
}

void WPFCppCliDemo::setUpSendMessageView()
{
  Console::Write("\n  setting up sendMessage view");
  hSendMessageGrid->Margin = Thickness(20);
  hSendMessageTab->Content = hSendMessageGrid;

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

  RowDefinition^ hRow2Def = gcnew RowDefinition();
  hRow2Def->Height = GridLength(75);
  hSendMessageGrid->RowDefinitions->Add(hRow2Def);
  hSendButton->Content = "Send Message";
  Border^ hBorder2 = gcnew Border();
  hBorder2->Width = 120;
  hBorder2->Height = 30;
  hBorder2->BorderThickness = Thickness(1);
  hBorder2->BorderBrush = Brushes::Black;
  hClearButton->Content = "Clear";
  hBorder2->Child = hSendButton;
  Border^ hBorder3 = gcnew Border();
  hBorder3->Width = 120;
  hBorder3->Height = 30;
  hBorder3->BorderThickness = Thickness(1);
  hBorder3->BorderBrush = Brushes::Black;
  hBorder3->Child = hClearButton;
  hStackPanel1->Children->Add(hBorder2);
  TextBlock^ hSpacer = gcnew TextBlock();
  hSpacer->Width = 10;
  hStackPanel1->Children->Add(hSpacer);
  hStackPanel1->Children->Add(hBorder3);
  hStackPanel1->Orientation = Orientation::Horizontal;
  hStackPanel1->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
  hSendMessageGrid->SetRow(hStackPanel1, 1);
  hSendMessageGrid->Children->Add(hStackPanel1);
}

void WPFCppCliDemo::setUpConnectionLayout()
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hConnectGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hConnectionDestIP->Width = 120;
	hConnectionDestIP->Height = 30;
	hConnectionDestIP->Text = "sasdda";
	hBorder1->Child = hConnectionSrcIP;
	hBorder1->Child = hConnectionDestIP;
	hBorder1->Child = hConnectionSrcPort;
	hBorder1->Child = hConnectionDestPort;
	hConnectButton->Padding = Thickness(15);
	hConnectTab->Content = hConnectGrid;


}

void WPFCppCliDemo::setUpConnectionView()
{

	Console::Write("\n  setting up Connection view");

	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hRow1Def->Height = GridLength(80);
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(80);
	hConnectGrid->RowDefinitions->Add(hRow1Def);
	hConnectGrid->RowDefinitions->Add(hRow2Def);
	hConnectGrid->Margin = Thickness(20);
	
	hConnectionSrcPortLbl->Content = "Source port";
	hConnectionSrcPortLbl->Height = 35;
	hConnectionSrcPort->Height = 35;
	hConnectionSrcPort->Height = 35;
	hConnectButton->Content = "Create connection";
	hConnectButton->BorderThickness = Thickness(2);
	hConnectButton->BorderBrush = Brushes::Azure;
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Width = 120;
	hBorder1->Height = 30;
	hBorder1->Child = hConnectButton;
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 10;
	TextBlock^ hSpacer1 = gcnew TextBlock();
	hSpacer1->Width = 10;

	hConnectStackPanel1->Children->Add(hConnectionSrcPortLbl);
	hConnectStackPanel1->Children->Add(hSpacer);
	hConnectStackPanel1->Children->Add(hConnectionSrcPort);
	hConnectStackPanel1->Children->Add(hSpacer1);
	hConnectStackPanel1->Children->Add(hBorder1);
	hConnectStackPanel1->Orientation = Orientation::Horizontal;
	hConnectStackPanel1->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;

	hConnectGrid->SetRow(hStackPanel1,0);
	hConnectGrid->Children->Add(hStackPanel1);
	
	//setUpConnectionLayout();
	//setTextBlockProperties();
	//setButtonsProperties();
}


std::string WPFCppCliDemo::toStdString(String^ pStr)
{
  std::string dst;
  for (int i = 0; i < pStr->Length; ++i)
    dst += (char)pStr[i];
  return dst;
}

void WPFCppCliDemo::sendMessage(Object^ obj, RoutedEventArgs^ args)
{
  pSendr_->postMessage(toStdString(msgText));
  Console::Write("\n  sent message");
  hStatus->Text = "Sent message";
}

String^ WPFCppCliDemo::toSystemString(std::string& str)
{
  StringBuilder^ pStr = gcnew StringBuilder();
  for (size_t i = 0; i < str.size(); ++i)
    pStr->Append((Char)str[i]);
  return pStr->ToString();
}

void WPFCppCliDemo::addText(String^ msg)
{
  hTextBlock1->Text += msg + "\n\n";
}

void WPFCppCliDemo::getMessage()//getchild thread to pass infor to ain thread using DISPATCHER(last line)
{
  // recvThread runs this function

  while (true)
  {
    std::cout << "\n  receive thread calling getMessage()";
    std::string msg = pRecvr_->getMessage();
    String^ sMsg = toSystemString(msg);
    array<String^>^ args = gcnew array<String^>(1);
    args[0] = sMsg;

    Action<String^>^ act = gcnew Action<String^>(this, &WPFCppCliDemo::addText);//binding delegate to a particular function--act is the delegate
    Dispatcher->Invoke(act, args);  // must call addText on main UI thread
  }
}

void WPFCppCliDemo::clear(Object^ sender, RoutedEventArgs^ args)
{
  Console::Write("\n  cleared message text");
  hStatus->Text = "Cleared message";
  hTextBlock1->Text = "";
}

void WPFCppCliDemo::setUpFileListView()
{
  Console::Write("\n  setting up FileList view");
  hFileListGrid->Margin = Thickness(20);
  hFileListTab->Content = hFileListGrid;
  RowDefinition^ hRow1Def = gcnew RowDefinition();
  //hRow1Def->Height = GridLength(75);
  hFileListGrid->RowDefinitions->Add(hRow1Def);
  Border^ hBorder1 = gcnew Border();
  hBorder1->BorderThickness = Thickness(1);
  hBorder1->BorderBrush = Brushes::Black;
  hBorder1->Child = hListBox;
  hFileListGrid->SetRow(hBorder1, 0);
  hFileListGrid->Children->Add(hBorder1);

  RowDefinition^ hRow2Def = gcnew RowDefinition();
  hRow2Def->Height = GridLength(75);
  hFileListGrid->RowDefinitions->Add(hRow2Def);
  hFolderBrowseButton->Content = "Select Directory";
  hFolderBrowseButton->Height = 30;
  hFolderBrowseButton->Width = 120;
  hFolderBrowseButton->BorderThickness = Thickness(2);
  hFolderBrowseButton->BorderBrush = Brushes::Black;
  hFileListGrid->SetRow(hFolderBrowseButton, 1);
  hFileListGrid->Children->Add(hFolderBrowseButton);

  hFolderBrowserDialog->ShowNewFolderButton = false;
  hFolderBrowserDialog->SelectedPath = System::IO::Directory::GetCurrentDirectory();
}


void WPFCppCliDemo::browseForFolder(Object^ sender, RoutedEventArgs^ args)
{
  std::cout << "\n  Browsing for folder";
  hListBox->Items->Clear();
  System::Windows::Forms::DialogResult result;
  result = hFolderBrowserDialog->ShowDialog();
  if (result == System::Windows::Forms::DialogResult::OK)
  {
    String^ path = hFolderBrowserDialog->SelectedPath;
    std::cout << "\n  opening folder \"" << toStdString(path) << "\"";
    array<String^>^ files = System::IO::Directory::GetFiles(path, L"*.*");
    for (int i = 0; i < files->Length; ++i)
      hListBox->Items->Add(files[i]);
    array<String^>^ dirs = System::IO::Directory::GetDirectories(path);
    for (int i = 0; i < dirs->Length; ++i)
      hListBox->Items->Add(L"<> " + dirs[i]);
  }
}

void WPFCppCliDemo::OnLoaded(Object^ sender, RoutedEventArgs^ args)
{
  Console::Write("\n  Window loaded");
}
void WPFCppCliDemo::Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args)
{
  Console::Write("\n  Window closing");
}

[STAThread]
//int _stdcall WinMain()
int main(array<System::String^>^ args)
{
  Console::WriteLine(L"\n Starting WPFCppCliDemo");

  Application^ app = gcnew Application();
  app->Run(gcnew WPFCppCliDemo());
  Console::WriteLine(L"\n\n");
}