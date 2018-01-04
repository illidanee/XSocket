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

	while (true)
	{

		//���տͻ�������
		MsgHeader request = {};
		int size = recv(_client, (char*)&request, sizeof(MsgHeader), 0);
		if (SOCKET_ERROR == size)
		{
			printf("Error:���տͻ�������!\n");
			break;
		}
		else if (size == 0)
		{
			printf("OK:�ͻ��˹ر�����!\n");
			break;
		}

		//��ʾ�ͻ�������
		printf("--���տͻ�������Type:%d - Length:%d\n", request.msgType, request.msgLength);

		//�����ͻ�������
		switch (request.msgType)
		{
		case MSG_LOGIN:
		{
			Login login;
			recv(_client, (char*)&login + sizeof(MsgHeader), sizeof(Login) - sizeof(MsgHeader), 0);

			printf("--������Ϣ���ݣ�Name:%s - Pwd:%s\n", login.name, login.pwd);

			LoginRes respond;
			send(_client, (char*)&respond, sizeof(LoginRes), 0);
		}
		break;
		case MSG_LOGOUT:
		{
			Logout logout;
			recv(_client, (char*)&logout + sizeof(MsgHeader), sizeof(Logout) - sizeof(MsgHeader), 0);

			printf("--�ǳ���Ϣ���ݣ�Name:%s\n", logout.name);

			LogoutRes respond;
			send(_client, (char*)&respond, sizeof(LogoutRes), 0);
		}
		break;
		default:
		{

		}
		}

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