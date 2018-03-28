#ifndef __XRECEIVESERVER_H__
#define __XRECEIVESERVER_H__

#include "XCommon.h"
#include "XIEvent.h"
#include "XClient.h"
#include "XTaskServer.h"
#include "XThread.h"
#include <map>

class XReceiveServer : public XObject<_SERVER_SIZE_, XReceiveServer>
{
public:
	XReceiveServer(int id);
	~XReceiveServer();

	void Init(XIEvent* pEventObj);
	void Done();

	void Start();
	void Stop();

	void AddClient(const std::shared_ptr<XClient> pClient);
	int GetClientNum();
	void AddTask(std::function<void()> pTask);

private:
	int _ID;														//服务线程编号。
	XIEvent* _pEventObj;											//主线程对象

	XTaskServer _TaskServer;										//服务器对应的任务线程。

	std::map<SOCKET, std::shared_ptr<XClient>> _AllClients;			//客户端
	std::map<SOCKET, std::shared_ptr<XClient>> _AllClientsCache;	//客户端缓冲区
	std::mutex _AllClientsCacheMutex;								//客户端缓冲区锁

	XThread _Thread;												//服务线程

private:
	time_t _LastTime;												//上次循环服务器时间

	fd_set _fdSetCache;												//fd_set 缓存。
	bool _ClientChange;												//是否客户端变化。
	SOCKET _MaxSocketID;											//客户端最大Socket。

private:
	void OnRun(XThread* pThread);
};

#endif