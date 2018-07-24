#ifndef __XCLIENT_H__
#define __XCLIENT_H__

#include "XCommon.h"
#include "XIGlobalEvent.h"
#include "XIServerEvent.h"
#include "XBuffer.h"
#include "XByteStream.h"

//客户端信息类
class XClient : public XObject<10240, XClient>, public std::enable_shared_from_this<XClient>
{
public:
	XClient(SOCKET client, XIGlobalEvent* pGlobalObj, XIServerEvent* pServerObj, int nHeartTime = _XCLIENT_HEART_TIME_, int nSendTime = _XCLIENT_SEND_TIME_, int nRecvBufferSize = _XCLIENT_RECV_BUFFER_SIZE_, int nSendBufferSize = _XCLIENT_SEND_BUFFER_SIZE_);
	~XClient();

	SOCKET GetSocket() { return _Socket; }

	int RecvData();
	int SendData(MsgHeader* pHeader);
	int SendStream(XByteStream* pByteStream);
	int SendData();

	bool HasMsg();
	bool HasData();

	//心跳
	void ResetHeartTime();
	bool CheckHeartTime(time_t t);

	//定时发送数据
	void ResetSendTime();
	void CheckSendTime(time_t t);

	XIGlobalEvent* GetGlobalObj();
	XIServerEvent* GetServerObj();

	//获取自身的shared_prt
	std::shared_ptr<XClient> GetSharedPtr();

private:
	SOCKET _Socket;								//客户端Socket

	XIGlobalEvent* _pGlobalObj;					//全局对象
	XIServerEvent* _pServerObj;					//服务对象

	time_t _CurHeartTime;						//心跳计时器
	time_t _HeartTime;							//心跳总时间
	time_t _CurSendTime;						//发送计时器
	time_t _SendTime;							//发送总时间

	XBuffer _RecvBuffer;						//接收缓冲区对象
	XBuffer _SendBuffer;						//发送缓冲区对象

public:
	int _CurMsgID;								//当前消息ID
};


#endif