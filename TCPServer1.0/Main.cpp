#define WIN32_LEAN_AND_MEAN

#include <stdio.h>
#include <windows.h>
#include <WinSock2.h>

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

	//向客户端发送消息
	char buffer[] = "Hello! I`m Server!";
	int size = send(_client, "Hello! I`m Server!", sizeof(buffer), 0);
	if (SOCKET_ERROR  == size)
	{
		printf("Error:向客户端发送消息!\n");
	}
	else
	{
		printf("OK:向客户端发送消息!\n");
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