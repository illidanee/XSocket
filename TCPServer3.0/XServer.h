#ifndef __XSERVER_H__
#define __XSERVER_H__

#include "XCommon.h"
#include "XIEvent.h"
#include "XReceiveServer.h"
#include "XClient.h"
#include "XThread.h"
#include <vector>

//����Server��
class XServer : public XIEvent
{
public:
	XServer();
	~XServer();

	int Start();
	int Stop();

private:
	SOCKET _Socket;												//����������Socket
	std::vector<std::shared_ptr<XReceiveServer>> _AllServers;	//��������Ϣ

	XThread _Thread;											//�����߳�

private:
	void Init();
	void Open();
	void Bind(const char* ip, unsigned short port);
	void Listen(int n);

	void Close();
	void Done();

	void Accept();

	void OnRun(XThread* pThread);
};


#endif