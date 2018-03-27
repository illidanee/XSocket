#ifndef __XSERVER_H__
#define __XSERVER_H__

#include "XCommon.h"
#include "XIEvent.h"
#include "XReceiveServer.h"
#include "XClient.h"
#include "XSignal.h"
#include <vector>

//监听Server类
class XServer : public XIEvent
{
public:
	XServer();
	~XServer();

	int Start();
	int OnRun();
	int Stop();

private:
	SOCKET _Socket;												//服务器监听Socket
	std::vector<std::shared_ptr<XReceiveServer>> _AllServers;	//服务器信息

	bool _Run;													//当前线程是否运行
	XSignal _Signal;											//同步信号

private:
	int Init();
	int Done();

	int Open();
	int Close();

	int Bind(const char* ip, unsigned short port);
	int Listen(int n);
	int Accept();
};


#endif