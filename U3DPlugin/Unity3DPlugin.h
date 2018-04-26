#ifndef __UNITY3DPLUGIN_H__
#define __UNITY3DPLUGIN_H__

#ifndef __NO_DLL_INS__
#if _WIN32
#define EXPORT_DLL _declspec(dllexport)
#else
#define EXPORT_DLL
#endif
#else
#define EXPORT_DLL
#endif

#include "../XSrc/Server/XTCPClient.h"
#include "../XSrc/ByteStream/XRecvByteStream.h"
#include "../XSrc/ByteStream/XSendByteStream.h"


typedef void(*OnMsgCallback)(void* csObj, MsgHeader* pMsgHeader);
typedef void(*OnLeaveCallback)(void* csObj);

//----------------------------------------------------------------------------------------------------------------------------
//类定义
class MyClient : public XTCPClient
{
public:
	MyClient();
	void Init(void* pObj, OnMsgCallback pMsgCallback, OnLeaveCallback pLeaveCallback);
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

private:
	void* _pObj;								//调用者对象。
	OnMsgCallback _pMsgCallback;				//调用者消息回调函数
	OnLeaveCallback _pLeaveCallback;			//调用者断开连接回掉函数(服务器断开通知)
};

extern "C"
{
	//----------------------------------------------------------------------------------------------------------------------------
	//导出日志接口 - Log
	EXPORT_DLL void SetLogPath(const char* pLogPath);

	//----------------------------------------------------------------------------------------------------------------------------
	//导出客户端接口 - MyClient
	EXPORT_DLL MyClient* Open(void* pObj, OnMsgCallback pMsgCallback, OnLeaveCallback pLeaveCallback);
	EXPORT_DLL bool Connect(MyClient* pClient, const char* ip, unsigned short port);
	EXPORT_DLL void Disconnect(MyClient* pClient);
	EXPORT_DLL void Close(MyClient* pClient);
	EXPORT_DLL bool IsRun(MyClient* pClient);
	EXPORT_DLL void OnRun(MyClient* pClient);
	EXPORT_DLL int SendStream(MyClient* pClient, XByteStream* pStream);

	//----------------------------------------------------------------------------------------------------------------------------
	//导出字节流接口 - ReadStream
	EXPORT_DLL XRecvByteStream* CppRecvStreamCreate(MsgHeader* pMsgHeader);
	EXPORT_DLL void CppRecvStreamClose(XRecvByteStream* pStream);
	EXPORT_DLL int8_t CppReadInt8(XRecvByteStream* pStream, int8_t n = 0);
	EXPORT_DLL int16_t CppReadInt16(XRecvByteStream* pStream, int16_t n = 0);
	EXPORT_DLL int32_t CppReadInt32(XRecvByteStream* pStream, int32_t n = 0);
	EXPORT_DLL int64_t CppReadInt64(XRecvByteStream* pStream, int64_t n = 0);
	EXPORT_DLL float CppReadFloat(XRecvByteStream* pStream, float n = 0.0f);
	EXPORT_DLL double CppReadDouble(XRecvByteStream* pStream, double n = 0.0);
	EXPORT_DLL int CppReadString(XRecvByteStream* pStream, char* pBuffer, int nSize);
	
	//----------------------------------------------------------------------------------------------------------------------------
	//导出字节流接口 - WriteStream
	EXPORT_DLL XSendByteStream* CppSendStreamCreate(int nSize = 128);
	EXPORT_DLL void CppSendStreamClose(XSendByteStream* pStream);
	EXPORT_DLL bool CppWriteInt8(XSendByteStream* pStream, int8_t n);
	EXPORT_DLL bool CppWriteInt16(XSendByteStream* pStream, int16_t n);
	EXPORT_DLL bool CppWriteInt32(XSendByteStream* pStream, int32_t n);
	EXPORT_DLL bool CppWriteInt64(XSendByteStream* pStream, int64_t n);
	EXPORT_DLL bool CppWriteFloat(XSendByteStream* pStream, float n);
	EXPORT_DLL bool CppWriteDouble(XSendByteStream* pStream, double n);
	EXPORT_DLL int CppWriteString(XSendByteStream* pStream, const char* pBuffer, int nSize);
	EXPORT_DLL void CppFinish(XSendByteStream* pStream, MGS_TYPE type);
}

#endif