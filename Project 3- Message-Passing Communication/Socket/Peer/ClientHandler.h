//#ifndef clienthandler_h
//#define clienthandler_h
///////////////////////////////////////////////////////////////////////////////
//// clienthandler.h - gets the request on a thread and releases the receiver// 
////					from the work so that it can continue to listen to	   //
////					incoming messages									   //
//// ver 1.0                                                                 //
//// ----------------------------------------------------------------------- //
//// copyright © ankit kumar, 2015                                           //
//// all rights granted provided that this notice is retained                //
//// ----------------------------------------------------------------------- //
//// language:    visual c++, visual studio 2013                             //
//// platform:    asus rog gl551jm-eh71, core i7, windows 8.1 64-bit         //
//// application: ood projects, 2015	                                       //
//// author:      ankit kumar, syracuse university						   //
////              (315) 728-8955, akumar15@syr.edu	                       //
///////////////////////////////////////////////////////////////////////////////
///*
//* module operations:
//* ==================
//* this module creates a worker thread so that the reciever can listen to 
//* other incoming messages from other peers
//*
//* public interface:
//* =================
//* 
//* bool getfile(msg, socket);
//* to get the file specified in the message header
//* 
//* message getmsgheader(socket& socket);
//* get the header file specified
//* 
//* required files:
//* ===============
//* filesystem.h, filesystem.cpp,sockets.h,sockets.cpp
//* message.h, message.cpp
//*
//* build command:
//* ==============
//* cl /eha /dtest_clienthandler clienthandler.cpp
//*
//* maintenance history:
//* ====================
//* ver 1.0 : 9 april 2015
//* - first release
//*/
//
//
//#include "../Sockets/Sockets.h"
//#include <string>
//#include "../Message/Message.h"
//#include "Filesystem.h"
//#include "Receiver.h"
//#include "../BlockingQueue/BlockingQueue.h"
//
//class ClientHandler
//{
//public:
//	ClientHandler(BlockingQueue<Message>& bQ_) : bQ(bQ_){}
//	void operator()(Socket& socket_);
//	bool getFile(Message msg, Socket& socket);	
//	Message getMsgHeader(Socket& socket);		
//
//private:
//	BlockingQueue<Message>& bQ;
//};
//
//#endif