#include "../XSrc/XCommon.h"
#include "../XSrc/XSignal.h"
#include "MyServer.h"

#ifndef _WIN32
#include <signal.h>
#endif

const char* ReadStrArgs(int argc, char* args[], int index, const char* def)
{
	if (index >= argc)
		XError("参数错误");

	return args[1];
}

int ReadIntArgs(int argc, char* args[], int index, int def)
{
	if (index >= argc)
		XError("参数错误");

	return atoi(args[1]);
}

int main(int argc, char* args[])
{
	//参数信息
	if (argc < 4)
	{
		return 0;
	}

	const char* ip = ReadStrArgs(argc, args, 1, "Any");
	short port = ReadIntArgs(argc, args, 2, 9090);
	int lqn = ReadIntArgs(argc, args, 3, 999);

	if (strcmp(ip, "Any") == 0)
		ip = nullptr;

	//日志信息
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
	server->Start(ip, port, lqn);

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