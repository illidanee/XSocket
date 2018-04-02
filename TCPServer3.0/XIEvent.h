﻿#ifndef __XIEVENT_H__
#define __XIEVENT_H__

//服务器全局事件接口。
struct MsgHeader;
class XClient;
class XReceiveServer;

class XIEvent
{
public:
	virtual void OnRunLoopBegin() = 0;
	virtual void OnClientJoin(std::shared_ptr<XClient> pClient) = 0;
	virtual void OnClientLeave(std::shared_ptr<XClient> pClient) = 0;
	virtual void OnNetRecv(XClient* pClient) = 0;
	virtual void OnNetSend(XClient* pClient) = 0;
	virtual void OnNetMsgRecv(XClient* pClient, MsgHeader* pMsgHeader, XReceiveServer* pReceiveServer) = 0;
	virtual void OnNetMsgDone(XClient* pClient, MsgHeader* pMsgHeader, XReceiveServer* pReceiveServer) = 0;
};

#endif