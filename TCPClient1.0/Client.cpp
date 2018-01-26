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
	//接收客户端请求
	char buffer[1024] = {};
	int size = recv(client, buffer, sizeof(MsgHeader), 0);
	MsgHeader* request = (MsgHeader*)buffer;
	if (SOCKET_ERROR == size)
	{
		printf("Error:接收服务器端请求!\n"); //点X强行关闭。
		return -1;
	}
	else if (size == 0)
	{
		printf("OK:服务器端关闭连接!\n");	 //用q命令关闭。
		return -2;
	}

	//显示客户端请求
	printf("--接收服务器端请求：Type:%d - Length:%d\n", request->msgType, request->msgLength);

	//处理客户端请求
	switch (request->msgType)
	{
	case MSG_LOGIN_RES:
	{
		recv(client, buffer + sizeof(MsgHeader), request->msgLength - sizeof(MsgHeader), 0);
		MsgLoginRes* login = (MsgLoginRes*)buffer;

		printf("--登入消息结果：Ret:%d\n", login->res);
	}
	break;
	case MSG_LOGOUT_RES:
	{
		recv(client, buffer + sizeof(MsgHeader), request->msgLength - sizeof(MsgHeader), 0);
		MsgLogoutRes* logout = (MsgLogoutRes*)buffer;

		printf("--登出消息结果：Ret:%d\n", logout->res);
	}
	break;
	case MSG_NEWUSER:
	{
		recv(client, buffer + sizeof(MsgHeader), request->msgLength - sizeof(MsgHeader), 0);
		MsgNewUser* newUser = (MsgNewUser*)buffer;

		printf("--新用户<Socket=%d>加入服务器!\n", newUser->user);
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

	//连接服务器
	sockaddr_in sinServer = {};
	sinServer.sin_family = AF_INET;
	sinServer.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	sinServer.sin_port = htons(9090);
	int sinLen = sizeof(sockaddr_in);
	if (SOCKET_ERROR == connect(_socket, (sockaddr*)&sinServer, sinLen))
	{
		printf("Error:连接服务器!\n");
	}
	else
	{
		printf("OK:连接服务器!\n");
	}

	while (true)
	{
		fd_set fdRead;
		FD_ZERO(&fdRead);
		FD_SET(_socket, &fdRead);

		timeval tv = { 1, 0 };

		int ret = select(0, &fdRead, NULL, NULL, &tv);
		if (SOCKET_ERROR == ret)
		{
			printf("Error:Select错误!\n");
			break;
		}

		if (FD_ISSET(_socket, &fdRead))
		{
			FD_CLR(_socket, &fdRead);

			int ret = HandleMsg(_socket);
			if (ret < 0)
			{
				//服务器错误退出循环
				break;
			}
		}

		MsgLogin login = {};
		memcpy(login.name,"illidan", sizeof("illidan"));
		memcpy(login.pwd, "12345", sizeof("12345"));
		send(_socket, (char*)&login, sizeof(MsgLogin), 0);
	}

	//关闭连接
	if (SOCKET_ERROR == closesocket(_socket))
	{
		printf("Error:关闭连接!\n");
	}
	else
	{
		printf("OK:关闭连接!\n");
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