#ifndef __XCLIENT_H__
#define __XCLIENT_H__

#include "XCommon.h"
#include "XIEvent.h"
#include "XReceiveServer.h"

//客户端信息类
class XClient : public XObject<1024, XClient>
{
public:
	XClient(SOCKET client);
	~XClient();

	void Init(XIEvent* pEventObj, XReceiveServer* pReceiveServerObj);

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
	//关联对象
	XIEvent* _pNetEventObj;						//主线程对象
	XReceiveServer* _pReceiveServerObj;			//ReceiveServer对象

	SOCKET _Socket;								//客户端Socket
	char _RecvBuffer[_RECV_BUFFER_SIZE_];		//接收缓冲区
	int _RecvStartPos;							//接收缓冲区中可以放入数据的起始位置
	char _SendBuffer[_SEND_BUFFER_SIZE_];		//发送缓冲区
	int _SendStartPos;							//发送缓冲区中可以放入数据的起始位置

private:
	time_t	_HeartTime;							//心跳计时器
	time_t	_SendTime;							//定时发送数据计时器
};


#endif