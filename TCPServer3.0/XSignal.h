#ifndef __XSIGNAL_H__
#define __XSIGNAL_H__

class XSignal
{
public:
	void Sleep();
	void Wait();
	void Wake();

private:
	bool _Wait;				//ÊÇ·ñ¼ÌÐøµÈ´ý
};

#endif