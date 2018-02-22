#include <iostream>
#include <thread>

void OtherThread()
{
	for (int i = 0; i < 5; ++i)
		std::cout << "Hello other thread!" << std::endl;
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
	std::thread t(OtherThread);
	t.detach();
	//t.join();

	for (int i = 0; i < 5; ++i)
		std::cout << "Hello main thread!" << std::endl;
	
	return 0;
}