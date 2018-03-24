#ifndef __MYSERVER_H__
#define __MYSERVER_H__

#include "XCommon.h"
#include "XServer.h"
#include "XSendTask.h"
#include <atomic>

//�Զ���Server
class MyServer : public XServer
{
private:
	XTimer _Timer;								//��ʱ��
	std::atomic_int _ClientNum;					//�ͻ��˼�����
	std::atomic_int _RecvNum;					//recv()�������ü���
	std::atomic_int _SendNum;					//send()�������ü���
	std::atomic_int _RecvPackageNum;			//�������ݰ�������
	std::atomic_int _DonePackageNum;			//�������ݰ�������
	std::atomic_int _PackageNum;				//ʣ�����������

public:
	MyServer();
	~MyServer();
	virtual void OnRunBegin();
	virtual void OnClientJoin(XClient* pClient);
	virtual void OnClientLeave(XClient* pClient);
	virtual void OnNetRecv(XClient* pClient);
	virtual void OnNetSend(XClient* pClient);
	virtual void OnNetMsgRecv(XClient* pClient, MsgHeader* pMsgHeader, XReceiveServer* pReceiveServer);
	virtual void OnNetMsgDone(XClient* pClient, MsgHeader* pMsgHeader, XReceiveServer* pReceiveServer);
};

#endif
