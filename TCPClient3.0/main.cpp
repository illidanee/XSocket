#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include "Client.h"

#include "XTimer.h"

XTimer timer;

const int mCount = 10;
const int cCount = 10000;
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

void RecvThread(int begin, int end)
{
	while (bRun)
	{
		for (int i = begin; i < end; ++i)
		{
			client[i]->OnRun();
		}
		//std::this_thread::sleep_for(std::chrono::microseconds(1));
	}
}

void SendThread(int begin, int end)
{
	MsgHeart msg[mCount];
	//for (int i = 0; i < mCount; ++i)
	//{
	//	memcpy(login[i]._Name, "illidan", sizeof("illidan"));
	//	memcpy(login[i]._Pwd, "12345", sizeof("12345"));
	//}
	int len = sizeof(msg);

	while (bRun)
	{
		for (int i = begin; i < end; ++i)
		{
			if (client[i]->SendData(msg, len) >= 0)
				sendCount++;
		}
		//std::this_thread::sleep_for(std::chrono::microseconds(100000));
	}
}

void ClientThread(int id)
{
	//分段
	int begin = id * cCount / tCount;
	int end = begin + cCount / tCount;

	//创建客户端
	for (int i = begin; i < end; ++i)
	{
		client[i] = new Client();
	}

	//连接服务器
	for (int i = begin; i < end; ++i)
	{
		client[i]->Open();
		client[i]->Connect("192.168.0.99", 9090);
	}

	//使用原子计数器优化线程等待。
	readyCount++;
	while (readyCount < tCount)
	{
		std::this_thread::sleep_for(std::chrono::microseconds(1000));
	}

	//开启数据接收线程
	std::thread rRecv = std::thread(RecvThread, begin, end);
	rRecv.detach();

	//开始发送数据
	SendThread(begin, end);

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

	//打印信息
	while (bRun)
	{
		if (timer.GetTime() > 1.0)
		{
			printf("| send Num = %7d  |\n", (int)sendCount);
			sendCount = 0;
			timer.UpdateTime();
		}

		std::this_thread::sleep_for(std::chrono::microseconds(1));
	}

	Client::Done();
	timer.XDone();
	return 0;
}
