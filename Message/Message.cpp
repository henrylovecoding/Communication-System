///////////////////////////////////////////////////////////////////////
// Message.cpp - HTTP based message passing channel Message facility //
// ver 1.0                                                           //
// Language:    Visual C++, Visual Studio 2012                       //
// platform:    Thinkpad, Windows 8 Pro                              //
// Application: Utility for CSE687 projects                          //
// Author:      Liuqize He, Syracuse University                      //
//              lhe03@syr.edu                                        //
///////////////////////////////////////////////////////////////////////
#include <iostream>
//#include <string.h>
#include <vector>
//#include <sstream>
#include "Message.h"
#include "..\Sockets.h"
#include "..\Base64Encoding\Base64.h"
#include "..\FileSystem.h"

using namespace FileSystem;
//----------------<textmessage Class>--------------------------------------------

//constructor:
TextMessage::TextMessage() {}

//Copy Constructor:
TextMessage::TextMessage(const TextMessage& textmsg)
	:Addr(textmsg.Addr),port(textmsg.port),body(textmsg.body),type(textmsg.type){}

TextMessage::TextMessage(const std::string& Address, const int& tport, const std::string& msgbody, const std::string& msgtype)
	: Addr(Address), port(tport), body(msgbody), type(msgtype){}

//Assignment Operator:
TextMessage& TextMessage::operator=(const TextMessage& textmsg)
{
   if(this==&textmsg)
	   return *this;
   Addr=textmsg.Addr;
   port=textmsg.port;
   body=textmsg.body;
   type=textmsg.type;
   return *this;
}

// Serilize all the data information into a string 
std::string TextMessage::WriteToString()
{
	std::string msg = "BEGIN\n"+type+"\n"+Addr+"\n"+
		::ToString(port)+"\n"+body+"\n"+"END\n";
	return msg;
}

// Deserilize from a string to extract all the data information
void TextMessage::ReadFromString(std::string Msg)
{
   char * token, *next_token;
	std::vector<char> table(Msg.size() + 1);
	std::copy(Msg.begin(), Msg.end(), table.begin());
	token = strtok_s(&table[0],"\n",&next_token);
	int i=0; 
	while (token != NULL)
	{
		  if(i==1)
		      { 
				std::string s(token);
			    type = s;
			   }
		  else if(i==2)
		      {
			    std::string s(token);
			    Addr=s;
			  }
		else if(i==3)
			   {
				  std::string s(token);
				  port=FromString<int>(s);
				}
		else if (i==4)
				{
				   std::string s(token);
				   body = s;
				}
			token = strtok_s (NULL,"\n", &next_token);
			i++;
		}
    }

// Message's polymorphic send process
void TextMessage:: Send_Message(Socket sckt)
{
	std::string txtmsg=WriteToString();
	size_t len =txtmsg.size()+1;
	std::string InitialLine = "Post " + type + " HttpStyle: "+ ToString(len);
	sckt.writeLine(InitialLine);
	 const char* pBuf = new char [body.size() + 1];
			     pBuf = txtmsg.c_str();
	sckt.sendAll(pBuf,len);

}

//Message's polymorphic receive process
 void TextMessage:: ReceMessage(Socket sckt, size_t length)
 {
    const int BUFSIZE = 1024;
	char body[BUFSIZE] = {'\0'};
	sckt.recvAll(body, length);
	std::string msg = body;
	TextMessage::ReadFromString(msg);
	std::cout<<body<<type;
}

//---------<FileMessgae Class>-----------------------------------------

FileMessage::FileMessage() {}

FileMessage::FileMessage(const FileMessage& filemsg)
	:Addr(filemsg.Addr),port(filemsg.port),type(filemsg.type),filename(filemsg.filename){}

FileMessage::FileMessage(const std::string& address, const int& tport, const std::string& msgtype, /*const std::string path,*/ const std::string& name)
	:Addr(address), port(tport),type(msgtype),filename(name){}


FileMessage& FileMessage::operator=(const FileMessage& filemsg)
{
   if(this==&filemsg)
	   return *this;
   Addr=filemsg.Addr;
   port=filemsg.port;
   type=filemsg.type;
   filename=filemsg.filename;
   return *this;
}

std::string FileMessage::WriteToString()
{
 std::string msg = "BEGIN\n"+type+"\n"+Addr+"\n"+
		::ToString(port)+"\n"+filename+"\n"+"END\n";
	return msg;

}

void FileMessage::ReadFromString(std::string Msg)
{
	char * token, *next_token;
	std::vector<char> table(Msg.size() + 1);
	std::copy(Msg.begin(), Msg.end(), table.begin());
	token = strtok_s(&table[0],"\n",&next_token);
	int i=0; 
	while (token != NULL)
	{
		if(i==1)
		{
			std::string s(token);
			type =s;
		}
		else if(i==2)
		{
			std::string s(token);
			Addr=s;
		}
		else if(i==3)
		{
			std::string s(token);
			port=FromString<int>(s);
		}
		else if (i==4)
		{
		   std::string s(token);
		   filename = s;
	    }
			token = strtok_s (NULL,"\n", &next_token);
			i++;
		}
}

void FileMessage::Send_Message(Socket sckt)
{
	std::string filemsg = FileMessage::WriteToString();
	size_t len= filemsg.size();
	std::string InitialLine = "Post " + type + " HttpStyle: "+ ToString(len);
	sckt.writeLine(InitialLine);
	const char* pBuf1 = new char [len]; pBuf1 = filemsg.c_str();
	sckt.sendAll(pBuf1,filemsg.size());
	const int blocksize=1024;
	char content[blocksize]={"\0"};
	bool EndofFile = false;
	std::string block = Base64::encodeFromFile(filename);
    std::vector<char> vmsg(block.begin(), block.end());  //encode the chunked message
	size_t indexBegin, indexEnd , tempEnd;
		indexBegin = 0;
		indexEnd = vmsg.size();
		std::string body;
		while (true)
		{
			body="";
			tempEnd = indexBegin + blocksize;
			if ( indexBegin < indexEnd )
			{
				EndofFile= false;
				if ( tempEnd >= indexEnd )
				{
					tempEnd = indexEnd;
					EndofFile= true;;
				}
				for ( ; indexBegin< tempEnd ; indexBegin++ )
				{
					body += vmsg[indexBegin]; 
				}
             }  
			    const char* pBuf = new char [body.size() + 1];
			    pBuf = body.c_str();
			    std::string size_of_block=ToString(body.size()+1);
				sckt.writeLine(size_of_block);
			//	size_t size = FromString<size_t>(size_of_block);
				sckt.sendAll(pBuf,body.size());
				//sckt.sendAll(pBuf,size);
				indexBegin = tempEnd;
			    if(EndofFile == true) break;
		}
}

void FileMessage::ReceMessage(Socket sckt,size_t length)
{
	//int Targetport=8080;
	//SocketListener Listener(Targetport);
	//Socket skt = Listener.waitForConnect();
	const int buffsize = 1024;
	char body[buffsize] = {'\0'};
	sckt.recvAll(body,length);
	std::string filemessage = body;
	FileMessage::ReadFromString(filemessage);
	std::cout<<filename<<type;
	std::string linesize = sckt.readLine();
	TempBlock fileblock;
	size_t size = FromString<size_t>(linesize);
	sckt.recvAll(body,size);
	fileblock.content=body;
	Container.push_back(fileblock); //store the chunked file blocks
	if(length!=1024)
	{
		std::string tempname = Path::getName(filename);
		std::string filename=tempname.substr(0,tempname.find("."));
        std::string tempextension = tempname.substr(tempname.find(".")+1);
		std::string extension = tempextension.substr(0,tempextension.find("\n"));
		std::string receivefile = filename+"."+extension;
		std::string msgcontent;
		for(std::vector<TempBlock>::iterator i=Container.begin();i!=Container.end();i++)
			{
						msgcontent +=i->content;
			}
		Base64::decodeToFile(receivefile,msgcontent);
	}
}

#ifdef TEST_MESSAGE
int main()
{
     try
	{
		Sleep(1000);
		//TextMessage textMsg("127.0.0.1", 8080,"a text message", "text");
		std::string filename = "test.txt";
		std::string temp(Path::getFullFileSpec(filename));
	    bool Exis = File::exists(temp);
		if(Exis)
		{
		FileMessage fileMsg("127.0.0.1", 8080,"file",temp);
		Socket skt;
		SocketSystem sys;
		std::string addr = sys.getLocalIP();
		skt.connect(addr, 8080);
		while (skt.error());
		fileMsg.Send_Message(skt);
		skt.disconnect();
		}
	 }
	catch (std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}

	system("pause");
	return 0;
	
}

#endif