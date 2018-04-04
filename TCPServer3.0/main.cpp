#include "../XSrc/XCommon.h"
#include "../XSrc/Signal/XSignal.h"
#include "MyServer.h"

#ifndef _WIN32
#include <signal.h>
#endif

int main()
{
	XLog::SetFile("./Server.log", "w");

	XInfo("---------------------------------------------------------------------------------------------------------------------------------------\n");
	XInfo("                                                               C++ Server                                                              \n");
	XInfo("                                                                                                        Designed by Org.illidan        \n");
	XInfo("---------------------------------------------------------------------------------------------------------------------------------------\n");

//去除中断信号关闭服务器。
#ifndef _WIN32
	//sigset_t signal_mask;
	//sigemptyset(&signal_mask);
	//sigaddset(&signal_mask, SIGPIPE);
	//if (pthread_sigmask(SIG_BLOCK, &signal_mask, NULL) == -1)
	//	perror("SIGPIPE");
	signal(SIGPIPE, SIG_IGN);
#endif

	//创建服务器
	//int nSize = sizeof(MyServer);
	MyServer* server = new MyServer;

	//开启服务器
	server->Start();

	while (true)
	{
		char buffer[32] = {};
		scanf("%s", buffer);
		if (0 == strcmp(buffer, "exit"))
		{
			server->Stop();
			break;
		}
		else if (0 == strcmp(buffer, "Exit"))
		{
			server->Stop();
			break;
		}
	}

	return 0;
}