#define WIN32_LEAN_AND_MEAN

#include <vector>

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

std::vector<SOCKET> g_AllClients;

int HandleMsg(SOCKET client)
{
	//接收客户端请求
	char buffer[1024] = {};
	int size = recv(client, buffer, sizeof(MsgHeader), 0);
	MsgHeader* request = (MsgHeader*)buffer;
	if (SOCKET_ERROR == size)
	{
		printf("Error:接收客户端<Socket=%d>请求!\n", client); //点X强行关闭。
		return -1;
	}
	else if (size == 0)
	{
		printf("OK:客户端<Socket=%d>关闭连接!\n", client);	 //用q命令关闭。
		return -2;
	}

	//显示客户端请求
	printf("--接收客户端<Socket=%d>请求：Type:%d - Length:%d\n", client, request->msgType, request->msgLength);

	//处理客户端请求
	switch (request->msgType)
	{
	case MSG_LOGIN:
	{
		recv(client, buffer + sizeof(MsgHeader), request->msgLength - sizeof(MsgHeader), 0);
		MsgLogin* login = (MsgLogin*)buffer;

		printf("--登入消息内容：Name:%s - Pwd:%s\n", login->name, login->pwd);

		MsgLoginRes respond;
		send(client, (char*)&respond, sizeof(MsgLoginRes), 0);
	}
	break;
	case MSG_LOGOUT:
	{
		recv(client, buffer + sizeof(MsgHeader), request->msgLength - sizeof(MsgHeader), 0);
		MsgLogout* logout = (MsgLogout*)buffer;

		printf("--登出消息内容：Name:%s\n", logout->name);

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
	sin.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
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

	while (true)
	{
		fd_set fdRead;
		FD_ZERO(&fdRead);
		FD_SET(_socket, &fdRead);

		for (size_t i = 0; i < g_AllClients.size(); ++i)
		{
			FD_SET(g_AllClients[i], &fdRead);
		}
		
		timeval tv = { 0, 0 };

		int ret = select(0, &fdRead, NULL, NULL, &tv);
		if (SOCKET_ERROR == ret)
		{
			printf("Error:Select错误!\n");
			break;
		}

		if (FD_ISSET(_socket, &fdRead))
		{
			FD_CLR(_socket, &fdRead);

			//等待客户端连接
			sockaddr_in sinClient = {};
			int sinLen = sizeof(sockaddr_in);
			SOCKET _client = INVALID_SOCKET;
			_client = accept(_socket, (sockaddr*)&sinClient, &sinLen);
			if (_client == INVALID_SOCKET)
			{
				printf("Error:客户端<Socket=%d>连接!\n", _client);
			}
			else
			{
				printf("OK:客户端<Socket=%d>连接!\n", _client);

				//转发客户登陆消息
				MsgNewUser msgNewUser;
				msgNewUser.user = _client;

				for (std::vector<SOCKET>::iterator it = g_AllClients.begin(); it != g_AllClients.end(); ++it)
				{
					send(*it, (char*)&msgNewUser, sizeof(MsgNewUser), 0);
				}

				//添加客户端
				g_AllClients.push_back(_client);
			}
		}

		for (unsigned int i = 0; i < fdRead.fd_count; ++i)
		{
			int ret = HandleMsg(fdRead.fd_array[i]);
			if (ret < 0)
			{
				//删除客户端
				std::vector<SOCKET>::iterator iter = find(g_AllClients.begin(), g_AllClients.end(), fdRead.fd_array[i]);
				if (iter != g_AllClients.end())
					g_AllClients.erase(iter);

				//关闭客户端连接
				if (SOCKET_ERROR == closesocket(fdRead.fd_array[i]))
				{
					printf("Error:关闭客户端<Socket=%d>连接!\n", fdRead.fd_array[i]);
				}
				else
				{
					printf("OK:关闭客户端<Socket=%d>连接!\n", fdRead.fd_array[i]);
				}
			}
		}
	}

	//关闭客户端连接
	for (std::vector<SOCKET>::iterator it = g_AllClients.begin(); it != g_AllClients.end(); ++it)
	{
		//关闭客户端连接
		if (SOCKET_ERROR == closesocket(*it))
		{
			printf("Error:关闭客户端连接!\n");
		}
		else
		{
			printf("OK:关闭客户端连接!\n");
		}
	}
	g_AllClients.clear();

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