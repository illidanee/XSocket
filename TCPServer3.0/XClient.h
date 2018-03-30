#ifndef __XCLIENT_H__
#define __XCLIENT_H__

#include "XCommon.h"
#include "XIEvent.h"
#include "XReceiveServer.h"
#include "XBuffer.h"

//�ͻ�����Ϣ��
class XClient : public XObject<1024, XClient>
{
public:
	XClient(SOCKET client, XIEvent* pEventObj, XReceiveServer* pReceiveServerObj);
	~XClient();

	SOCKET GetSocket() { return _Socket; }

	int RecvData();
	int SendData(MsgHeader* pHeader);
	int SendData();

	//����
	void ResetHeartTime();
	bool CheckHeartTime(time_t t);

	//��ʱ��������
	void ResetSendTime();
	void CheckSendTime(time_t t);

private:
	SOCKET _Socket;								//�ͻ���Socket

	XIEvent* _pNetEventObj;						//���̶߳���
	XReceiveServer* _pReceiveServerObj;			//ReceiveServer����

	XBuffer _RecvBuffer;						//���ջ���������
	XBuffer _SendBuffer;						//���ͻ���������

private:
	time_t	_HeartTime;							//������ʱ��
	time_t	_SendTime;							//��ʱ�������ݼ�ʱ��
};


#endif