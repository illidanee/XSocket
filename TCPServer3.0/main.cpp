#include "MyServer.h"

#ifndef _WIN32
#include <signal.h>
#endif

//命令线程
void CmdThread(XServer* pServer)
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
//去除中断信号关闭服务器。
#ifndef _WIN32
	sigset_t signal_mask;
	sigemptyset(&signal_mask);
	sigaddset(&signal_mask, SIGPIPE);
	if (pthread_sigmask(SIG_BLOCK, &signal_mask, NULL) == -1)
		perror("SIGPIPE");
#endif

	//创建服务器
	//int nSize = sizeof(MyServer);
	MyServer* server = new MyServer;

	//开启服务器
	server->Init();
	server->Open();
	server->Bind(NULL, 9090);
	server->Listen(1000);
	server->Start();

	//开启命令线程
	std::thread cmdThread(CmdThread, server);
	cmdThread.detach();

	//运行服务器
	server->OnRun();

	//关闭服务器
	server->Close();

	return 0;
}