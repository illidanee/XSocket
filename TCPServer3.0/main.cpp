#include <thread>
#include "Server.h"

#ifndef _WIN32
#include <signal.h>
#endif

//�����߳�
void CmdThread(_ListenServer* pServer)
{
	while (true)
	{
		char buffer[32] = {};
		scanf("%s", buffer);
		if (0 == strcmp(buffer, "Exit"))
		{
			pServer->Close();
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
	MainServer* server = new MainServer;

	//����������
	server->Init();
	server->Open();
	server->Bind(NULL, 9090);
	server->Listen(1000);
	server->Start();

	std::thread cmdThread(CmdThread, server);
	cmdThread.detach();

	//���з�����
	server->OnRun();

	//�رշ�����
	server->Close();

	return 0;
}