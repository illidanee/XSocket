#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#include "XCommon.h"
#include "XTCPClient.h"
#include "XRecvByteStream.h"
#include "XSendByteStream.h"

class HelloWorld : public cocos2d::Scene, public XTCPClient
{
public:
    virtual bool init() override;

    static cocos2d::Scene* scene();

    // a selector callback
    void menuCloseCallback(Ref* sender);

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

	void update(float dt);

public:
	void DoMsg(MsgHeader* pMsgHeader);

private:
	virtual void OnRunLoopBegin();
	virtual void OnClientJoin(std::shared_ptr<XClient> pClient);
	virtual void OnClientLeave(std::shared_ptr<XClient> pClient);
	virtual void OnNetRecv(std::shared_ptr<XClient> pClient);
	virtual void OnNetSend(std::shared_ptr<XClient> pClient);
	virtual void OnNetMsgRecv(std::shared_ptr<XClient> pClient, MsgHeader* pMsgHeader);
	virtual void OnNetMsgDone(std::shared_ptr<XClient> pClient, MsgHeader* pMsgHeader);

	virtual void AddTask(std::function<void()> pTask);
};

#endif // __HELLOWORLD_SCENE_H__
