#ifndef MESSAGE_H
#define MESSAGE_H
/////////////////////////////////////////////////////////////////////
// Message.h   -  Message for message passing communication        //
//                facility                                         //
// ver 1.0                                                         //
// Language:      Visual C++, 2012                                 //
// Platform:      Thinkpad, Windows 8 Pro                          //
// Application:   Utility for CSE687 projects                      //
// Author:       Liuqize He, Syracuse University                   //
//               lhe03@syr.edu                                     //
/////////////////////////////////////////////////////////////////////

/*
  Module Operations: 
  ==================
   This module provides a set of related network message class that
   implement the Message interface. These classes take care of their
   own sending and receiving process which complies the HTTP netwo-
   rk protocol. At the socket communication level, it uses the soc-
   ket package provided by Dr. Jim Fawcett.
   Two classes are provided:

   TextMessage:
   -------------
   Provides facility for sending text message. It corporates 
   all info about the message, include target address and port,
   type, etc.

   FileMessage:
   -------
   Provides facility for sending file message. It does the same
   thing as the TextMessage. Additionally it provides the actual send 
   and receive of file.

  Public Interface:
  ==================
  WriteToString(): Serialize a message string;
  ReadFromString(): Deserailziae from a string to build a message;
  Send_Message(): Send out Message, for sender to use;
  ReceMessage(): Receive Message, for receiver to use
*/

#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include "..\Sockets.h"
#include "..\Locks.h"

struct TempBlock
{
  std::string content;

};



class Message
{
public:
	virtual std::string WriteToString()=0;  // bond all the information together
    virtual void ReadFromString(std::string Msg)=0; // extract all information from a string 
	virtual void Send_Message(Socket sckt)=0;
	virtual void ReceMessage(Socket sckt, size_t length)=0;
	virtual ~Message() {}
	
};



class TextMessage : public Message
{
public:
	TextMessage();
	TextMessage(const TextMessage& textmsg);
	TextMessage(const std::string& Addr, const int& Port, const std::string& body, const std::string& type);
	TextMessage& operator=(const TextMessage& textmsg);
	std::string WriteToString();
	void ReadFromString(std::string Msg);
	void Send_Message(Socket sckt);
    void ReceMessage(Socket sckt, size_t length);
	~TextMessage(){}
private:
	std::string Addr;
	int port;
	std::string body;
	std::string type;
};

class FileMessage: public Message
{
public: 
	    FileMessage();
	    FileMessage(const FileMessage& filemsg);
		FileMessage(const std::string& address, const int& tport, const std::string& type, const std::string& name);
		FileMessage& operator=(const FileMessage& textmsg);
		std::string WriteToString();
		void ReadFromString(std::string Msg);
		void Send_Message(Socket sckt);
		void ReceMessage(Socket sckt, size_t length);
		~FileMessage(){}
		
private:
	std::vector<TempBlock> Container;
	std::string filename;
	std::string Addr;
	int port;
	std::string type;

};

#endif