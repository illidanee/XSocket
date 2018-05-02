#include "MyClient.h"

void MyClient::DoMsg(MsgHeader* pMsgHeader)
{

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

void MyClient::OnNetMsgDone(std::shared_ptr<XClient>, MsgHeader* pMsgHeader)
{

}

void MyClient::AddTask(std::function<void()> pTask)
{

}