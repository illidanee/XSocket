#include <iostream>
#include <thread>
#include <chrono>
#include "Client.h"

const int cCount = 10000;
const int tCount = 4;
bool bRun = true;
Client* client[cCount];


void CmdThread()
{
	while (true)
	{
		char buffer[32] = {};
		scanf("%s", buffer);
		if (0 == strcmp(buffer, "Exit"))
		{
			bRun = false;
			break;
		}
	}
}

void ClientThread(int id)
{
	int begin = id * cCount / tCount;
	int end = begin + cCount / tCount;

	for (int i = begin; i < end; ++i)
	{
		client[i] = new Client();
		client[i]->Open();
	}

	for (int i = begin; i < end; ++i)
	{
		client[i]->Connect("192.168.0.99", 9090);
	}
	
	std::this_thread::sleep_for(std::chrono::seconds(5));

	MsgLogin login[10];
	for (int i = 0; i < 10; ++i)
	{
		memcpy(login[i]._Name, "illidan", sizeof("illidan"));
		memcpy(login[i]._Pwd, "12345", sizeof("12345"));
	}
	int len = sizeof(login);

	while (bRun)
	{
		for (int i = begin; i < end; ++i)
		{
			client[i]->SendData(login, len);
		}
	}

	for (int i = begin; i < end; ++i)
	{
		client[i]->Close();
		delete client[i];
	}
}

int main()
{
	Client::Init();

	//启动命令线程
	std::thread cmdThread(CmdThread);
	cmdThread.detach();

	//启动客户端线程
	std::thread clientThread[tCount];
	for (int i = 0; i < tCount; ++i)
	{
		clientThread[i] = std::thread(ClientThread, i);
	}

	for (int i = 0; i < tCount; ++i)
	{
		clientThread[i].join();
	}

	while (bRun)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	Client::Done();
	return 0;
}
