#ifndef __MYCLIENT_H__
#define __MYCLIENT_H__

#include "../XSrc/XTCPClient.h"
#include "../XSrc/XRecvByteStream.h"

//客户端配置变量
extern const int g_tCount;
extern const int g_cCount;
extern const time_t g_time;
extern const time_t g_msgCount;

class MyClient : public XTCPClient
{
public:
	MyClient()
	{
		_curTime = 0;
		_nCount = g_msgCount;
		_MsgID = 0;
	}
	void DoMsg(MsgHeader* pMsgHeader);

	bool CanSend();
	bool Send(MsgHeader* pMsgHeader);
	void ResetCount();

private:
	virtual void OnRunBegin();
	virtual void OnClientJoin(std::shared_ptr<XClient> pClient);
	virtual void OnClientLeave(std::shared_ptr<XClient> pClient);
	virtual void OnNetRecv(std::shared_ptr<XClient> pClient);
	virtual void OnNetSend(std::shared_ptr<XClient> pClient);
	virtual void OnNetMsgBegin(std::shared_ptr<XClient> pClient, MsgHeader* pMsgHeader);
	virtual void OnNetMsgEnd(std::shared_ptr<XClient> pClient, MsgHeader* pMsgHeader);

	virtual void AddTask(std::function<void()> pTask);

	time_t _curTime;
	time_t _nCount;
	int _MsgID;
};

#endif