#include <iostream>
#include <thread>

void OtherThread(int id)
{
	for (int i = 0; i < 5; ++i)
		std::cout << id << "Hello other thread!" << std::endl;
}

/****************************************************************************************************************
 *
 *    Brief   : 多线程的两种启动方式的比较。
		* detach() 启动线程并与主线程同时运行。
		* join() 启动线程并等待线程执行完成后执行主线程。
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