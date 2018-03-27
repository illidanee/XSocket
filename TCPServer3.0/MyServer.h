#ifndef __MYSERVER_H__
#define __MYSERVER_H__

#include "XCommon.h"
#include "XServer.h"
#include <atomic>
#include <functional>

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
	virtual void OnRunLoopBegin();
	virtual void OnClientJoin(std::shared_ptr<XClient> pClient);
	virtual void OnClientLeave(std::shared_ptr<XClient> pClient);
	virtual void OnNetRecv(XClient* pClient);
	virtual void OnNetSend(XClient* pClient);
	virtual void OnNetMsgRecv(XClient* pClient, MsgHeader* pMsgHeader, XReceiveServer* pReceiveServer);
	virtual void OnNetMsgDone(XClient* pClient, MsgHeader* pMsgHeader, XReceiveServer* pReceiveServer);
};

#endif
