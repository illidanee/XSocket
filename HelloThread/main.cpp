#include <iostream>
#include <thread>

void OtherThread(int id)
{
	for (int i = 0; i < 5; ++i)
		std::cout << id << "Hello other thread!" << std::endl;
}

/****************************************************************************************************************
 *
 *    Brief   : ���̵߳�����������ʽ�ıȽϡ�
		* detach() �����̲߳������߳�ͬʱ���С�
		* join() �����̲߳��ȴ��߳�ִ����ɺ�ִ�����̡߳�
 *
 ****************************************************************************************************************/
int main()
{
	std::thread t[3];
	for (int i = 0; i < 3; ++i)
	{
		t[i] = std::thread(OtherThread, i);
	}
	for (int i = 0; i < 3; ++i)
	{
		t[i].detach();
		//t[i].join();
	}

	for (int i = 0; i < 5; ++i)
		std::cout << "Hello main thread!" << std::endl;
	
	return 0;
}