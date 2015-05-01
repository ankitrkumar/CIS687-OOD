#ifndef DISPATCHER_H
#define DISPATCHER_H

#include<iostream>
#include<thread>
#include<map>
#include<string>
#include"../Message/Message.h"
#include "../ApplicationHelpers/AppHelpers.h"
#include"Sender.h"
#include"Receiver.h"
using namespace std;

class Dispatcher
{
public:
	Dispatcher(BlockingQueue<Message>& sBQ_, BlockingQueue<Message>& rBQ_) : sBQ(sBQ_), rBQ(rBQ_){}
	void start();		// starts a dispatcher worker thread that monitors
	void dispatcherFunc();
	void fileUpload(Message msg);
	void fileListSend(Message msg);
	void fileDlSuccess(Message msg);
	void fileSearchSuccess(Message msg);
	void textSearchSuccess(Message msg);
	void sendFile(Message msg);
private:
	BlockingQueue<Message>& sBQ;
	BlockingQueue<Message>& rBQ;
};


#endif