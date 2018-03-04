#include <thread>
#include "Server.h"

#ifndef _WIN32
#include <signal.h>
#endif

//命令线程
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

//自定义Server
class MyServer : public MainServer
{
private:
	XTimer _Timer;								//计时器
	std::atomic_int _ClientNum;					//客户端计数器
	std::atomic_int _PackageNum;				//接收数据包计数器

public:
	MyServer()
	{
		//初始化其他
		_Timer.XInit();
		_ClientNum = 0;
		_PackageNum = 0;
	}
	~MyServer()
	{
		_Timer.XDone();
	}
	virtual void OnRunBegin()
	{
		if (_Timer.GetTime() > 1.0)
		{
			printf("| Client Num = %6d  | Package Num = %6d  |\n", (int)_ClientNum, (int)_PackageNum);
			_PackageNum = 0;
			_Timer.UpdateTime();
		}
	}
	virtual void OnClientJoin(_Client* pClient)
	{
		++_ClientNum;
	}
	virtual void OnClientLeave(_Client* pClient)
	{
		--_ClientNum;
	}
	virtual void OnNetMsg(_Client* pClient)
	{
		++_PackageNum;
	}
};

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
	MyServer* server = new MyServer;

	//开启服务器
	server->Init();
	server->Open();
	server->Bind(NULL, 9090);
	server->Listen(1000);
	server->Start();

	std::thread cmdThread(CmdThread, server);
	cmdThread.detach();

	//运行服务器
	server->OnRun();

	//关闭服务器
	server->Close();

	return 0;
}