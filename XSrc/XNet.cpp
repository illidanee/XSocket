#include "XNet.h"

void XNet::Go()
{
	static XNet obj;
}

XNet::XNet()
{
//初始化网络环境
#ifdef _WIN32
	WORD wsaVersion = MAKEWORD(2, 2);
	WSADATA wsaData = {};
	int iRet = WSAStartup(wsaVersion, &wsaData);
	if (iRet)
	{
		XError("WSAStartup!\n");
	}
	else
	{
		XInfo("WSAStartup!\n");
	}
#endif
}

XNet::~XNet()
{
//销毁网络环境
#ifdef _WIN32
	int iRet = WSACleanup();
	if (SOCKET_ERROR == iRet)
	{
		XError("WSACleanup!\n");
	}
	else
	{
		XInfo("WSACleanup!\n");
	}
#endif
}