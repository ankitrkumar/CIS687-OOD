#ifndef MOCKCHANNEL_H
#define MOCKCHANNEL_H
/////////////////////////////////////////////////////////////////////////////
// MockChannel.h - This channel allows for use of native code onC++\CLI	   //
//					client. MockCHannel read from senderQ and writes to	   //
//					receiver Q											   //
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

#ifdef IN_DLL
#define DLL_DECL __declspec(dllexport)
#else
#define DLL_DECL __declspec(dllimport)
#endif

#include <string>
#include "../Message/Message.h"

struct ISendr
{
	virtual void addSenderQ(Message msg) = 0;
	virtual bool connectPeer(std::string ip, size_t port) = 0;
	//virtual BlockingQueue<Message>& getSenderQ() = 0;
};

struct IRecvr
{
	virtual Message rmRecvrQ() = 0;
	virtual void listen(int port) = 0;
	virtual void connect() = 0;
//	virtual BlockingQueue<Message>& getRecvrQ() = 0;
};


struct IMockChannel
{
public:
  virtual void postMessage(const Message& msg) = 0;
  virtual Message getMessage() = 0;
  virtual bool start() = 0;
  virtual bool stop() = 0;
  virtual void getSrc(std::string srcAdd_, size_t srcPort_) = 0;
  virtual void getdest(std::string destAdd_, size_t destPort_) = 0;
};

extern "C" {
  struct ObjectFactory
  {
    DLL_DECL ISendr* createSendr();
    DLL_DECL IRecvr* createRecvr();
	DLL_DECL IMockChannel* createMockChannel(ISendr* pISendr, IRecvr* pIRecvr);
	//DLL_DECL IMockChannel* createMockChannel(ISendr* pISendr, IRecvr* pIRecvr);
  };
}

#endif


