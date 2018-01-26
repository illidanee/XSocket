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

std::vector<SOCKET> g_AllClients;

int HandleMsg(SOCKET client)
{
	//���տͻ�������
	char buffer[1024] = {};
	int size = recv(client, buffer, sizeof(MsgHeader), 0);
	MsgHeader* request = (MsgHeader*)buffer;
	if (SOCKET_ERROR == size)
	{
		printf("Error:���տͻ�������!\n"); //��Xǿ�йرա�
		return -1;
	}
	else if (size == 0)
	{
		printf("OK:�ͻ��˹ر�����!\n");	 //��q����رա�
		return -2;
	}

	//��ʾ�ͻ�������
	printf("--���տͻ�������Type:%d - Length:%d\n", request->msgType, request->msgLength);

	//����ͻ�������
	switch (request->msgType)
	{
	case MSG_LOGIN:
	{
		recv(client, buffer + sizeof(MsgHeader), request->msgLength - sizeof(MsgHeader), 0);
		Login* login = (Login*)buffer;

		printf("--������Ϣ���ݣ�Name:%s - Pwd:%s\n", login->name, login->pwd);

		LoginRes respond;
		send(client, (char*)&respond, sizeof(LoginRes), 0);
	}
	break;
	case MSG_LOGOUT:
	{
		recv(client, buffer + sizeof(MsgHeader), request->msgLength - sizeof(MsgHeader), 0);
		Logout* logout = (Logout*)buffer;

		printf("--�ǳ���Ϣ���ݣ�Name:%s\n", logout->name);

		LogoutRes respond;
		send(client, (char*)&respond, sizeof(LogoutRes), 0);
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

	while (true)
	{
		fd_set fdRead;
		fd_set fdWrite;
		fd_set fdExp;

		FD_ZERO(&fdRead);
		FD_ZERO(&fdWrite);
		FD_ZERO(&fdExp);

		FD_SET(_socket, &fdRead);
		FD_SET(_socket, &fdWrite);
		FD_SET(_socket, &fdExp);

		for (size_t i = 0; i < g_AllClients.size(); ++i)
		{
			FD_SET(g_AllClients[i], &fdRead);
		}
		
		timeval tv = { 0, 0 };

		int ret = select(0, &fdRead, &fdWrite, &fdExp, &tv);
		if (SOCKET_ERROR == ret)
		{
			printf("Error:Select����!\n");
			break;
		}

		if (FD_ISSET(_socket, &fdRead))
		{
			FD_CLR(_socket, &fdRead);

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

				g_AllClients.push_back(_client);
			}
		}

		for (unsigned int i = 0; i < fdRead.fd_count; ++i)
		{
			if (HandleMsg(fdRead.fd_array[i]) < 0)
			{
				std::vector<SOCKET>::iterator iter = find(g_AllClients.begin(), g_AllClients.end(), fdRead.fd_array[i]);
				if (iter != g_AllClients.end())
					g_AllClients.erase(iter);

				//�رտͻ�������
				if (SOCKET_ERROR == closesocket(fdRead.fd_array[i]))
				{
					printf("Error:�رտͻ�������!\n");
				}
				else
				{
					printf("OK:�رտͻ�������!\n");
				}
			}
		}
	}

	//�رտͻ�������
	for (std::vector<SOCKET>::iterator it = g_AllClients.begin(); it != g_AllClients.end(); ++it)
	{
		//�رտͻ�������
		if (SOCKET_ERROR == closesocket(*it))
		{
			printf("Error:�رտͻ�������!\n");
		}
		else
		{
			printf("OK:�رտͻ�������!\n");
		}
	}
	g_AllClients.clear();

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