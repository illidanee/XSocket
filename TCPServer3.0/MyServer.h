#ifndef __MYSERVER_H__
#define __MYSERVER_H__

#include "../XSrc/XCommon.h"
#include "../XSrc/XTCPSelectServer.h"
#include "../XSrc/XRecvByteStream.h"
#include "../XSrc/XSendByteStream.h"
#include "../XSrc/XMariaDB.h"
#include <atomic>
#include <functional>

//自定义Server
class MyServer : public XTCPSelectServer
{
public:
	virtual void OnMsg(std::shared_ptr<XClient> pClient, MsgHeader* pMsgHeader);
};

#endif
