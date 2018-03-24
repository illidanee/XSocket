#ifndef __XRECEIVESERVER_H__
#define __XRECEIVESERVER_H__

#include "XCommon.h"
#include "XIEvent.h"
#include "XClient.h"
#include "XTask.h"
#include "XTaskServer.h"
#include <map>

class XReceiveServer : public XObject<_SERVER_SIZE_, XReceiveServer>
{
private:
	XIEvent* _pNetEventObj;											//���̶߳���

	std::map<SOCKET, std::shared_ptr<XClient>> _AllClients;			//�ͻ���
	std::map<SOCKET, std::shared_ptr<XClient>> _AllClientsCache;	//�ͻ��˻�����
	std::mutex _AllClientsCacheMutex;								//�ͻ��˻�������

private:
	XTaskServer _TaskServer;										//��������Ӧ�������̡߳�

private:
	fd_set _fdSetCache;
	bool _ClientChange;
	SOCKET _MaxSocketID;

public:
	XReceiveServer();
	~XReceiveServer();

	void SetNetEventObj(XIEvent* pEventObj);

	int Start();
	int OnRun();

	void AddClient(const std::shared_ptr<XClient>& pClient);
	int GetClientNum();
	void AddTask(const std::shared_ptr<XTask>& pTask);
};

#endif