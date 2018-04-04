#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "../XCommon.h"
#include "XClient.h"

//Client��
class XTCPClient : public XIGlobalEvent
{
public:
	XTCPClient();
	~XTCPClient();

	void Open();
	void Connect(const char* ip, unsigned short port);
	void Close();

	bool IsRun();
	void OnRun();

	int SendData(MsgHeader* pHeader, int len);
	virtual int OnNetMsg(MsgHeader* pHeader);

private:
	XClient* _Client;
};

#endif