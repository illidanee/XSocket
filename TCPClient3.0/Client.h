#ifndef __CLIENT_H__
#define __CLIENT_H__

//标准头文件
#include <iostream>

#ifdef _WIN32
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

//协议头文件
#include "MsgProtocol.h"

#define _BUFFER_SIZE_ 10240

//Client类
class Client
{
private:
	SOCKET _Socket;							//客户端Socket

	char _RecvBuffer[_BUFFER_SIZE_];		//接收缓冲区
	char _DataBuffer[_BUFFER_SIZE_ * 10];	//数据缓冲区
	int _StartPos;							//数据缓冲区中可以放入数据的起始位置

public:
	Client();
	~Client();

	int Init();
	int Done();

	int Open();
	int Connect(const char* ip, unsigned short port);
	int Close();

	int IsRun();
	int OnRun();

	int SendData(MsgHeader* pHeader);
	int RecvData();
	virtual int OnNetMsg(MsgHeader* pHeader);
};

#endif