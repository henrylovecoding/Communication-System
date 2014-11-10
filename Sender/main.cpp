#include<iostream>
#include<thread>
#include"..\Sender\Sender.h"
#include "..\FileSystem.h"
using namespace FileSystem;
int main()
{
	try
	{
	Sleep(500);
	Sender sdnr("127.0.0.1",8080);
    Message* textmsg = new TextMessage("127.0.0.1", 8080,"a text message", "text");
	std::string filename = "test.txt";
	std::string temp(Path::getFullFileSpec(filename));
	Message* filemsg = new FileMessage("127.0.0.1", 8080,"file",temp);
	Message* textmsg2 = new TextMessage("127.0.0.1", 8080,"another text message", "text");

	sdnr.PostMsgToQueue(filemsg);
	sdnr.PostMsgToQueue(textmsg);
	std::thread SendThrd(&Sender::Send, &sdnr);
	SendThrd.join();

	}
	catch (std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}
	system("Pause");
    return 0;

}