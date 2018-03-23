#ifndef __XSERVER_H__
#define __XSERVER_H__

//标准头文件
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

//内存池
#include "../Memory/XMemory.h"
#include "../Object/XObjectManager.h"
//计时器
#include "XTimer.h"
//任务
#include "XTask.h"

//协议头文件
#include "MsgProtocol.h"

//客户端单次发送消息大于_SEND_BUFFER_SIZE_会出问题
#define _SERVER_SIZE_ 4
#define _RECV_BUFFER_SIZE_ 10240
#define _SEND_BUFFER_SIZE_ 10240

//类前置声明
class _Client;
class _ReceiveServer;

//接口
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

//客户端信息类
class _Client : public XObjectManager<10000, _Client>
{
private:
	//关联对象
	IEvent* _pNetEventObj;						//主线程对象
	_ReceiveServer* _pReceiveServerObj;			//ReceiveServer对象

	//私有信息
	SOCKET _Socket;								//客户端Socket
	char _RecvBuffer[_RECV_BUFFER_SIZE_];		//接收缓冲区
	int _RecvStartPos;							//接收缓冲区中可以放入数据的起始位置
	char _SendBuffer[_SEND_BUFFER_SIZE_];		//发送缓冲区
	int _SendStartPos;							//发送缓冲区中可以放入数据的起始位置

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
	IEvent* _pNetEventObj;											//主线程对象

	std::map<SOCKET, std::shared_ptr<_Client>> _AllClients;			//客户端
	std::map<SOCKET, std::shared_ptr<_Client>> _AllClientsCache;	//客户端缓冲区
	std::mutex _AllClientsCacheMutex;								//客户端缓冲区锁

private:
	XTaskServer _TaskServer;										//服务器对应的任务线程。

//优化
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

//监听Server类
class _ListenServer : public IEvent
{
private:
	SOCKET _Socket;												//服务器监听Socket
	std::vector<std::shared_ptr<_ReceiveServer>> _AllServers;	//服务器信息

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