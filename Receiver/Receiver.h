#ifndef RECEIVER_H
#define RECEIVER_H
/////////////////////////////////////////////////////////////////////
// Receiver.h -Receiver for message passing communication          //
//                facility                                         //
// ver 1.0                                                         //
// Language:      Visual C++, 2012                                 //
// Platform:      Thinkpad, Windows 8 Pro                          //
// Application:   Utility for CSE687 projects                      //
// Author:        Liuqize He, Syracuse University                  //
//                lhe03@syr.edu                                    //
/////////////////////////////////////////////////////////////////////

/*
  Module Operations: 
  ==================
   This package provides a single class, Receiver. It uses blocking
   queue provided by Dr. Jim Fawcett as a container for messages r-
   eceived. The the queue contains pointers to Message interface. 
   To start, receiver starts a socket listener thread. The receiver
   infinitely loops to receive any connection that is calling it's 
   port. Each time a connection is established, it spawns a new th-
   read to process the receiving process. To receiver the message,
   receiver doesn't know how each bytes is organized to be a messa-
   ge. Like the sender, it calls polymorphic behavior of Message
   interface to handle the specific case.


  Public Interface:
  ==================
  WaitForConnection(): Listen for its own port;
  PushToRcvrBq(): enqueue the message pointer into the receive blockqing queue;
  Receive(): Call different ReceMessage() according to their message type;

*/

#include <iostream>
#include <thread>
#include "..\Sockets.h"
#include "..\Message\Message.h"
#include "..\BlockingQueue.h"


class Receiver
{
public:
	Receiver(std::string lisaddress,int lisport);
	void WaitForConnection();
	void PushToRcvrBq(Message* pTrmsg);
	void Receive(Socket& skt);
	~Receiver();
private:
	std::string Addr;
	int port;
	std::thread* Listen_Thrd;
	bool Stop_Listen;
	SocketListener sckt_listen;
	std::vector<std::thread> ThrdPool_;
    BlockingQueue<Message*> Recrv_Q;
	
	
};

#endif