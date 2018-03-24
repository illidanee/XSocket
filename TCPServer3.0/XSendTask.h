#ifndef __XSENDTASK_H__
#define __XSENDTASK_H__

#include "MsgProtocol.h"
#include "XTask.h"
#include "XClient.h"

class XSendTask : public XTask
{
private:
	XClient* _pClient;
	MsgHeader* _pHeader;

public:
	XSendTask(XClient* pClient, MsgHeader* pHeader);
	~XSendTask();
	void DoTask();
};

#endif