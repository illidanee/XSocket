#ifndef __XRECEIVESERVER_H__
#define __XRECEIVESERVER_H__

#include "XCommon.h"
#include "XIGlobalEvent.h"
#include "XIServerEvent.h"

#include "XClient.h"
#include "XTaskServer.h"
#include "XThread.h"
#include <map>

class XServer : public XIServerEvent, public XObject<_SERVER_SIZE_, XServer>
{
public:
	XServer(int id);
	~XServer();

	void Init(XIGlobalEvent* pGlobalEventObj);
	void Done();

	void Start();
	void Stop();

	void AddClient(const std::shared_ptr<XClient> pClient);
	int GetClientNum();

	XIGlobalEvent* GetGlobalObj();

public:
	virtual void AddTask(std::function<void()> pTask);

private:
	int _ID;														//服务线程编号

	XIGlobalEvent* _pGlobalEventObj;								//全局事件对象

	std::map<SOCKET, std::shared_ptr<XClient>> _AllClients;			//客户端
	std::map<SOCKET, std::shared_ptr<XClient>> _AllClientsCache;	//客户端缓冲区
	std::mutex _AllClientsCacheMutex;								//客户端缓冲区锁

	XThread _Thread;												//服务线程

	XTaskServer _TaskServer;										//服务器对应的任务线程

private:
	time_t _LastTime;												//上次循环服务器时间
	time_t _CurTime;												//本次循环服务器时间
	time_t _TimeDelta;												//每次循环服务器时间间隔

	fd_set _fdSetCache;												//fd_set 缓存
	bool _ClientChange;												//是否客户端变化
	SOCKET _MaxSocketID;											//客户端最大Socket

private:
	void OnRun(XThread* pThread);

	void TimeDelta();
	void RecvData(fd_set& fdSet);
	void SendData(fd_set& fdSet);
	void CheckTime();
};

#endif