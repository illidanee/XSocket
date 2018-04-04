#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include "../XSrc/Server/XTCPClient.h"
#include "../XSrc/Timer/XTimer.h"

XTimer timer;

const int mCount = 1;
const int cCount = 1000;
const int tCount = 1;
bool bRun = true;
XTCPClient* client[cCount];

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
	std::chrono::time_point<std::chrono::high_resolution_clock> t1 = std::chrono::high_resolution_clock::now();
	while (bRun)
	{
		std::chrono::time_point<std::chrono::high_resolution_clock> t2 = std::chrono::high_resolution_clock::now();
		std::chrono::seconds t = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1);
		for (int i = begin; i < end; ++i)
		{
			//if (t.count() > 3.0f && i == begin)
			//	continue;
			client[i]->OnRun();
		}
	}
}

void SendThread(int begin, int end)
{
	MsgHeart msg[mCount];
	//for (int i = 0; i < mCount; ++i)
	//{
	//	memcpy(msg[i]._Name, "illidan", sizeof("illidan"));
	//	memcpy(msg[i]._Pwd, "12345", sizeof("12345"));
	//}
	int len = sizeof(msg);

	while (bRun)
	{
		for (int i = begin; i < end; ++i)
		{
			if (client[i]->SendData(msg, len) >= 0)
				sendCount++;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(99));
	}

	int a = 0;
	return;
}

void ClientThread(int id)
{
	//�ֶ�
	int begin = id * cCount / tCount;
	int end = begin + cCount / tCount;

	//�����ͻ���
	for (int i = begin; i < end; ++i)
	{
		client[i] = new XTCPClient();
	}

	//���ӷ�����
	for (int i = begin; i < end; ++i)
	{
		client[i]->Open();
		client[i]->Connect("192.168.0.99", 9090);
	}

	//ʹ��ԭ�Ӽ������Ż��̵߳ȴ���
	readyCount++;
	while (readyCount < tCount)
	{
		std::this_thread::sleep_for(std::chrono::microseconds(1000));
	}

	//�������ݽ����߳�
	std::thread rRecv = std::thread(RecvThread, begin, end);
	rRecv.detach();

	//��ʼ��������
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
	XTCPClient::Init();

	//���������߳�
	std::thread cmdThread(CmdThread);
	cmdThread.detach();

	//�����ͻ����߳�
	std::thread clientThread[tCount];
	for (int i = 0; i < tCount; ++i)
	{
		clientThread[i] = std::thread(ClientThread, i);
		clientThread[i].detach();
	}

	//��ӡ��Ϣ
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

	XTCPClient::Done();
	timer.XDone();
	return 0;
}
