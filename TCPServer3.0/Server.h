#ifndef __SERVER_H__
#define __SERVER_H__

//��׼ͷ�ļ�
#include <iostream>
#include <vector>
#include <map>

#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>

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

//��ʱ��
#include "XTimer.h"

//Э��ͷ�ļ�
#include "MsgProtocol.h"

#define _SERVER_SIZE_ 4
#define _RECV_BUFFER_SIZE_ 10240
#define _SEND_BUFFER_SIZE_ 10240

//��ǰ������
class _Client;

//�ӿ�
class IEvent
{
public:
	virtual void OnRunBegin() = 0;
	virtual void OnClientJoin(_Client* pClient) = 0;
	virtual void OnClientLeave(_Client* pClient) = 0;
	virtual void OnNetRecv(_Client* pClient) = 0;
	virtual void OnNetSend(_Client* pClient) = 0;
	virtual void OnNetMsg(_Client* pClient, MsgHeader* pHeader) = 0;
};

//�ͻ�����Ϣ��
class _Client
{
private:
	IEvent* _pNetEventObj;						//���̶߳���
	SOCKET _Socket;								//�ͻ���Socket
	char _RecvBuffer[_RECV_BUFFER_SIZE_];		//���ջ�����
	int _RecvStartPos;							//���ջ������п��Է������ݵ���ʼλ��
	char _SendBuffer[_SEND_BUFFER_SIZE_];		//���ͻ�����
	int _SendStartPos;							//���ͻ������п��Է������ݵ���ʼλ��
public:
	_Client(SOCKET client);
	~_Client();

	void SetNetEventObj(IEvent* pEventObj);

	SOCKET GetSocket() { return _Socket; }
	char* GetRecvBuffer() { return _RecvBuffer; }
	int GetRecvStartPos() { return _RecvStartPos; }
	void SetRecvStartPos(int startPos) { _RecvStartPos = startPos; }

public:
	int RecvData();
	int SendData(MsgHeader* pHeader);
};

class _ReceiveServer
{
private:
	IEvent* _pNetEventObj;							//���̶߳���
	std::map<SOCKET, _Client*> _AllClients;			//�ͻ���
	std::map<SOCKET, _Client*> _AllClientsCache;	//�ͻ��˻�����
	std::mutex _AllClientsCacheMutex;				//�ͻ��˻�������

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

	void AddClient(_Client* pClient);
	int GetClientNum();
};

class _SendServer
{

};

//����Server��
class _ListenServer : public IEvent
{
private:
	SOCKET _Socket;								//����������Socket
	std::vector<_ReceiveServer*> _AllServers;	//��������Ϣ

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

typedef _ListenServer MainServer;

#endif