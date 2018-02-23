#include <iostream>
#include <thread>
#include <chrono>
#include "Client.h"

const int cCount = 100;
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
		client[i]->Connect("192.168.0.99", 9090);
	}

	MsgLogin login;
	memcpy(login._Name, "illidan", sizeof("illidan"));
	memcpy(login._Pwd, "12345", sizeof("12345"));

	while (bRun)
	{
		for (int i = begin; i < end; ++i)
		{
			client[i]->SendData(&login);
			client[i]->OnRun();
		}
	}

	for (int i = begin; i < end; ++i)
	{
		client[i]->Close();
		client[i]->Done();
	}
}

int main()
{
	//创建1000个客户端
	for (int i = 0; i < cCount; ++i)
	{
		client[i] = new Client;
		client[i]->Init();
		client[i]->Open();
	}

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

	return 0;
}
