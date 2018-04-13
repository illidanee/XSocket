#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include "MyClient.h"
#include "../XSrc/Timer/XTimer.h"
#include "../XSrc/ByteStream/XSendByteStream.h"

XTimer timer;

const int cCount = 4;
const int tCount = 1;
bool bRun = true;
MyClient* client[cCount];

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
	}
}

void SendThread(int begin, int end)
{
	XSendByteStream s(1024);
	s.WriteInt8(1);
	s.WriteInt16(2);
	s.WriteInt32(3);
	s.WriteInt64(4);
	s.WriteFloat(5.6f);
	s.WriteDouble(7.8);
	char a[] = "hahah";
	s.WriteArray(a, strlen(a));
	int b[] = { 1, 3, 5 };
	s.WriteArray(b, sizeof(b));
	s.Finish(MSG_BYTESTREAM);

	MsgHeart msg;

	while (bRun)
	{
		for (int i = begin; i < end; ++i)
		{
			if (client[i]->SendData((MsgHeart*)s.GetBuffer()) >= 0)
				sendCount++;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(99));
	}

	return;
}

void ClientThread(int id)
{
	//分段
	int begin = id * cCount / tCount;
	int end = begin + cCount / tCount;

	//创建客户端
	for (int i = begin; i < end; ++i)
	{
		client[i] = new MyClient();
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
	XLog::SetFile("./Client.log", "w");

	//XInfo("---------------------------------------------------------------------------------------------------------------------------------------\n");
	//XInfo("                                                               C++ Client                                                              \n");
	//XInfo("                                                                                                        Designed by Org.illidan        \n");
	//XInfo("---------------------------------------------------------------------------------------------------------------------------------------\n");

	timer.XInit();

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

	timer.XDone();
	return 0;
}
