#ifndef DISPATCHER_H
#define DISPATCHER_H

#include<iostream>
#include<thread>
#include<map>
#include<string>
#include"../Message/Message.h"
#include "../ApplicationHelpers/AppHelpers.h"

using namespace std;

class Dispatcher
{
public:
	void start();		// starts a dispatcher worker thread that monitors
	void dispatcherFunc();
private:

};
void fileUpload(Message msg);

#endif