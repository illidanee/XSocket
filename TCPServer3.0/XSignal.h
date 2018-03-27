#ifndef __XSIGNAL_H__
#define __XSIGNAL_H__

#include <condition_variable>
#include <mutex>

class XSignal
{
public:
	XSignal();

	void Wait();
	void Wake();

private:
	std::condition_variable _CV;	//��������
	std::mutex _Mutex;				//��

	int _WaitNum;					//�ȴ�����
	int _WakeNum;					//���Ѽ���
};

#endif