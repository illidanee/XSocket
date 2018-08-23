﻿#ifndef __XSERVER_H__
#define __XSERVER_H__

#include "XCommon.h"
#include "XIGlobalEvent.h"
#include "XIServerEvent.h"

#include "XClient.h"
#include "XTaskServer.h"
#include "XThread.h"
#include <map>

class XServer : public XIServerEvent
{
public:
	XServer();
	virtual ~XServer();

	virtual void Init(XIGlobalEvent* pGlobalEventObj, int id);
	virtual void Done();

	void Start();
	void Stop();

public:
	virtual void AddTask(std::function<void()> pTask);

public:
	XIGlobalEvent* GetGlobalObj();

	void AddClient(const std::shared_ptr<XClient> pClient);
	int GetClientNum();

protected:
	void OnRun(XThread* pThread);

	void UpdateFrameTimeDelta();
	int CheckClientNum();
	int CheckClientState();
	virtual int DoNetEvent() = 0;

	//void RecvMsg();
	//void SendMsg();
	void DoMsg();

protected:
	XIGlobalEvent* _pGlobalEventObj;								//全局事件对象

	int _ID;														//服务线程ID
	XThread _Thread;												//服务线程
	std::map<SOCKET, std::shared_ptr<XClient>> _AllClients;			//客户端
	std::map<SOCKET, std::shared_ptr<XClient>> _AllClientsCache;	//客户端缓冲区
	std::mutex _AllClientsCacheMutex;								//客户端缓冲区锁

protected:
	time_t _LastFrameTime;											//上次循环服务器时间
	time_t _CurFrameTime;											//本次循环服务器时间
	time_t _FrameTimeDelta;											//每次循环服务器时间间隔

	//此集合占用大量的内存,使对象大小变大。
	//XFdSet _FdRead;
	//XFdSet _FdWrite;
	//XFdSet _FdSetCache;												//fd_set 缓存
	bool _ClientChange;												//是否客户端变化
	//SOCKET _MaxSocketID;											//客户端最大Socket

protected:
	XTaskServer _TaskServer;										//服务器对应的任务线程
};

#endif