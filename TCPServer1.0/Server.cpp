#define WIN32_LEAN_AND_MEAN

#include <stdio.h>
#include <windows.h>
#include <WinSock2.h>

enum MGS_TYPE
{
	MSG_ERROR,
	MSG_LOGIN,
	MSG_LOGIN_RES,
	MSG_LOGOUT,
	MSG_LOGOUT_RES
};

struct MsgHeader
{
	MGS_TYPE msgType;
	int msgLength;
};

struct Login : public MsgHeader
{
	Login()
	{
		msgType = MSG_LOGIN;
		msgLength = sizeof(Login);
	}
	char name[32];
	char pwd[32];
};

struct LoginRes : public MsgHeader
{
	LoginRes()
	{
		msgType = MSG_LOGIN_RES;
		msgLength = sizeof(LoginRes);
		res = 1;
	}
	int res;
};

struct Logout : public MsgHeader
{
	Logout()
	{
		msgType = MSG_LOGOUT;
		msgLength = sizeof(Logout);
	}
	char name[32];
};

struct LogoutRes : public MsgHeader
{
	LogoutRes()
	{
		msgType = MSG_LOGOUT_RES;
		msgLength = sizeof(LogoutRes);
		res = 1;
	}
	int res;
};

int main()
{
	//打开网络
	WORD wsaVersion = MAKEWORD(2, 2);
	WSADATA wsaData = {};
	int iErrorNo = WSAStartup(wsaVersion, &wsaData);
	if (iErrorNo)
	{
		printf("Error:WSA start up failed!\n");
	}

	//创建Socket
	SOCKET _socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SOCKET_ERROR == _socket)
	{
		printf("Error:创建Socket!\n");
	}
	else
	{
		printf("OK:创建Socket!\n");
	}

	//绑定IP和端口
	sockaddr_in sin = {};
	sin.sin_family = AF_INET;
	sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	sin.sin_port = htons(9090);
	if (SOCKET_ERROR == bind(_socket, (sockaddr*)&sin, sizeof(sockaddr_in)))
	{
		printf("Error:绑定IP和端口!\n");
	}
	else
	{
		printf("OK:绑定IP和端口!\n");
	}

	//创建监听
	if (SOCKET_ERROR == listen(_socket, 10))
	{
		printf("Error:创建监听!\n");
	}
	else
	{
		printf("OK:创建监听!\n");
	}

	//等待客户端连接
	sockaddr_in sinClient = {};
	int sinLen = sizeof(sockaddr_in);
	SOCKET _client = INVALID_SOCKET;
	_client = accept(_socket, (sockaddr*)&sinClient, &sinLen);
	if (_client == INVALID_SOCKET)
	{
		printf("Error:客户端连接!\n");
	}
	else
	{
		printf("OK:客户端连接!\n");
	}

	while (true)
	{

		//接收客户端请求
		char buffer[1024] = {};
		int size = recv(_client, buffer, sizeof(MsgHeader), 0);
		MsgHeader* request = (MsgHeader*)buffer;
		if (SOCKET_ERROR == size)
		{
			printf("Error:接收客户端请求!\n");
			break;
		}
		else if (size == 0)
		{
			printf("OK:客户端关闭连接!\n");
			break;
		}

		//显示客户端请求
		printf("--接收客户端请求：Type:%d - Length:%d\n", request->msgType, request->msgLength);

		//处理客户端请求
		switch (request->msgType)
		{
		case MSG_LOGIN:
		{
			recv(_client, buffer + sizeof(MsgHeader), request->msgLength - sizeof(MsgHeader), 0);
			Login* login = (Login*)buffer;

			printf("--登入消息内容：Name:%s - Pwd:%s\n", login->name, login->pwd);

			LoginRes respond;
			send(_client, (char*)&respond, sizeof(LoginRes), 0);
		}
		break;
		case MSG_LOGOUT:
		{
			recv(_client, buffer + sizeof(MsgHeader), request->msgLength - sizeof(MsgHeader), 0);
			Logout* logout = (Logout*)buffer;

			printf("--登出消息内容：Name:%s\n", logout->name);

			LogoutRes respond;
			send(_client, (char*)&respond, sizeof(LogoutRes), 0);
		}
		break;
		default:
		{

		}
		}

	}

	//关闭客户端连接
	if (SOCKET_ERROR == closesocket(_client))
	{
		printf("Error:关闭客户端连接!\n");
	}
	else
	{
		printf("OK:关闭客户端连接!\n");
	}

	//关闭网络
	int iError = WSACleanup();
	if (SOCKET_ERROR == iError)
	{
		printf("Error:WSA clean up failed!\n");
	}

	//退出
	getchar();
	return 0;
}