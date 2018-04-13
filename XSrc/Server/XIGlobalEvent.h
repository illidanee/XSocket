#ifndef __XIGLOBALEVENT_H__
#define __XIGLOBALEVENT_H__

//全局事件接口。
struct MsgHeader;
class XClient;

class XIGlobalEvent
{
public:
	virtual void OnRunLoopBegin() = 0;
	virtual void OnClientJoin(XClient* pClient) = 0;
	virtual void OnClientLeave(XClient* pClient) = 0;
	virtual void OnNetRecv(XClient* pClient) = 0;
	virtual void OnNetSend(XClient* pClient) = 0;
	virtual void OnNetMsgRecv(XClient* pClient, MsgHeader* pMsgHeader) = 0;
	virtual void OnNetMsgDone(XClient* pClient, MsgHeader* pMsgHeader) = 0;
};

#endif