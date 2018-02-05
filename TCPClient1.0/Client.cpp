#include <iostream>
#include <thread>


#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
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

int HandleMsg(SOCKET client)
{
	//Cache Buffer
	char buffer[1024] = {};
	int size = recv(client, buffer, sizeof(MsgHeader), 0);
	MsgHeader* request = (MsgHeader*)buffer;
	if (SOCKET_ERROR == size)
	{
		printf("Error:Client off!\n");
		return -1;
	}
	else if (size == 0)
	{
		printf("OK:Client quit!\n");
		return -2;
	}

	printf("--Recieve Msg Type:%d - Length:%d\n", request->msgType, request->msgLength);

	switch (request->msgType)
	{
	case MSG_LOGIN_RES:
	{
		recv(client, buffer + sizeof(MsgHeader), request->msgLength - sizeof(MsgHeader), 0);
		MsgLoginRes* login = (MsgLoginRes*)buffer;

		printf("--Login Ret:%d\n", login->res);
	}
	break;
	case MSG_LOGOUT_RES:
	{
		recv(client, buffer + sizeof(MsgHeader), request->msgLength - sizeof(MsgHeader), 0);
		MsgLogoutRes* logout = (MsgLogoutRes*)buffer;

		printf("--Logout Ret:%d\n", logout->res);
	}
	break;
	case MSG_NEWUSER:
	{
		recv(client, buffer + sizeof(MsgHeader), request->msgLength - sizeof(MsgHeader), 0);
		MsgNewUser* newUser = (MsgNewUser*)buffer;

		printf("--New User<Socket=%d>Join Server!\n", newUser->user);
	}
	break;
	default:
	{

	}
	}

	return 0;
}

bool gRun = true;

void CmdThread(SOCKET socket)
{
	while (true)
	{
		char buffer[32] = {};
		scanf("%s", buffer);
		if (0 == strcmp(buffer, "Login"))
		{
			MsgLogin login = {};
			memcpy(login.name, "illidan", sizeof("illidan"));
			memcpy(login.pwd, "12345", sizeof("12345"));
			send(socket, (char*)&login, sizeof(MsgLogin), 0);
		}
		else if (0 == strcmp(buffer, "Logout"))
		{
			gRun = false;
			MsgLogout logout = {};
			memcpy(logout.name, "illidan", sizeof("illidan"));
			send(socket, (char*)&logout, sizeof(MsgLogout), 0);
		}
	}
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
#endif // _WIN32

	SOCKET _socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SOCKET_ERROR == _socket)
	{
		printf("Error:Can't Create Server!\n");
	}
	else
	{
		printf("OK:Create Server!\n");
	}

	sockaddr_in sinServer = {};
	sinServer.sin_family = AF_INET;
#ifdef _WIN32
	sinServer.sin_addr.S_un.S_addr = inet_addr("192.168.0.90");
#else
	sinServer.sin_addr.s_addr = inet_addr("192.168.0.90");
#endif // _WIN32
	sinServer.sin_port = htons(9090);
	int sinLen = sizeof(sockaddr_in);
	if (SOCKET_ERROR == connect(_socket, (sockaddr*)&sinServer, sinLen))
	{
		printf("Error:Can't Connect Server!\n");
	}
	else
	{
		printf("OK:Connect Server!\n");
	}

	std::thread t1(CmdThread, _socket);
	t1.detach();

	while (gRun)
	{
		fd_set fdRead;
		FD_ZERO(&fdRead);
		FD_SET(_socket, &fdRead);

		timeval tv = { 0, 0 };

		int ret = select(0, &fdRead, NULL, NULL, &tv);
		if (SOCKET_ERROR == ret)
		{
			printf("Error:Select Error!\n");
			break;
		}

		if (FD_ISSET(_socket, &fdRead))
		{
			FD_CLR(_socket, &fdRead);

			int ret = HandleMsg(_socket);
			if (ret < 0)
			{
				break;
			}
		}
	}


#ifdef _WIN32
	if (SOCKET_ERROR == closesocket(_socket))
	{
		printf("Error:!\n");
	}
	else
	{
		printf("OK:!\n"); 
	}

	int iError = WSACleanup();
	if (SOCKET_ERROR == iError)
	{
		printf("Error:WSA clean up failed!\n");
	}
#else
	if (SOCKET_ERROR == close(_socket))
	{
		printf("Error:!\n");
	}
	else
	{
		printf("OK:!\n");
	}
#endif // _WIN32

	return 0;
}