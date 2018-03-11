#include <thread>
#include "XServer.h"

#ifndef _WIN32
#include <signal.h>
#endif

//�����߳�
void CmdThread(_ListenServer* pServer)
{
	while (true)
	{
		char buffer[32] = {};
		scanf("%s", buffer);
		if (0 == strcmp(buffer, "Exit"))
		{
			pServer->Close();
			break;
		}
	}
}

//�Զ���Server
class MyServer : public XServer
{
private:
	XTimer _Timer;								//��ʱ��
	std::atomic_int _ClientNum;					//�ͻ��˼�����
	std::atomic_int _RecvNum;					//recv()�������ü���
	std::atomic_int _SendNum;					//send()�������ü���
	std::atomic_int _PackageNum;				//�������ݰ�������

public:
	MyServer()
	{
		//��ʼ������
		_Timer.XInit();
		_ClientNum = 0;
		_RecvNum = 0;
		_SendNum = 0;
		_PackageNum = 0;
	}
	~MyServer()
	{
		_Timer.XDone();
	}
	virtual void OnRunBegin()
	{
		if (_Timer.GetTime() > 1.0)
		{
			printf("| Client Num = %7d  | Recv Num = %7d  | Send Num = %7d  | Package Num = %7d  |\n", (int)_ClientNum, (int)_RecvNum, (int)_SendNum, (int)_PackageNum);
			_RecvNum = 0;
			_SendNum = 0;
			_PackageNum = 0;
			_Timer.UpdateTime();
		}
	}
	virtual void OnClientJoin(_Client* pClient)
	{
		++_ClientNum;
	}
	virtual void OnClientLeave(_Client* pClient)
	{
		--_ClientNum;
	}
	virtual void OnNetRecv(_Client* pClient)
	{
		++_RecvNum;
	}
	virtual void OnNetSend(_Client* pClient)
	{
		++_SendNum;
	}
	virtual void OnNetMsg(_Client* pClient, MsgHeader* pHeader, _ReceiveServer* pReceiveServer)
	{
		++_PackageNum;

		//����ͻ�������
		switch (pHeader->_MsgType)
		{
		case MSG_LOGIN:
		{
			MsgLoginRes* respond = new MsgLoginRes;
			XSendTask* pTask = new XSendTask(pClient, respond);
			pReceiveServer->AddTask(pTask);
		}
		break;
		default:
		{
			printf("Warn�� default Msg��");
		}
		}
	}
};

int main()
{
//ȥ���ж��źŹرշ�������
#ifndef _WIN32
	sigset_t signal_mask;
	sigemptyset(&signal_mask);
	sigaddset(&signal_mask, SIGPIPE);
	if (pthread_sigmask(SIG_BLOCK, &signal_mask, NULL) == -1)
		perror("SIGPIPE");
#endif

	//����������
	MyServer* server = new MyServer;

	//����������
	server->Init();
	server->Open();
	server->Bind(NULL, 9090);
	server->Listen(1000);
	server->Start();

	std::thread cmdThread(CmdThread, server);
	cmdThread.detach();

	//���з�����
	server->OnRun();

	//�رշ�����
	server->Close();

	return 0;
}