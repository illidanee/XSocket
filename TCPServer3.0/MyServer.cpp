#include "MyServer.h"

MyServer::MyServer()
{
	//初始化其他
	_Timer.XInit();
	_ClientNum = 0;
	_RecvNum = 0;
	_SendNum = 0;
	_RecvPackageNum = 0;
	_DonePackageNum = 0;
	_PackageNum = 0;
}

MyServer::~MyServer()
{
	_Timer.XDone();
}

void MyServer::OnRunLoopBegin()
{
	if (_Timer.GetTime() > 1.0)
	{
		XLog("| Client Num = %7d  | Recv Num = %7d  | Send Num = %7d  | RecvPackage Num = %7d  | DonePackage Num = %7d  | Package Num = %7d  |\n", (int)_ClientNum, (int)_RecvNum, (int)_SendNum, (int)_RecvPackageNum, (int)_DonePackageNum, (int)_PackageNum);
		_RecvNum = 0;
		_SendNum = 0;
		_RecvPackageNum = 0;
		_DonePackageNum = 0;
		_Timer.UpdateTime();
	}
}

void MyServer::OnClientJoin(std::shared_ptr<XClient> pClient)
{
	++_ClientNum;
}

void MyServer::OnClientLeave(std::shared_ptr<XClient> pClient)
{
	--_ClientNum;
}

void MyServer::OnNetRecv(XClient* pClient)
{
	++_RecvNum;
}

void MyServer::OnNetSend(XClient* pClient)
{
	++_SendNum;
}

void MyServer::OnNetMsgRecv(XClient* pClient, MsgHeader* pMsgHeader, XReceiveServer* pReceiveServer)
{
	++_RecvPackageNum;
	++_PackageNum;

	//处理客户端请求
	switch (pMsgHeader->_MsgType)
	{
	case MSG_LOGIN:
	{
		std::shared_ptr<MsgLoginRes> respond = std::make_shared<MsgLoginRes>();
		std::function<void()> pTask = [pClient, respond]()
		{
			if (0 == pClient->SendData(respond.get()))
			{
				XLog("<Client=%d Send Buffer Full!!!\n", (int)pClient->GetSocket());
			}
		};
		
		pReceiveServer->AddTask(pTask);
	}
	break;
	case MSG_HEART:
	{
		pClient->ResetHeartTime();

		//使用任务系统
		std::shared_ptr<MsgHeart> respond = std::make_shared<MsgHeart>();
		std::function<void()> pTask = [pClient, respond]()
		{
			if (0 == pClient->SendData(respond.get()))
			{
				XLog("<Client=%d Send Buffer Full!!!\n", (int)pClient->GetSocket());
			}
		};

		pReceiveServer->AddTask(pTask);

		////直接发送数据
		//MsgHeart respond;
		//if (0 == pClient->SendData(&respond))
		//{
		//	XLog("<Client=%d Send Buffer Full!!!\n", (int)pClient->GetSocket());
		//}
	}
	break;
	default:
	{
		XLog("Warn： default Msg。");
	}
	}
}

void MyServer::OnNetMsgDone(XClient* pClient, MsgHeader* pMsgHeader, XReceiveServer* pReceiveServer)
{
	++_DonePackageNum;
	--_PackageNum;
}
