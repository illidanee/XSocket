#include <iostream>
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

//协议头文件
#include "MsgProtocol.h"

#define _SERVER_SIZE_ 4
#define _BUFFER_SIZE_ 10240

//客户端信息类
class ClientInfo
{
private:
	SOCKET _Socket;							//客户端Socket
	char _DataBuffer[_BUFFER_SIZE_ * 10];	//数据缓冲区
	int _StartPos;							//数据缓冲区中可以放入数据的起始位置

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
	std::vector<ClientInfo*> _AllClients;		//客户端信息
	std::vector<ClientInfo*> _AllClientsCache;	//客户端信息缓冲区
	std::mutex _Mutex;							//锁
	char _RecvBuffer[_BUFFER_SIZE_];			//接收缓冲区

public:
	std::atomic_int _PackageNum;				//接收数据包计数器

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

//Server类
class Server
{
private:
	SOCKET _Socket;							//服务器Socket
	std::vector<ClientInfo*> _AllClients;	//客户端信息
	std::vector<MicroServer*> _AllServers;  //服务器信息

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