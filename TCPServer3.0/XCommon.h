#ifndef __XCOMMON_H__
#define __XCOMMON_H__

//�Զ����
#define _SERVER_SIZE_ 4
#define _RECV_BUFFER_SIZE_ 10240
#define _SEND_BUFFER_SIZE_ 10240

//�ڴ��
#include "../Memory/XMemory.h"

//�����
#include "../Object/XObject.h"

//��ʱ��
#include "XTimer.h"

//Э��ͷ
#include "MsgProtocol.h"

//Socket���
#ifdef _WIN32
#define FD_SETSIZE 1024
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

//ϵͳͷ�ļ�
#include <stdlib.h>
#include <stdio.h>
#include <memory>

#endif