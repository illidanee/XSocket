#include "MyClient.h"

void MyClient::OnRunLoopBegin()
{

}

void MyClient::OnClientJoin(std::shared_ptr<XClient> pClient)
{

}
void MyClient::OnClientLeave(std::shared_ptr<XClient> pClient)
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

}

void MyClient::OnNetMsgDone(XClient* pClient, MsgHeader* pMsgHeader)
{

}

void MyClient::AddTask(std::function<void()> pTask)
{

}