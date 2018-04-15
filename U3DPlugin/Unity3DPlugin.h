#ifndef __UNITY3DPLUGIN_H__
#define __UNITY3DPLUGIN_H__

#if _WIN32
#define EXPORT_DLL _declspec(dllexport)
#else
#define EXPORT_DLL
#endif

#include "../XSrc/Server/XTCPClient.h"
#include "../XSrc/ByteStream/XRecvByteStream.h"

class MyClient : public XTCPClient
{
public:
	void DoMsg(MsgHeader* pMsgHeader);

private:
	virtual void OnRunLoopBegin();
	virtual void OnClientJoin(XClient* pClient);
	virtual void OnClientLeave(XClient* pClient);
	virtual void OnNetRecv(XClient* pClient);
	virtual void OnNetSend(XClient* pClient);
	virtual void OnNetMsgRecv(XClient* pClient, MsgHeader* pMsgHeader);
	virtual void OnNetMsgDone(XClient* pClient, MsgHeader* pMsgHeader);

	virtual void AddTask(std::function<void()> pTask);
};

extern "C"
{
	EXPORT_DLL void SetLogPath(const char* pLogPath);

	EXPORT_DLL MyClient* Open();
	EXPORT_DLL bool Connect(MyClient* pClient, const char* ip, unsigned short port);
	EXPORT_DLL void Close(MyClient* pClient);

	EXPORT_DLL bool IsRun(MyClient* pClient);
	EXPORT_DLL void OnRun(MyClient* pClient);

	EXPORT_DLL int SendData(MyClient* pClient, MsgHeader* pHeader);
}

#endif