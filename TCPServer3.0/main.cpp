#include "../XSrc/XCommon.h"
#include "../XSrc/XConfig.h"
#include "../XSrc/XSignal.h"
#include "MyServer.h"

#ifndef _WIN32
#include <signal.h>
#endif

int main(int argc, char* args[])
{

//关闭中断信号，禁止中断服务器进程。
#ifndef _WIN32
	signal(SIGPIPE, SIG_IGN);
#endif

	//日志信息
	XLog::SetFileName("./server.log", "w");

	XInfo("---------------------------------------------------------------------------------------------------------------------------------------\n");
	XInfo("                                                               C++ Server                                                              \n");
	XInfo("                                                                                                        Designed by Org.illidan        \n");
	XInfo("---------------------------------------------------------------------------------------------------------------------------------------\n");

	//解析参数。
	XConfig::Instance().Init(argc, args);

	const char* ip = XConfig::Instance().GetStringArg("ip", nullptr);
	short port = XConfig::Instance().GetIntArg("port", 9090);
	int lqn = XConfig::Instance().GetIntArg("lqn", 1000);

	XInfo("Configure Infos: \n");
	XInfo("         %8s    =    %-16s\n", "IP", ip);
	XInfo("         %8s    =    %-16d\n", "Port", port);
	XInfo("         %8s    =    %-16d\n", "Lqn", lqn);

	if (ip && strcmp("Any", ip) == 0)
	{
		ip = nullptr;
	}

	//创建服务器
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