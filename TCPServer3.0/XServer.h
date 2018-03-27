#ifndef __XSERVER_H__
#define __XSERVER_H__

#include "XCommon.h"
#include "XIEvent.h"
#include "XReceiveServer.h"
#include "XClient.h"
#include "XSignal.h"
#include <vector>

//����Server��
class XServer : public XIEvent
{
public:
	XServer();
	~XServer();

	int Start();
	int OnRun();
	int Stop();

private:
	SOCKET _Socket;												//����������Socket
	std::vector<std::shared_ptr<XReceiveServer>> _AllServers;	//��������Ϣ

	bool _Run;													//��ǰ�߳��Ƿ�����
	XSignal _Signal;											//ͬ���ź�

private:
	int Init();
	int Done();

	int Open();
	int Close();

	int Bind(const char* ip, unsigned short port);
	int Listen(int n);
	int Accept();
};


#endif