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
	int _ID;														//�����̱߳�š�
	XIEvent* _pEventObj;											//���̶߳���

	XTaskServer _TaskServer;										//��������Ӧ�������̡߳�

	std::map<SOCKET, std::shared_ptr<XClient>> _AllClients;			//�ͻ���
	std::map<SOCKET, std::shared_ptr<XClient>> _AllClientsCache;	//�ͻ��˻�����
	std::mutex _AllClientsCacheMutex;								//�ͻ��˻�������

	XThread _Thread;												//�����߳�

private:
	time_t _LastTime;												//�ϴ�ѭ��������ʱ��

	fd_set _fdSetCache;												//fd_set ���档
	bool _ClientChange;												//�Ƿ�ͻ��˱仯��
	SOCKET _MaxSocketID;											//�ͻ������Socket��

private:
	void OnRun(XThread* pThread);
};

#endif