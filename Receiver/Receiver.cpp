/////////////////////////////////////////////////////////////////////
// Receiver.cpp  - Implementation of class Receiver                //
// ver 1.0                                                         //
// Language:      Visual C++, 2012                                 //
// Platform:      Thinkpad e530 win8 Pro                           //
// Application:   Utility for CSE687 projects                      //
// Author:        Liuqize He, Syracuse University                  //
// Contact:       lhe03@syr.edu                                    //
/////////////////////////////////////////////////////////////////////

#include <string>
#include <iostream>
#include <vector>
#include "Receiver.h"
#include "..\Locks.h"
#include "..\Base64Encoding\Base64.h"
#include "..\FileSystem.h"


using namespace FileSystem;

//Constructor:
Receiver::Receiver(std::string lisaddress, int lisport)
	:Addr(lisaddress),port(lisport),sckt_listen(lisport), Stop_Listen(false)
{
  Listen_Thrd = new std::thread(&Receiver::WaitForConnection, &(*this));
}

//Destructor:
Receiver::~Receiver()
{
	for (size_t i=0; i<ThrdPool_.size(); ++i)
		ThrdPool_.at(i).join();
	Listen_Thrd->join();
	delete Listen_Thrd;
}

// The listener thread function
void Receiver::WaitForConnection()
{
   while (!Stop_Listen)
	{
	    Socket skt = sckt_listen.waitForConnect();
		std::thread thrd(&Receiver::Receive, &(*this), skt);
		// Once skt created, spawn an thread, then move thread to thread pool
		ThrdPool_.push_back(std::move(thrd));
		
	}

}

void Receiver:: PushToRcvrBq(Message* pTrmsg)
{
	Recrv_Q.enQ(pTrmsg);
}

// Handle connection, create coresponding pointer according
// to the information contains in socket
void Receiver::Receive(Socket& skt)
{
	while (!skt.bytesLeft())
		;
	while (skt.bytesLeft() > 0)
	{
	
		std::string initLine = skt.readLine();
		size_t pos_front = initLine.find(" ");
	    std::string temp = initLine.substr(pos_front + 1);
	    size_t pos_back = temp.find(" ");
	    std::string type_ = temp.substr(0, pos_back);
		std::string msgType =type_;
		size_t pos = initLine.find(":");
	    std::string size = initLine.substr(pos +2);
	    size_t rcvrsize = FromString<int>(size);
		Message* ptrMsg;
		if (msgType == "text")
			ptrMsg = new TextMessage();   // raw pointer
		else if (msgType == "file")
			ptrMsg = new FileMessage();
		else
			break;
		ptrMsg->ReceMessage(skt,rcvrsize);
		PushToRcvrBq(ptrMsg);
		delete ptrMsg;
	
	}
	
  }
