#define WIN32_LEAN_AND_MEAN

#include <stdio.h>
#include <windows.h>
#include <WinSock2.h>

int main()
{
	//������
	WORD wsaVersion = MAKEWORD(2, 2);
	WSADATA wsaData = {};
	int iErrorNo = WSAStartup(wsaVersion, &wsaData);
	if (iErrorNo)
	{
		printf("Error:WSA start up failed!\n");
	}

	//����Socket
	SOCKET _socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SOCKET_ERROR == _socket)
	{
		printf("Error:����Socket!\n");
	}
	else
	{
		printf("OK:����Socket!\n");
	}

	//���ӷ�����
	sockaddr_in sinServer = {};
	sinServer.sin_family = AF_INET;
	sinServer.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	sinServer.sin_port = htons(9090);
	int sinLen = sizeof(sockaddr_in);
	if (SOCKET_ERROR == connect(_socket, (sockaddr*)&sinServer, sinLen))
	{
		printf("Error:���ӷ�����!\n");
	}
	else
	{
		printf("OK:���ӷ�����!\n");
	}

	//���ܷ���������
	char buffer[256] = {};
	int size = recv(_socket, buffer, 256, 0);
	if (SOCKET_ERROR == size)
	{
		printf("Error:���ܷ���������!\n");
	}
	else
	{
		printf("OK:���ܷ���������! - %s\n", buffer);
	}

	//�ر�����
	if (SOCKET_ERROR == closesocket(_socket))
	{
		printf("Error:�ر�����!\n");
	}
	else
	{
		printf("OK:�ر�����!\n");
	}

	//�ر�����
	int iError = WSACleanup();
	if (SOCKET_ERROR == iError)
	{
		printf("Error:WSA clean up failed!\n");
	}

	//�˳�
	getchar();
	return 0;
}