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

//ͳ�Ƽ���
std::atomic_int readyCount = 0;
std::atomic_int connectCount = 0;
std::atomic_int sendCount = 0;
std::atomic_int errorCount = 0;
std::atomic_int loopCount = 0;

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
	//�ֶ�
	std::vector<MyClient*> clients(g_cCount);

	//�����ͻ���
	for (int i = 0; i < g_cCount; ++i)
	{
		if (!pThread->IsRun())
			break;

		clients[i] = new MyClient();

		XThread::Sleep(0);
	}

	//���ӷ�����
	for (int i = 0; i < g_cCount; ++i)
	{
		if (!pThread->IsRun())
			break;
		
		if (clients[i]->Connect("192.168.0.99", 9090))
			connectCount++;

		XThread::Sleep(0);
	}

	//ʹ��ԭ�Ӽ������Ż��̵߳ȴ���
	readyCount++;
	while (readyCount < g_tCount)
	{
		XThread::Sleep(1);
	}

	MsgHeart heart;
	heart._ID = 123;
	strcpy(heart._Name, "LiLei");
	strcpy(heart._Pwd, "HanMeiMei");

	std::chrono::time_point<std::chrono::system_clock> t0 = std::chrono::system_clock::now();
	std::chrono::time_point<std::chrono::system_clock> t1 = std::chrono::system_clock::now();
	time_t tt = 0;



	while (pThread->IsRun())
	{
		for (int c = 0; c < g_msgCount; ++c)
		{
			for (int i = 0; i < g_cCount; ++i)
			{
				if (clients[i]->IsRun() && clients[i]->CanSend())
				{
					if (clients[i]->Send(&heart))
					{
						sendCount++;
					}
					else
					{
						errorCount++;
					}
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

		if (tt > g_time)
		{
			tt -= g_time;

			for (int i = 0; i < g_cCount; ++i)
			{
				if (clients[i]->IsRun())
				{
					clients[i]->ResetCount();
				}
			}	
		}

		loopCount++;
		XThread::Sleep(1);  //999��ֹ�������ݹ��죬д������������޷������������ݡ�
	}

	for (int i = 0; i < g_cCount; ++i)
	{
		clients[i]->Disconnect();
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

	//���������߳�
	XThread cmdThread;
	cmdThread.Start(
		nullptr, 
		[](XThread* pThread) {
			CmdThread(pThread);
		},
		nullptr);

	//�����ͻ����߳�
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

	//��ӡͳ����Ϣ
	std::chrono::time_point<std::chrono::system_clock> t0 = std::chrono::system_clock::now();
	std::chrono::time_point<std::chrono::system_clock> t1 = std::chrono::system_clock::now();
	time_t tt = 0;

	while (cmdThread.IsRun())
	{
		t1 = std::chrono::system_clock::now();
		std::chrono::microseconds dt = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0);
		t0 = t1;

		tt += dt.count();

		if (tt > g_time)
		{
			XInfo("| ConnectNum = %7d  | SendNum = %7d  | ErrorNum = %7d  | LoopNum = %7d  |\n", (int)connectCount,(int)sendCount, (int)errorCount, (int)loopCount);
			sendCount = 0;
			errorCount = 0;
			loopCount = 0;

			tt -= g_time;
		}

		XThread::Sleep(1);
	}

	//ֹͣ�ͻ����߳�
	for (int i = 0; i < g_tCount; ++i)
	{
		clientThreads[i]->Stop();
		delete clientThreads[i];
	}

	return 0;
}
