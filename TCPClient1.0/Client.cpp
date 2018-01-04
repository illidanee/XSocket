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

		//输入命令
		char cmd[256] = {};
		scanf("%s", cmd);

		//退出
		if (0 == strcmp(cmd, "q"))
		{
			printf("OK:退出程序!\n");
			break;
		}

		//发送客户端请求
		if (0 == strcmp(cmd, "Login"))
		{
			Login login;
			strcpy(login.name, "admin");
			strcpy(login.pwd, "12345");
			send(_socket, (char*)&login, sizeof(Login), 0);
		}
		else if (0 == strcmp(cmd, "Logout"))
		{
			Logout logout;
			strcpy(logout.name, "admin");
			send(_socket, (char*)&logout, sizeof(Logout), 0);
		}
		else
		{
			printf("WARNING:无效的命令!\n");
			continue;
		}

		//接收服务器响应
		MsgHeader respond = {};
		int size = recv(_socket, (char*)&respond, sizeof(MsgHeader), 0);
		if (SOCKET_ERROR == size)
		{
			printf("Error:接收服务器响应!\n");
			break;
		}
		else if (0 == size)
		{
			printf("OK:服务器关闭连接!\n");
			break;
		}

		//显示服务器响应
		printf("--接收服务器响应：Type:%d - Length:%d\n", respond.msgType, respond.msgLength);

		//处理服务器响应
		switch (respond.msgType)
		{
		case MSG_LOGIN_RES:
		{
			LoginRes result;
			recv(_socket, (char*)&result + sizeof(MsgHeader), sizeof(LoginRes) - sizeof(MsgHeader), 0);

			printf("--登入结果：Res:%d\n", result.res);
		}
		break;
		case MSG_LOGOUT_RES:
		{
			LogoutRes result;
			recv(_socket, (char*)&result + sizeof(MsgHeader), sizeof(LogoutRes) - sizeof(MsgHeader), 0);

			printf("--登出结果：Res:%d\n", result.res);
		}
		break;
		default:
		{

		}
		}

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