#ifndef __MYSERVER_H__
#define __MYSERVER_H__

#include "../XSrc/XCommon.h"
#include "../XSrc/Server/XTCPServer.h"
#include "../XSrc/ByteStream/XRecvByteStream.h"
#include "../XSrc/ByteStream/XSendByteStream.h"
#include <atomic>
#include <functional>

//自定义Server
class MyServer : public XTCPServer
{
private:
	XTimer _Timer;								//计时器
	std::atomic_int _ClientNum;					//客户端计数器
	std::atomic_int _RecvNum;					//recv()函数调用计数
	std::atomic_int _SendNum;					//send()函数调用计数
	std::atomic_int _RecvPackageNum;			//接收数据包计数器
	std::atomic_int _DonePackageNum;			//处理数据包计数器
	std::atomic_int _PackageNum;				//剩余任务包计数

public:
	MyServer();
	~MyServer();
	virtual void OnRunLoopBegin();
	virtual void OnClientJoin(XClient* pClient);
	virtual void OnClientLeave(XClient* pClient);
	virtual void OnNetRecv(XClient* pClient);
	virtual void OnNetSend(XClient* pClient);
	virtual void OnNetMsgRecv(XClient* pClient, MsgHeader* pMsgHeader);
	virtual void OnNetMsgDone(XClient* pClient, MsgHeader* pMsgHeader);
};

#endif
