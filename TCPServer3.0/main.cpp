#include "MyServer.h"
#include "XSignal.h"

#ifndef _WIN32
#include <signal.h>
#endif

XSignal _gSignal;

//命令线程
void CmdThread(XServer* pServer)
{
	while (true)
	{
		char buffer[32] = {};
		scanf("%s", buffer);
		if (0 == strcmp(buffer, "exit"))
		{
			pServer->Stop();
			//唤醒同步信号
			_gSignal.Wake();
			break;
		}
		else if (0 == strcmp(buffer, "Exit"))
		{
			pServer->Stop();
			//唤醒同步信号
			_gSignal.Wake();
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
	server->Start();

	//开启命令线程
	std::thread cmdThread(CmdThread, server);
	cmdThread.detach();

	//设置同步信号-等在子线程任务执行完成再退出。否则子线程对象被释放会出现内存错误。
	_gSignal.Sleep();

	//运行服务器
	server->OnRun();

	//等待同步信号
	_gSignal.Wait();
	return 0;
}