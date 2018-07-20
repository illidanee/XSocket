#include "../XSrc/XCommon.h"
#include "../XSrc/XTimer.h"
#include "../XSrc/XSendByteStream.h"
#include "../XSrc/XThread.h"
#include "MyClient.h"
#include <vector>
#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>

const int g_tCount = 1;
const int g_cCount = 10000;

std::atomic_int readyCount = 0;
std::atomic_int connectCount = 0;
std::atomic_int sendCount = 0;

void CmdThread(XThread* pThread)
{
	while (pThread->IsRun())
	{
		char buffer[32] = {};
		scanf("%s", buffer);
		if (0 == strcmp(buffer, "Exit"))
		{
			pThread->Exit();
			break;
		}
	}
}

void ClientThread(XThread* pThread, int id)
{
	//分段
	std::vector<MyClient*> clients(g_cCount);

	//创建客户端
	for (int i = 0; i < g_cCount; ++i)
	{
		if (!pThread->IsRun())
			break;

		clients[i] = new MyClient();

		pThread->Sleep(0);
	}

	//连接服务器
	for (int i = 0; i < g_cCount; ++i)
	{
		if (!pThread->IsRun())
			break;
		
		clients[i]->Open();
		clients[i]->Connect("192.168.0.99", 9091);
		connectCount++;

		pThread->Sleep(0);
	}

	//使用原子计数器优化线程等待。
	readyCount++;
	while (readyCount < g_tCount)
	{
		pThread->Sleep(0);
	}

	MsgHeart heart;
	heart.no = 123;
	strcpy(heart.name, "LiLei");
	strcpy(heart.pwd, "HanMeiMei");

	std::chrono::time_point<std::chrono::system_clock> t0 = std::chrono::system_clock::now();
	std::chrono::time_point<std::chrono::system_clock> t1 = std::chrono::system_clock::now();
	time_t tt = 0;

	while (pThread->IsRun())
	{
		for (int i = 0; i < g_msgCount; ++i)
		{
			for (int i = 0; i < g_cCount; ++i)
			{
				if (clients[i]->IsRun())
				{
					if (clients[i]->Send(&heart))
						sendCount++;
				}
			}
		}

		for (int i = 0; i < g_cCount; ++i)
		{
			if (clients[i]->IsRun())
			{
				clients[i]->OnRun();
			}
		}

		t1 = std::chrono::system_clock::now();
		std::chrono::microseconds dt = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0);
		t0 = t1;

		tt += dt.count();

		if (tt > 1000000)
		{
			for (int i = 0; i < g_cCount; ++i)
			{
				if (clients[i]->IsRun())
				{
					clients[i]->ResetCount();
				}
			}

			tt -= 1000000;
		}

		pThread->Sleep(1);  //999防止发送数据过快，写满缓冲区造成无法继续发送数据。
	}

	for (int i = 0; i < g_cCount; ++i)
	{
		clients[i]->Disconnect();
		clients[i]->Close();
		delete clients[i];
	}
}

int main()
{
	XLog::SetFileName("./client.log", "w");

	XInfo("---------------------------------------------------------------------------------------------------------------------------------------\n");
	XInfo("                                                               C++ Client                                                              \n");
	XInfo("                                                                                                        Designed by Org.illidan        \n");
	XInfo("---------------------------------------------------------------------------------------------------------------------------------------\n");

	//启动命令线程
	XThread cmdThread;
	cmdThread.Start(
		nullptr, 
		[](XThread* pThread) {
			CmdThread(pThread);
		},
		nullptr);

	//启动客户端线程
	std::vector<XThread*> clientThreads;
	for (int i = 0; i < g_tCount; ++i)
	{
		XThread* pThread = new XThread();
		pThread->Start(
			nullptr,
			[i](XThread* pThread) {
				ClientThread(pThread, i);
			},
			nullptr);

		clientThreads.push_back(pThread);
	}

	//打印统计信息
	std::chrono::time_point<std::chrono::system_clock> t0 = std::chrono::system_clock::now();
	std::chrono::time_point<std::chrono::system_clock> t1 = std::chrono::system_clock::now();
	time_t tt = 0;

	while (cmdThread.IsRun())
	{
		t1 = std::chrono::system_clock::now();
		std::chrono::microseconds dt = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0);
		t0 = t1;

		tt += dt.count();

		if (tt > 1000000)
		{
			XInfo("| send Num = %7d  |\n", (int)sendCount);
			sendCount = 0;

			tt -= 1000000;
		}

		cmdThread.Sleep(1);
	}

	//停止客户端线程
	for (int i = 0; i < g_tCount; ++i)
	{
		clientThreads[i]->Stop();
		delete clientThreads[i];
	}

	return 0;
}
