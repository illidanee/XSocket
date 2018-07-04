#include "../XSrc/XCommon.h"
#include "../XSrc/XConfig.h"
#include "../XSrc/XSignal.h"
#include "MyServer.h"

#ifndef _WIN32
#include <signal.h>
#endif

const char* ReadStrArgs(int argc, char* args[], int index, const char* def)
{
	if (index >= argc)
		XError("参数错误");

	return args[index];
}

int ReadIntArgs(int argc, char* args[], int index, int def)
{
	if (index >= argc)
		XError("参数错误");

	return atoi(args[index]);
}

int main(int argc, char* args[])
{
	//解析参数。
	XConfig::Instance().Init(argc, args);
	const char* ip = XConfig::Instance().GetStringArg("ip", nullptr);
	short port = XConfig::Instance().GetIntArg("port", 9090);
	int lqn = XConfig::Instance().GetIntArg("lqn", 1000);

	//日志信息
	XLog::GetInstance();
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