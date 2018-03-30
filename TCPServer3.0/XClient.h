#ifndef __XCLIENT_H__
#define __XCLIENT_H__

#include "XCommon.h"
#include "XIEvent.h"
#include "XReceiveServer.h"
#include "XBuffer.h"

//客户端信息类
class XClient : public XObject<1024, XClient>
{
public:
	XClient(SOCKET client, XIEvent* pEventObj, XReceiveServer* pReceiveServerObj);
	~XClient();

	SOCKET GetSocket() { return _Socket; }

	int RecvData();
	int SendData(MsgHeader* pHeader);
	int SendData();

	//心跳
	void ResetHeartTime();
	bool CheckHeartTime(time_t t);

	//定时发送数据
	void ResetSendTime();
	void CheckSendTime(time_t t);

private:
	SOCKET _Socket;								//客户端Socket

	XIEvent* _pNetEventObj;						//主线程对象
	XReceiveServer* _pReceiveServerObj;			//ReceiveServer对象

	XBuffer _RecvBuffer;						//接收缓冲区对象
	XBuffer _SendBuffer;						//发送缓冲区对象

private:
	time_t	_HeartTime;							//心跳计时器
	time_t	_SendTime;							//定时发送数据计时器
};


#endif