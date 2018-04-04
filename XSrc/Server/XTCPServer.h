#ifndef __XSERVER_H__
#define __XSERVER_H__

#include "../XCommon.h"
#include "XIGlobalEvent.h"
#include "XServer.h"
#include "XClient.h"
#include "XNet.h"
#include "../Thread/XThread.h"
#include <vector>

//监听Server类
class XTCPServer : public XIGlobalEvent
{
public:
	XTCPServer();
	~XTCPServer();

	int Start();
	int Stop();

private:
	SOCKET _Socket;												//服务器监听Socket
	std::vector<std::shared_ptr<XServer>> _AllServers;			//服务器信息

	XThread _Thread;											//任务线程

private:
	void Open();
	void Bind(const char* ip, unsigned short port);
	void Listen(int n);
	void Close();

	void Accept();

	void OnRun(XThread* pThread);
};


#endif