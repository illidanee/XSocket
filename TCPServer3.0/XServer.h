#ifndef __XSERVER_H__
#define __XSERVER_H__

#include "XCommon.h"
#include "XIEvent.h"
#include "XReceiveServer.h"
#include "XClient.h"
#include "XThread.h"
#include <vector>

//监听Server类
class XServer : public XIEvent
{
public:
	XServer();
	~XServer();

	int Start();
	int Stop();

private:
	SOCKET _Socket;												//服务器监听Socket
	std::vector<std::shared_ptr<XReceiveServer>> _AllServers;	//服务器信息

	XThread _Thread;											//任务线程

private:
	void Init();
	void Open();
	void Bind(const char* ip, unsigned short port);
	void Listen(int n);

	void Close();
	void Done();

	void Accept();

	void OnRun(XThread* pThread);
};


#endif