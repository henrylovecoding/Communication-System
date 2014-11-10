#ifndef SENDER_H
#define SENDER_H
/////////////////////////////////////////////////////////////////////
// Sender.h   -  Sender for message passing communication          //
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
   This package provides a single class, Sender. It uses a blocking
   queue provided by Dr. Jim Fawcett as a container for messages to
   be sent. The the queue contains pointers to Message interface. 
   To send the message, use the public post method to push message
   on queue. The sender then gets message from queue and calls Me-
   ssage's virtual send method. Sender doesn't know how each kind 
   of message handles their send process.

  Public Interface:
  ==================
  PostToQueue(): enqueue the message pointer into the send blockqing queue;
  send():  Call different Send_Message() according to their message type
*/

#include <iostream>
#include <thread>
#include "..\Sockets.h"
#include "..\Message\Message.h"
#include "..\BlockingQueue.h"


class Sender
{
public:
	Sender(const std::string& address, const int& tport);
	void PostMsgToQueue(Message* msg);
	void Send();
	~Sender(){}
private:
	std::string Addr;
	int port;
	Socket sckt;
	std::thread* SendThrd;
	BlockingQueue<Message*> Send_Q;

};

#endif