#include "Unity3DPlugin.h"

//----------------------------------------------------------------------------------------------------------------------------
//类定义
MyClient::MyClient()
{
	_pObj = nullptr;
	_pCallback = nullptr;
}

void MyClient::Init(void* pObj, OnMsg pCallback)
{
	_pObj = pObj;
	_pCallback = pCallback;
}

void MyClient::DoMsg(MsgHeader* pMsgHeader)
{
	if (_pObj && _pCallback)
		_pCallback(_pObj, pMsgHeader);
}

void MyClient::OnRunLoopBegin()
{

}

void MyClient::OnClientJoin(XClient* pClient)
{

}
void MyClient::OnClientLeave(XClient* pClient)
{

}
void MyClient::OnNetRecv(XClient* pClient)
{

}

void MyClient::OnNetSend(XClient* pClient)
{

}

void MyClient::OnNetMsgRecv(XClient* pClient, MsgHeader* pMsgHeader)
{
	DoMsg(pMsgHeader);

	//XRecvByteStream r(pMsgHeader);
	//int8_t r1;
	//r.ReadInt8(r1);
	//int16_t r2;
	//r.ReadInt16(r2);
	//int32_t r3;
	//r.ReadInt32(r3);
	//int64_t r4;
	//r.ReadInt64(r4);
	//float r5;
	//r.ReadFloat(r5);
	//double r6;
	//r.ReadDouble(r6);
	//char aa[32] = {};
	//r.ReadArray(aa, 32);
	//int bb[32] = {};
	//r.ReadArray(bb, 32);
}

void MyClient::OnNetMsgDone(XClient* pClient, MsgHeader* pMsgHeader)
{

}

void MyClient::AddTask(std::function<void()> pTask)
{

}

//----------------------------------------------------------------------------------------------------------------------------
//导出日志接口 - Log
void SetLogPath(const char* pLogPath)
{
	XLog::SetFile(pLogPath, "w");
}

//----------------------------------------------------------------------------------------------------------------------------
//导出客户端接口 - MyClient
MyClient* Open(void* pObj, OnMsg pCallback)
{
	MyClient* pClient = new MyClient();
	pClient->Init(pObj, pCallback);
	pClient->Open();
	return pClient;
}

bool Connect(MyClient* pClient, const char* ip, unsigned short port)
{
	if (pClient)
		return pClient->Connect(ip, port);

	return false;
}

void Disconnect(MyClient* pClient)
{
	if (pClient)
		pClient->Disconnect();
}

void Close(MyClient* pClient)
{
	if (pClient)
	{
		pClient->Close();
		delete pClient;
		pClient = nullptr;
	}
}

bool IsRun(MyClient* pClient)
{
	if (pClient)
		return pClient->IsRun();

	return false;
}

void OnRun(MyClient* pClient)
{
	if (pClient)
		pClient->OnRun();
}

int SendStream(MyClient* pClient, XByteStream* pStream)
{
	if (pClient)
		return pClient->SendStream(pStream);

	return 0;
}

//----------------------------------------------------------------------------------------------------------------------------
//导出字节流接口 - ReadStream
XRecvByteStream* CppRecvStreamCreate(MsgHeader* pMsgHeader)
{
	XRecvByteStream* pStream = new XRecvByteStream(pMsgHeader);
	return pStream;
}

void CppRecvStreamClose(XRecvByteStream* pStream)
{
	if (pStream)
	{
		delete pStream;
		pStream = nullptr;
	}
}

int8_t CppReadInt8(XRecvByteStream* pStream,  int8_t n)
{
	if (pStream)
		pStream->ReadInt8(n);

	return n;
}

int16_t CppReadInt16(XRecvByteStream* pStream, int16_t n)
{
	if (pStream)
		pStream->ReadInt16(n);

	return n;
}

int32_t CppReadInt32(XRecvByteStream* pStream, int32_t n)
{
	if (pStream)
		pStream->ReadInt32(n);

	return n;
}

int64_t CppReadInt64(XRecvByteStream* pStream, int64_t n)
{
	if (pStream)
		pStream->ReadInt64(n);

	return n;
}

float CppReadFloat(XRecvByteStream* pStream, float n)
{
	if (pStream)
		pStream->ReadFloat(n);

	return n;
}

double CppReadDouble(XRecvByteStream* pStream, double n)
{
	if (pStream)
		pStream->ReadDouble(n);

	return n;
}

int CppReadString(XRecvByteStream* pStream, char* pBuffer, int nSize)
{
	if (pStream)
		return pStream->ReadArray(pBuffer, nSize);

	return 0;
}

//----------------------------------------------------------------------------------------------------------------------------
//导出字节流接口 - WriteStream
XSendByteStream* CppSendStreamCreate(int nSize)
{
	XSendByteStream* pStream = new XSendByteStream(nSize);
	return pStream;
}

void CppSendStreamClose(XSendByteStream* pStream)
{
	if (pStream)
	{
		delete pStream;
		pStream = nullptr;
	}
}

bool CppWriteInt8(XSendByteStream* pStream, int8_t n)
{
	if (pStream)
		return pStream->WriteInt8(n);

	return false;
}

bool CppWriteInt16(XSendByteStream* pStream, int16_t n)
{
	if (pStream)
		return pStream->WriteInt16(n);

	return false;
}

bool CppWriteInt32(XSendByteStream* pStream, int32_t n)
{
	if (pStream)
		return pStream->WriteInt32(n);

	return false;
}

bool CppWriteInt64(XSendByteStream* pStream, int64_t n)
{
	if (pStream)
		return pStream->WriteInt64(n);

	return false;
}

bool CppWriteFloat(XSendByteStream* pStream, float n)
{
	if (pStream)
		return pStream->WriteFloat(n);

	return false;
}

bool CppWriteDouble(XSendByteStream* pStream, double n)
{
	if (pStream)
		return pStream->WriteDouble(n);

	return false;
}

int CppWriteString(XSendByteStream* pStream, const char* pBuffer, int nSize)
{
	if (pStream)
		return pStream->WriteArray(pBuffer, nSize);

	return 0;
}

void CppFinish(XSendByteStream* pStream)
{
	if (pStream)
		return pStream->Finish(MSG_BYTESTREAM);
}