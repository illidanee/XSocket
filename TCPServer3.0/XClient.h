#ifndef __XCLIENT_H__
#define __XCLIENT_H__

#include "XCommon.h"
#include "XIEvent.h"
#include "XReceiveServer.h"

//�ͻ�����Ϣ��
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

	//����
	void ResetHeartTime();
	bool CheckHeartTime(time_t t);

	//��ʱ��������
	void ResetSendTime();
	void CheckSendTime(time_t t);

private:
	//��������
	XIEvent* _pNetEventObj;						//���̶߳���
	XReceiveServer* _pReceiveServerObj;			//ReceiveServer����

	SOCKET _Socket;								//�ͻ���Socket
	char _RecvBuffer[_RECV_BUFFER_SIZE_];		//���ջ�����
	int _RecvStartPos;							//���ջ������п��Է������ݵ���ʼλ��
	char _SendBuffer[_SEND_BUFFER_SIZE_];		//���ͻ�����
	int _SendStartPos;							//���ͻ������п��Է������ݵ���ʼλ��

private:
	time_t	_HeartTime;							//������ʱ��
	time_t	_SendTime;							//��ʱ�������ݼ�ʱ��
};


#endif