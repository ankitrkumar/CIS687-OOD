/////////////////////////////////////////////////////////////////////////////
// MockChannel.cpp - Demo for CSE687 Project #4, Spring 2015               //
// - build as DLL to show how C++\CLI client can use native code channel   //
// - MockChannel reads from sendQ and writes to recvQ                      //
//                                                                         //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2015               //
/////////////////////////////////////////////////////////////////////////////

#define IN_DLL
#include "MockChannel.h"
#include <string>
#include <thread>
#include "../Peer/Peer.h"
#include "../Sockets/Sockets.h"
#include <iostream>

class MockChannel : public IMockChannel
{
public:
  MockChannel();
  bool start();
  bool stop();

private:
  std::thread thread_;
  ISendr* pISendr_;
  IRecvr* pIRecvr_;
  bool stop_ = false;
};


//----< signal server thread to stop >---------------------------------------

bool MockChannel::stop() { stop_ = true; return true; }

//----< factory functions >--------------------------------------------------

ISendr* ObjectFactory::createSendr() { return new Sender(); }

IRecvr* ObjectFactory::createRecvr() { return new Receiver(); }

//----< factory function creaes a Peer>--------------------------------------------------
IMockChannel* ObjectFactory::createMockChannel(ISendr* pISendr, IRecvr* pIRecvr)
{ 
	return new Peer(pISendr, pIRecvr);
}

#ifdef TEST_MOCKCHANNEL

//----< test stub >----------------------------------------------------------

int main()
{
  ObjectFactory objFact;
  ISendr* pSendr = objFact.createSendr();
  IRecvr* pRecvr = objFact.createRecvr();
  IMockChannel* pMockChannel = objFact.createMockChannel(pSendr, pRecvr);
  pMockChannel->start();
  pSendr->postMessage("Hello World");
  pSendr->postMessage("CSE687 - Object Oriented Design");
  Message msg = pRecvr->getMessage();
  std::cout << "\n  received message = \"" << msg << "\"";
  msg = pRecvr->getMessage();
  std::cout << "\n  received message = \"" << msg << "\"";
  pSendr->postMessage("stopping");
  msg = pRecvr->getMessage();
  std::cout << "\n  received message = \"" << msg << "\"";
  pMockChannel->stop();
  pSendr->postMessage("quit");
  std::cin.get();
}
#endif
