#ifndef __CLIENT_H__
#define __CLIENT_H__

//��׼ͷ�ļ�
#include <iostream>

#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#define _WINSOCK_DEPRECATED_NO_WARNINGS
	#include <windows.h>
	#include <WinSock2.h>
	#pragma comment(lib, "ws2_32.lib")
#else
	#include <unistd.h>
	#include <string.h>
	#include <arpa/inet.h>

	#define SOCKET int
	#define INVALID_SOCKET  (SOCKET)(~0)
	#define SOCKET_ERROR    (-1)
#endif // _WIN32

//Э��ͷ�ļ�
#include "MsgProtocol.h"

#define _BUFFER_SIZE_ 10240

//Client��
class Client
{
private:
	SOCKET _Socket;							//�ͻ���Socket

	char _RecvBuffer[_BUFFER_SIZE_];		//���ջ�����
	char _DataBuffer[_BUFFER_SIZE_ * 10];	//���ݻ�����
	int _StartPos;							//���ݻ������п��Է������ݵ���ʼλ��

public:
	Client();
	~Client();

	int Init();
	int Done();

	int Open();
	int Connect(const char* ip, unsigned short port);
	int Close();

	int IsRun();
	int OnRun();

	int SendData(MsgHeader* pHeader);
	int RecvData();
	virtual int OnNetMsg(MsgHeader* pHeader);
};

#endif