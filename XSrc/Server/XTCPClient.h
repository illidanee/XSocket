#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "../XCommon.h"
#include "XIGlobalEvent.h"
#include "XIServerEvent.h"
#include "XNet.h"
#include "XClient.h"
#include "../ByteStream/XSendByteStream.h"

//Client¿‡
class XTCPClient : public XIGlobalEvent, public XIServerEvent
{
public:
	XTCPClient();
	~XTCPClient();

	void Open();
	bool Connect(const char* ip, unsigned short port);
	void Disconnect();
	void Close();

	bool IsRun();
	void OnRun();

	int SendData(MsgHeader* pHeader);
	int SendStream(XByteStream* pStream);

private:
	XClient* _Client;

	bool _bRun;
};

#endif