#include "MyServer.h"
#include "XSignal.h"

#ifndef _WIN32
#include <signal.h>
#endif

XSignal _gSignal;

//�����߳�
void CmdThread(XServer* pServer)
{
	while (true)
	{
		char buffer[32] = {};
		scanf("%s", buffer);
		if (0 == strcmp(buffer, "exit"))
		{
			pServer->Stop();
			//����ͬ���ź�
			_gSignal.Wake();
			break;
		}
		else if (0 == strcmp(buffer, "Exit"))
		{
			pServer->Stop();
			//����ͬ���ź�
			_gSignal.Wake();
			break;
		}
	}
}

int main()
{
//ȥ���ж��źŹرշ�������
#ifndef _WIN32
	sigset_t signal_mask;
	sigemptyset(&signal_mask);
	sigaddset(&signal_mask, SIGPIPE);
	if (pthread_sigmask(SIG_BLOCK, &signal_mask, NULL) == -1)
		perror("SIGPIPE");
#endif

	//����������
	//int nSize = sizeof(MyServer);
	MyServer* server = new MyServer;

	//����������
	server->Start();

	//���������߳�
	std::thread cmdThread(CmdThread, server);
	cmdThread.detach();

	//����ͬ���ź�-�������߳�����ִ��������˳����������̶߳����ͷŻ�����ڴ����
	_gSignal.Sleep();

	//���з�����
	server->OnRun();

	//�ȴ�ͬ���ź�
	_gSignal.Wait();
	return 0;
}