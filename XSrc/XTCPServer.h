#ifndef __XSERVER_H__
#define __XSERVER_H__

#include "XCommon.h"
#include "XConfig.h"
#include "XIGlobalEvent.h"
#include "XNet.h"
#include "XServer.h"
#include "XClient.h"
#include "XThread.h"
#include <vector>

//监听Server类
class XTCPServer : public XIGlobalEvent
{
public:
	XTCPServer();
	~XTCPServer();

	int Init();
	int Start();
	int Stop();

private:
	SOCKET _Socket;												//服务器监听Socket
	std::vector<std::shared_ptr<XServer>> _AllServers;			//服务器信息

	XThread _Thread;											//任务线程

//配置参数
private:
	const char* _IP;
	short _Port;
	int _LQN;
	int _ServerThreadSize;										//服务器线程数量
	int _ClientHeartTime;										//客户端心跳计时
	int _ClientSendTime;										//客户端发送计时
	int _ClientRecvBufferSize;									//客户端接收缓冲区大小
	int _ClientSendBufferSize;									//客户端发送缓冲区大小

private:
	void Open();
	void Bind(const char* ip, unsigned short port);
	void Listen(int n);
	void Close();

	void Accept();

	void OnRun(XThread* pThread);
};


#endif