#include "MyClient.h"

void MyClient::DoMsg(MsgHeader* pMsgHeader)
{
	XRecvByteStream r(pMsgHeader);

	int32_t type = MSG_ERROR;
	r.ReadInt32(type);

	int8_t r1;
	r.ReadInt8(r1);
	int16_t r2;
	r.ReadInt16(r2);
	int32_t r3;
	r.ReadInt32(r3);
	int64_t r4;
	r.ReadInt64(r4);
	float r5;
	r.ReadFloat(r5);
	double r6;
	r.ReadDouble(r6);
	char r7[32] = {};
	r.ReadArray(r7, 32);
	char r8[32] = {};
	r.ReadArray(r8, 32);
}

void MyClient::OnRunLoopBegin()
{

}

void MyClient::OnClientJoin(std::shared_ptr<XClient> pClient)
{

}
void MyClient::OnClientLeave(std::shared_ptr<XClient> pClient)
{

}
void MyClient::OnNetRecv(std::shared_ptr<XClient> pClient)
{

}

void MyClient::OnNetSend(std::shared_ptr<XClient> pClient)
{

}

void MyClient::OnNetMsgRecv(std::shared_ptr<XClient> pClient, MsgHeader* pMsgHeader)
{
	DoMsg(pMsgHeader);
}

void MyClient::OnNetMsgDone(std::shared_ptr<XClient>, MsgHeader* pMsgHeader)
{

}

void MyClient::AddTask(std::function<void()> pTask)
{

}