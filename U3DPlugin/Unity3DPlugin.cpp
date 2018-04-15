#include "Unity3DPlugin.h"

void MyClient::DoMsg(MsgHeader* pMsgHeader)
{

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

//����

void SetLogPath(const char* pLogPath)
{
	XLog::SetFile(pLogPath, "w");
}

MyClient* Open()
{
	MyClient* pClient = new MyClient();
	pClient->Open();
	return pClient;
}

bool Connect(MyClient* pClient, const char* ip, unsigned short port)
{
	if (pClient)
		return pClient->Connect(ip, port);

	return false;
}

void Close(MyClient* pClient)
{
	if (pClient)
		pClient->Close();
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

int SendData(MyClient* pClient, MsgHeader* pHeader)
{
	if (pClient)
		return pClient->SendData(pHeader);

	return 0;
}