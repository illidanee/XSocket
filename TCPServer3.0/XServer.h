#ifndef __XSERVER_H__
#define __XSERVER_H__

//��׼ͷ�ļ�
#include <iostream>
#include <vector>
#include <map>

#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include <memory>

#ifdef _WIN32
#define FD_SETSIZE 2510
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <windows.h>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define SOCKET int
#define INVALID_SOCKET  (SOCKET)(~0)
#define SOCKET_ERROR    (-1)
#endif // _WIN32

//�ڴ��
#include "../Memory/XMemory.h"
#include "../Object/XObjectManager.h"
//��ʱ��
#include "XTimer.h"
//����
#include "XTask.h"

//Э��ͷ�ļ�
#include "MsgProtocol.h"

//�ͻ��˵��η�����Ϣ����_SEND_BUFFER_SIZE_�������
#define _SERVER_SIZE_ 4
#define _RECV_BUFFER_SIZE_ 10240
#define _SEND_BUFFER_SIZE_ 10240

//��ǰ������
class _Client;
class _ReceiveServer;

//�ӿ�
class IEvent
{
public:
	virtual void OnRunBegin() = 0;
	virtual void OnClientJoin(_Client* pClient) = 0;
	virtual void OnClientLeave(_Client* pClient) = 0;
	virtual void OnNetRecv(_Client* pClient) = 0;
	virtual void OnNetSend(_Client* pClient) = 0;
	virtual void OnNetMsgRecv(_Client* pClient, MsgHeader* pHeader, _ReceiveServer* pReceiveServer) = 0;
	virtual void OnNetMsgDone(_Client* pClient, MsgHeader* pHeader, _ReceiveServer* pReceiveServer) = 0;
};

//�ͻ�����Ϣ��
class _Client : public XObjectManager<10000, _Client>
{
private:
	//��������
	IEvent* _pNetEventObj;						//���̶߳���
	_ReceiveServer* _pReceiveServerObj;			//ReceiveServer����

	//˽����Ϣ
	SOCKET _Socket;								//�ͻ���Socket
	char _RecvBuffer[_RECV_BUFFER_SIZE_];		//���ջ�����
	int _RecvStartPos;							//���ջ������п��Է������ݵ���ʼλ��
	char _SendBuffer[_SEND_BUFFER_SIZE_];		//���ͻ�����
	int _SendStartPos;							//���ͻ������п��Է������ݵ���ʼλ��

public:
	_Client(SOCKET client);
	~_Client();

	void Init(IEvent* pEventObj, _ReceiveServer* pReceiveServerObj);

	SOCKET GetSocket() { return _Socket; }

	int RecvData();
	int SendData(MsgHeader* pHeader);
};

class XSendTask : public XTask
{
private:
	_Client* _pClient;
	MsgHeader* _pHeader;

public:
	XSendTask(_Client* pClient, MsgHeader* pHeader);
	~XSendTask();
	void DoTask();
};

class _ReceiveServer
{
private:
	IEvent* _pNetEventObj;											//���̶߳���

	std::map<SOCKET, std::shared_ptr<_Client>> _AllClients;			//�ͻ���
	std::map<SOCKET, std::shared_ptr<_Client>> _AllClientsCache;	//�ͻ��˻�����
	std::mutex _AllClientsCacheMutex;								//�ͻ��˻�������

private:
	XTaskServer _TaskServer;										//��������Ӧ�������̡߳�

//�Ż�
private:
	fd_set _fdSetCache;
	bool _ClientChange;
	SOCKET _MaxSocketID;

public:
	_ReceiveServer();
	~_ReceiveServer();

	void SetNetEventObj(IEvent* pEventObj);

	int Start();
	int OnRun();

	void AddClient(std::shared_ptr<_Client>& pClient);
	int GetClientNum();
	void AddTask(std::shared_ptr<XTask>& pTask);
};

class _SendServer
{

};

//����Server��
class _ListenServer : public IEvent
{
private:
	SOCKET _Socket;												//����������Socket
	std::vector<std::shared_ptr<_ReceiveServer>> _AllServers;	//��������Ϣ

public:
	_ListenServer();							
	~_ListenServer();

	int Init();
	int Done();

	int Open();
	int Bind(const char* ip, unsigned short port);
	int Listen(int n);
	int Accept();
	int Start();
	int Close();

	int IsRun();
	int OnRun();
};

typedef _ListenServer XServer;

#endif