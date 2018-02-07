#ifndef __SERVER_H__
#define __SERVER_H__

#include <iostream>
#include <vector>

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

class Server
{
private:
	SOCKET _Socket;
	std::vector<SOCKET> _AllClients;

	char _Buffer[1024];

public:
	Server();
	~Server();

	int Init();
	int Done();

	int Open();
	int Bind(const char* ip, unsigned short port);
	int Listen(int n);
	int Accept();
	int Close();

	int CloseOne(SOCKET client);
	int CloseAll();

	int IsRun();
	int OnRun();

	int RecvData(SOCKET client);
	int SendData(SOCKET client, MsgHeader* pHeader);
	int SendDataToAll(MsgHeader* pHeader);
};

#endif