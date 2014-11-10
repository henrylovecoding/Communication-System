/////////////////////////////////////////////////////////////////////
// Sender.cpp  - Implementation of class Sender                   ///
// ver 1.0                                                         //
// Language:      Visual C++, 2012                                 //
// Platform:      Thinkpad e530 win8 Pro                           //
// Application:   Utility for CSE687 projects                      //
// Author:        Liuqize He, Syracuse University                  //
// Contact:       lhe03@syr.edu                                    //
/////////////////////////////////////////////////////////////////////

#include<iostream>
#include<thread>
#include "Sender.h"
#include "..\FileSystem.h"
using namespace FileSystem;

//Constructor:
Sender:: Sender(const std::string& address, const int& tport)
	: Addr(address), port(tport){}

//Destructor:
//Sender::~Sender()
//{
//	SendThrd->join();
//	//ClearQueue();
//	delete SendThrd;
//}

//Push the pointer which point to base class Message into queue
void Sender::PostMsgToQueue(Message* Ptrmsg)
{
	Send_Q.enQ(Ptrmsg);
}

// It will call the corresponding kind message's Send_Message() function
// accoring to the type information extracted before
void Sender:: Send()
{
	if(!sckt.connect(Addr,port))
		throw std::exception("\n Not connect to the target ip and port");
	else
	{
	while(Send_Q.size()>0)
	  {
	   std::auto_ptr<Message>Ptr_Msg(Send_Q.deQ()); // smart pointer
	   Ptr_Msg->Send_Message(sckt);
	  }
    }

}
