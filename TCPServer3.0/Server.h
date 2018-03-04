#ifndef __SERVER_H__
#define __SERVER_H__

//标准头文件
#include <iostream>
#include <vector>

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

//计时器
#include "XTimer.h"

//协议头文件
#include "MsgProtocol.h"

#define _SERVER_SIZE_ 4
#define _BUFFER_SIZE_ 10240

//类前置声明
class _Client;

//接口
class IEvent
{
public:
	virtual void OnRunBegin() = 0;
	virtual void OnClientJoin(_Client* pClient) = 0;
	virtual void OnClientLeave(_Client* pClient) = 0;
	virtual void OnNetMsg(_Client* pClient) = 0;
};

//客户端信息类
class _Client
{
private:
	SOCKET _Socket;							//客户端Socket
	char _DataBuffer[_BUFFER_SIZE_ * 10];	//数据缓冲区
	int _StartPos;							//数据缓冲区中可以放入数据的起始位置

public:
	_Client(SOCKET client)
	{
		_Socket = client;
		memset(_DataBuffer, 0, sizeof(_DataBuffer));
		_StartPos = 0;
	}

	SOCKET GetSocket() { return _Socket; }
	char* GetDataBuffer() { return _DataBuffer; }
	int GetStartPos() { return _StartPos; }
	void SetStartPos(int startPos) { _StartPos = startPos; }

public:
	int SendData(MsgHeader* pHeader);
};

class _ReceiveServer
{
private:
	IEvent* _pNetEventObj;					//主线程对象
	std::vector<_Client*> _AllClients;			//客户端
	std::vector<_Client*> _AllClientsCache;		//客户端缓冲区
	std::mutex _AllClientsCacheMutex;			//客户端缓冲区锁
	char _RecvBuffer[_BUFFER_SIZE_];			//接收缓冲区

public:
	_ReceiveServer();
	~_ReceiveServer();

	void SetNetEventObj(IEvent* pEventObj);

	int Start();
	int OnRun();

	int RecvData(_Client* pClient);
	int OnNetMsg(_Client* pClient, MsgHeader* pHeader);

	void AddClient(_Client* pClient);
	int GetClientNum();
};

//监听Server类
class _ListenServer : public IEvent
{
private:
	SOCKET _Socket;								//服务器监听Socket
	std::vector<_ReceiveServer*> _AllServers;	//服务器信息

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

	virtual void OnRunBegin();
	virtual void OnClientJoin(_Client* pClient);
	virtual void OnClientLeave(_Client* pClient);
	virtual void OnNetMsg(_Client* pClient);
};

typedef _ListenServer MainServer;

#endif