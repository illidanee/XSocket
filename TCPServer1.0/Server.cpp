#include <stdio.h>
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


enum MGS_TYPE
{
	MSG_ERROR,
	MSG_LOGIN,
	MSG_LOGIN_RES,
	MSG_LOGOUT,
	MSG_LOGOUT_RES,
	MSG_NEWUSER
};

struct MsgHeader
{
	MGS_TYPE msgType;
	int msgLength;
};

struct MsgLogin : public MsgHeader
{
	MsgLogin()
	{
		msgType = MSG_LOGIN;
		msgLength = sizeof(MsgLogin);
	}
	char name[32];
	char pwd[32];
};

struct MsgLoginRes : public MsgHeader
{
	MsgLoginRes()
	{
		msgType = MSG_LOGIN_RES;
		msgLength = sizeof(MsgLoginRes);
		res = 0;
	}
	int res;
};

struct MsgLogout : public MsgHeader
{
	MsgLogout()
	{
		msgType = MSG_LOGOUT;
		msgLength = sizeof(MsgLogout);
	}
	char name[32];
};

struct MsgLogoutRes : public MsgHeader
{
	MsgLogoutRes()
	{
		msgType = MSG_LOGOUT_RES;
		msgLength = sizeof(MsgLogoutRes);
		res = 0;
	}
	int res;
};

struct MsgNewUser : public MsgHeader
{
	MsgNewUser()
	{
		msgType = MSG_NEWUSER;
		msgLength = sizeof(MsgNewUser);
		user = 0;
	}
	int user;
};

std::vector<SOCKET> g_AllClients;

int HandleMsg(SOCKET client)
{
	char buffer[1024] = {};
	int size = recv(client, buffer, sizeof(MsgHeader), 0);
	MsgHeader* request = (MsgHeader*)buffer;
	if (SOCKET_ERROR == size)
	{
		printf("Error:Client<Socket=%d> Recv Header!\n", client);
		return -1;
	}
	else if (size == 0)
	{
		printf("OK:Client<Socket=%d> Recv Header!\n", client);
		return -2;
	}

	printf("--Client<Socket=%d> Msg Type:%d - Length:%d\n", client, request->msgType, request->msgLength);

	switch (request->msgType)
	{
	case MSG_LOGIN:
	{
		recv(client, buffer + sizeof(MsgHeader), request->msgLength - sizeof(MsgHeader), 0);
		MsgLogin* login = (MsgLogin*)buffer;

		printf("--MSG_LOGIN : Name:%s - Pwd:%s\n", login->name, login->pwd);

		MsgLoginRes respond;
		send(client, (char*)&respond, sizeof(MsgLoginRes), 0);
	}
	break;
	case MSG_LOGOUT:
	{
		recv(client, buffer + sizeof(MsgHeader), request->msgLength - sizeof(MsgHeader), 0);
		MsgLogout* logout = (MsgLogout*)buffer;

		printf("--MSG_LOGOUT : Name:%s\n", logout->name);

		MsgLogoutRes respond;
		send(client, (char*)&respond, sizeof(MsgLogoutRes), 0);
	}
	break;
	default:
	{

	}
	}

	return 0;
}

int main()
{

#ifdef _WIN32
	WORD wsaVersion = MAKEWORD(2, 2);
	WSADATA wsaData = {};
	int iErrorNo = WSAStartup(wsaVersion, &wsaData);
	if (iErrorNo)
	{
		printf("Error:WSA start up failed!\n");
	}
#endif

	//Create Socket
	SOCKET _socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SOCKET_ERROR == _socket)
	{
		printf("Error:Create Socket!\n");
	}
	else
	{
		printf("OK:Create Socket!\n");
	}

	sockaddr_in sin = {};
	sin.sin_family = AF_INET;
#ifdef _WIN32
	sin.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
#else
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
#endif
	sin.sin_port = htons(9090);
	if (SOCKET_ERROR == bind(_socket, (sockaddr*)&sin, sizeof(sockaddr_in)))
	{
		printf("Error:Bind Socket!\n");
	}
	else
	{
		printf("OK:Bind Socket!\n");
	}

	if (SOCKET_ERROR == listen(_socket, 10))
	{
		printf("Error:Listen!\n");
	}
	else
	{
		printf("OK:Listen!\n");
	}

	while (true)
	{
		fd_set fdRead;
		FD_ZERO(&fdRead);
		FD_SET(_socket, &fdRead);

		SOCKET MaxSocket = _socket;
		for (size_t i = 0; i < g_AllClients.size(); ++i)
		{
			FD_SET(g_AllClients[i], &fdRead);
			if (MaxSocket < g_AllClients[i])
				MaxSocket = g_AllClients[i];
		}

		timeval tv = { 0, 0 };

		int ret = select(MaxSocket + 1, &fdRead, NULL, NULL, &tv);
		if (SOCKET_ERROR == ret)
		{
			printf("Error:Select!\n");
			break;
		}

		if (FD_ISSET(_socket, &fdRead))
		{
			FD_CLR(_socket, &fdRead);

			sockaddr_in sinClient = {};
			int sinLen = sizeof(sockaddr_in);
			SOCKET _client = INVALID_SOCKET;
#ifdef _WIN32
			_client = accept(_socket, (sockaddr*)&sinClient, &sinLen);
#else
			_client = accept(_socket, (sockaddr*)&sinClient, (socklen_t*)&sinLen);
#endif
			if (_client == INVALID_SOCKET)
			{
				printf("Error:Client<Socket=%d> Accept!\n", _client);
			}
			else
			{
				printf("OK:Client<Socket=%d> Accept!\n", _client);

				MsgNewUser msgNewUser;
				msgNewUser.user = _client;

				for (std::vector<SOCKET>::iterator it = g_AllClients.begin(); it != g_AllClients.end(); ++it)
				{
					send(*it, (char*)&msgNewUser, sizeof(MsgNewUser), 0);
				}

				g_AllClients.push_back(_client);
			}
		}

		for (std::vector<SOCKET>::iterator iter = g_AllClients.begin(); iter != g_AllClients.end(); )
		{
			if (FD_ISSET(*iter, &fdRead))
			{
				int ret = HandleMsg(*iter);
				if (ret < 0)
				{

#ifdef _WIN32
					if (SOCKET_ERROR == closesocket(*iter))
					{
						printf("Error:Client<Socket=%d> Close!\n", *iter);
					}
					else
					{
						printf("OK:Client<Socket=%d> Close!\n", *iter);
					}
#else
					if (SOCKET_ERROR == close(*iter))
					{
						printf("Error:Client<Socket=%d> Close!\n", *iter);
					}
					else
					{
						printf("OK:Client<Socket=%d> Close!\n", *iter);
					}
#endif

					iter = g_AllClients.erase(iter);
					continue;
				}
			}

			++iter;
		}
	}

#ifdef _WIN32
	for (std::vector<SOCKET>::iterator it = g_AllClients.begin(); it != g_AllClients.end(); ++it)
	{
		if (SOCKET_ERROR == closesocket(*it))
		{
			printf("Error:Close All!\n");
		}
		else
		{
			printf("OK:Close All!\n");
		}
	}
	g_AllClients.clear();

	closesocket(_socket);

	int iError = WSACleanup();
	if (SOCKET_ERROR == iError)
	{
		printf("Error:WSA clean up failed!\n");
	}
#else
	for (std::vector<SOCKET>::iterator it = g_AllClients.begin(); it != g_AllClients.end(); ++it)
	{
		if (SOCKET_ERROR == close(*it))
		{
			printf("Error:Close All!\n");
		}
		else
		{
			printf("OK:Close All!\n");
		}
	}
	g_AllClients.clear();

	close(_socket);
#endif

	return 0;
}
