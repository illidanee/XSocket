#include <iostream>
#ifndef __SERVER_H__
#define __SERVER_H__

//��׼ͷ�ļ�
#include <iostream>
#include <vector>

#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>

#ifdef _WIN32
#define FD_SETSIZE 1024
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

#include "XTimer.h"

//Э��ͷ�ļ�
#include "MsgProtocol.h"

#define _SERVER_SIZE_ 4
#define _BUFFER_SIZE_ 10240

//�ͻ�����Ϣ��
class ClientInfo
{
private:
	SOCKET _Socket;							//�ͻ���Socket
	char _DataBuffer[_BUFFER_SIZE_ * 10];	//���ݻ�����
	int _StartPos;							//���ݻ������п��Է������ݵ���ʼλ��

public:
	ClientInfo(SOCKET client)
	{
		_Socket = client;
		memset(_DataBuffer, 0, sizeof(_DataBuffer));
		_StartPos = 0;
	}

	SOCKET GetSocket() { return _Socket; }
	char* GetDataBuffer() { return _DataBuffer; }
	int GetStartPos() { return _StartPos; }
	void SetStartPos(int startPos) { _StartPos = startPos; }
};


class MicroServer
{
private:
	std::vector<ClientInfo*> _AllClients;		//�ͻ�����Ϣ
	std::vector<ClientInfo*> _AllClientsCache;	//�ͻ�����Ϣ������
	std::mutex _Mutex;							//��
	char _RecvBuffer[_BUFFER_SIZE_];			//���ջ�����

public:
	std::atomic_int _PackageNum;				//�������ݰ�������

public:
	MicroServer();
	~MicroServer();

	int Start();
	int OnRun();

	int SendData(ClientInfo* pClientInfo, MsgHeader* pHeader);
	int SendDataToAll(MsgHeader* pHeader);
	int RecvData(ClientInfo* pClientInfo);
	virtual int OnNetMsg(ClientInfo* pClientInfo, MsgHeader* pHeader);

	void AddClient(ClientInfo* pClient);
	int GetClientNum();
};

//Server��
class Server
{
private:
	SOCKET _Socket;							//������Socket
	std::vector<ClientInfo*> _AllClients;	//�ͻ�����Ϣ
	std::vector<MicroServer*> _AllServers;  //��������Ϣ

	XTimer _Timer;
public:
	Server();							
	~Server();

	int Init();
	int Done();

	int Open();
	int Bind(const char* ip, unsigned short port);
	int Listen(int n);
	int Accept();
	int Start();
	int Close();

	int CloseOne(ClientInfo* pClientInfo);
	int CloseAll();

	int IsRun();
	int OnRun();
};


#endif