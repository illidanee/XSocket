﻿#ifndef __XCOMMON_H__
#define __XCOMMON_H__

//自定义宏
#define _SERVER_SIZE_ 4
#define _RECV_BUFFER_SIZE_ 8192
#define _SEND_BUFFER_SIZE_ 10240
#define _XCLIENT_HEART_TIME_ 60000000
#define _XCLIENT_SEND_TIME_ 200000

#define _MARIADB_CONNECT_NUM_ 1000

//日志
#include "../Log/XLog.h"

//内存池
#include "../Memory/XMemory.h"

//对象池
#include "../Object/XObject.h"

//计时器
#include "../Timer/XTimer.h"

//协议头
#include "MsgProtocol.h"

//Socket相关
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

//系统头文件
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory>

#endif