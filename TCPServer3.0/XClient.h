#ifndef __XCLIENT_H__
#define __XCLIENT_H__

#include "XCommon.h"
#include "XIEvent.h"
#include "XReceiveServer.h"

//�ͻ�����Ϣ��
class XClient : public XObject<1024, XClient>
{
private:
	//��������
	XIEvent* _pNetEventObj;						//���̶߳���
	XReceiveServer* _pReceiveServerObj;			//ReceiveServer����

												//˽����Ϣ
	SOCKET _Socket;								//�ͻ���Socket
	char _RecvBuffer[_RECV_BUFFER_SIZE_];		//���ջ�����
	int _RecvStartPos;							//���ջ������п��Է������ݵ���ʼλ��
	char _SendBuffer[_SEND_BUFFER_SIZE_];		//���ͻ�����
	int _SendStartPos;							//���ͻ������п��Է������ݵ���ʼλ��

public:
	XClient(SOCKET client);
	~XClient();

	void Init(XIEvent* pEventObj, XReceiveServer* pReceiveServerObj);

	SOCKET GetSocket() { return _Socket; }

	int RecvData();
	int SendData(MsgHeader* pHeader);
};


#endif