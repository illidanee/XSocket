#ifndef __CLIENT_H__
#define __CLIENT_H__

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

#include "MsgProtocol.h"

class Client
{
private:
	SOCKET _Socket;
	char _Buffer[409600000];

public:
	Client();
	~Client();

	int Init();
	int Done();

	int Connect(const char* ip, unsigned short port);
	int Close();

	int IsRun();
	int OnRun();

	int SendData(MsgHeader* pHeader);
	int RecvData();
};

#endif