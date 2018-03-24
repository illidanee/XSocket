#ifndef __XSERVER_H__
#define __XSERVER_H__

#include "XCommon.h"
#include "XIEvent.h"
#include "XReceiveServer.h"
#include "XClient.h"
#include <vector>

//����Server��
class XServer : public XIEvent
{
private:
	SOCKET _Socket;												//����������Socket
	std::vector<std::shared_ptr<XReceiveServer>> _AllServers;	//��������Ϣ

public:
	XServer();							
	~XServer();

	int Init();
	int Done();

	int Open();
	int Bind(const char* ip, unsigned short port);
	int Listen(int n);
	int Accept();
	int Start();
	int Close();

	int IsRun();
	int OnRun();
};


#endif