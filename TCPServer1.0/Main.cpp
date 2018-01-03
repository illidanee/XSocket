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

	//��IP�Ͷ˿�
	sockaddr_in sin = {};
	sin.sin_family = AF_INET;
	sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	sin.sin_port = htons(9090);
	if (SOCKET_ERROR == bind(_socket, (sockaddr*)&sin, sizeof(sockaddr_in)))
	{
		printf("Error:��IP�Ͷ˿�!\n");
	}
	else
	{
		printf("OK:��IP�Ͷ˿�!\n");
	}

	//��������
	if (SOCKET_ERROR == listen(_socket, 10))
	{
		printf("Error:��������!\n");
	}
	else
	{
		printf("OK:��������!\n");
	}

	//�ȴ��ͻ�������
	sockaddr_in sinClient = {};
	int sinLen = sizeof(sockaddr_in);
	SOCKET _client = INVALID_SOCKET;
	_client = accept(_socket, (sockaddr*)&sinClient, &sinLen);
	if (_client == INVALID_SOCKET)
	{
		printf("Error:�ͻ�������!\n");
	}
	else
	{
		printf("OK:�ͻ�������!\n");
	}

	//��ͻ��˷�����Ϣ
	char buffer[] = "Hello! I`m Server!";
	int size = send(_client, "Hello! I`m Server!", sizeof(buffer), 0);
	if (SOCKET_ERROR  == size)
	{
		printf("Error:��ͻ��˷�����Ϣ!\n");
	}
	else
	{
		printf("OK:��ͻ��˷�����Ϣ!\n");
	}

	//�رտͻ�������
	if (SOCKET_ERROR == closesocket(_client))
	{
		printf("Error:�رտͻ�������!\n");
	}
	else
	{
		printf("OK:�رտͻ�������!\n");
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