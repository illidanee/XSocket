#include "HelloWorldScene.h"
#include "AppMacros.h"

USING_NS_CC;


Scene* HelloWorld::scene()
{
     return HelloWorld::create();
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        CC_CALLBACK_1(HelloWorld::menuCloseCallback,this));
    
    closeItem->setPosition(origin + Vec2(visibleSize) - Vec2(closeItem->getContentSize() / 2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Hello World", "fonts/arial.ttf", TITLE_FONT_SIZE);
    
    // position the label on the center of the screen
    label->setPosition(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height);

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize / 2) + origin);

    // add the sprite as a child to this layer
    this->addChild(sprite);
    
	//------------------------------------

	XLog::SetFile("./Client.log", "w");

	this->Open();
	this->Connect("192.168.0.99", 9090);

	scheduleUpdate();

    return true;
}

void HelloWorld::menuCloseCallback(Ref* sender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::update(float dt)
{
	if (this->IsRun())
		this->OnRun();

	//²âÊÔ·¢ËÍÊý¾Ý
	XSendByteStream s(1024);
	s.WriteInt8(1);
	s.WriteInt16(2);
	s.WriteInt32(3);
	s.WriteInt64(4);
	s.WriteFloat(5.6f);
	s.WriteDouble(7.8);
	char b[] = { 'a', 'b', 'c', 'd', 'e' };
	s.WriteArray(b, sizeof(b));
	s.WriteArray("client", (int)strlen("client"));
	s.Finish(MSG_BYTESTREAM);

	this->SendStream(&s);
}

void HelloWorld::DoMsg(MsgHeader* pMsgHeader)
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

void HelloWorld::OnRunLoopBegin()
{

}

void HelloWorld::OnClientJoin(std::shared_ptr<XClient> pClient)
{

}
void HelloWorld::OnClientLeave(std::shared_ptr<XClient> pClient)
{

}
void HelloWorld::OnNetRecv(std::shared_ptr<XClient> pClient)
{

}

void HelloWorld::OnNetSend(std::shared_ptr<XClient> pClient)
{

}

void HelloWorld::OnNetMsgRecv(std::shared_ptr<XClient> pClient, MsgHeader* pMsgHeader)
{
	DoMsg(pMsgHeader);
}

void HelloWorld::OnNetMsgDone(std::shared_ptr<XClient>, MsgHeader* pMsgHeader)
{

}

void HelloWorld::AddTask(std::function<void()> pTask)
{

}
