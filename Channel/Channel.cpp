///////////////////////////////////////////////////////////////////////
// Channel.cpp - HTTP based message passing channel Message facility //
// ver 1.0                                                           //
// Language:    Visual C++, Visual Studio 2012                       //
// platform:    Thinkpad, Windows 8 Pro                              //
// Application: Utility for CSE687 projects                          //
// Author:      Liuqize He, Syracuse University                      //
//              lhe03@syr.edu                                        //
///////////////////////////////////////////////////////////////////////
#include "Channel.h"

Channel:: Channel(std::string& targetaddress, int& targetport, int& listenport)
{
  SocketSystem sktsys;
  sndr_ = new Sender(targetaddress,targetport);
  rcvr_ = new Receiver(sktsys.getLocalIP(),listenport);
}

//Constructor:
Channel::Channel(int listenport)
{
  SocketSystem sktsys;
   rcvr_ = new Receiver(sktsys.getLocalIP(),listenport);
}

Channel::Channel(std::string& targetaddress, int& targetport)
{
	sndr_ = new Sender(targetaddress,targetport);
}

//Desctructor:
Channel::~Channel()
{
	delete sndr_;
	delete rcvr_;
}

#ifdef TEST_CHANNEL

int main(){ return 0;}

#endif