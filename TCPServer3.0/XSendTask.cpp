#include "XSendTask.h"

XSendTask::XSendTask(XClient* pClient, MsgHeader* pHeader)
{
	_pClient = pClient;
	_pHeader = pHeader;
}

XSendTask::~XSendTask()
{
	//delete _pHeader;
}

void XSendTask::DoTask()
{
	_pClient->SendData(_pHeader);
}