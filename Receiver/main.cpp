#include <string>
#include <iostream>
#include <vector>
#include"..\Channel\Channel.h"
#include "Receiver.h"
//#include "..\Locks.h"


int main()
{
	try
	{
	/*int Targetport=8080;
	SocketListener Listener(Targetport);
	
	Receiver rcvr(Listener.waitForConnect());
	rcvr.Receive();*/
	
	std::string targetAddress = "127.0.0.1";
	int targetPort = 8080;
	int listenPort = 8080;

	Channel* chan = new Channel(targetAddress, targetPort, listenPort);
	
	delete chan;
	}
	catch (std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}
    system("pause");
	return 0;

}