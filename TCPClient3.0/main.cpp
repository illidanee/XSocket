#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include "Client.h"

#include "XTimer.h"

XTimer timer;

const int mCount = 1000;
const int cCount = 40;
const int tCount = 4;
bool bRun = true;
Client* client[cCount];

std::atomic_int readyCount = 0;
std::atomic_int sendCount = 0;

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
	
	//使用原子计数器优化线程等待。
	readyCount++;
	while (readyCount < tCount)
	{
		std::this_thread::sleep_for(std::chrono::microseconds(1000));
	}

	MsgLogin login[mCount];
	for (int i = 0; i < mCount; ++i)
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
			client[i]->OnRun();
			sendCount++;
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
	timer.XInit();
	Client::Init();

	//启动命令线程
	std::thread cmdThread(CmdThread);
	cmdThread.detach();

	//启动客户端线程
	std::thread clientThread[tCount];
	for (int i = 0; i < tCount; ++i)
	{
		clientThread[i] = std::thread(ClientThread, i);
		clientThread[i].detach();
	}

	while (bRun)
	{
		if (timer.GetTime() > 1.0)
		{
			printf("| send Num = %7d  |\n", (int)sendCount);
			sendCount = 0;
			timer.UpdateTime();
		}

		std::this_thread::sleep_for(std::chrono::microseconds(1000));
	}

	Client::Done();
	timer.XDone();
	return 0;
}
