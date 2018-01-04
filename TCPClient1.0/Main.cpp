#define WIN32_LEAN_AND_MEAN

#include <stdio.h>
#include <windows.h>
#include <WinSock2.h>

struct Msg
{
	int age;
	char name[32];
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

		if (0 == strcmp(cmd, "q"))
		{
			printf("OK:退出程序!\n");
			break;
		}

		//发送命令
		int size = send(_socket, cmd, sizeof(cmd), 0);
		if (SOCKET_ERROR == size)
		{
			printf("Error:发送命令!\n");
			break;
		}
		else
		{
			printf("OK:发送命令!\n");
		}

		//接受服务器消息
		char buffer[256] = {};
		size = recv(_socket, buffer, 256, 0);
		if (SOCKET_ERROR == size)
		{
			printf("Error:接受服务器消息!\n");
			break;
		}
		else if (0 == size)
		{
			printf("OK:服务器关闭连接!\n");
			break;
		}

		Msg* msg = (Msg*)buffer;
		printf("OK:接受服务器数据! - %s - %d\n", msg->name, msg->age);

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