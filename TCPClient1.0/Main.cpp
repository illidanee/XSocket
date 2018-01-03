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

	//接受服务器数据
	char buffer[256] = {};
	int size = recv(_socket, buffer, 256, 0);
	if (SOCKET_ERROR == size)
	{
		printf("Error:接受服务器数据!\n");
	}
	else
	{
		printf("OK:接受服务器数据! - %s\n", buffer);
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