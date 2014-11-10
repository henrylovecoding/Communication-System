#ifndef CHANNEL_H
#define CHANNEL_H
/////////////////////////////////////////////////////////////////////
// Channel.h   -  Provides HTTP based massage passing communicating//
//                channel                                          //
// ver 1.0                                                         //
// Language:      Visual C++, 2012                                 //
// Platform:      Thinkpad, Windows 8 Pro                          //
// Application:   Utility for CSE687 projects                      //
// Author:        Liuqize He, Syracuse University                  //
//                lhe03@syr.edu                                    //
/////////////////////////////////////////////////////////////////////
/*
   Package Operations:
   ==================
   This package provides a single class, Channel. It contains a sender
   end and a receiver end which together act as a network communication
   endpoint. Once a Channel instance is created, it activates both it's
   sender and receiver. The receiver sits at a certain port to wait for
   connection while sender sends message from it's sending queue. To s-
   end a message, simply post the message using public post method.

*/

#include<iostream>
#include "..\Sender\Sender.h"
#include "..\Receiver\Receiver.h"


class Channel
{
public:
	Channel(std::string& targetaddress, int& targetport, int& listenport);
	Channel(int listenport);
	Channel(std::string& targetaddress, int& targetport);
	
	~Channel();

private:
	Sender* sndr_ ;
	Receiver* rcvr_;


};

#endif
