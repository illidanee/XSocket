#ifndef __MYCLIENT_H__
#define __MYCLIENT_H__

#include "../XSrc/Server/XTCPClient.h"
#include "../XSrc/ByteStream/XRecvByteStream.h"

class MyClient : public XTCPClient
{
	virtual void OnRunLoopBegin();
	virtual void OnClientJoin(XClient* pClient);
	virtual void OnClientLeave(XClient* pClient);
	virtual void OnNetRecv(XClient* pClient);
	virtual void OnNetSend(XClient* pClient);
	virtual void OnNetMsgRecv(XClient* pClient, MsgHeader* pMsgHeader);
	virtual void OnNetMsgDone(XClient* pClient, MsgHeader* pMsgHeader);

	virtual void AddTask(std::function<void()> pTask);
};

#endif