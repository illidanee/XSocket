#include <iostream>
#include <thread>

void OtherThread()
{
	for (int i = 0; i < 5; ++i)
		std::cout << "Hello other thread!" << std::endl;
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
	std::thread t(OtherThread);
	t.detach();
	//t.join();

	for (int i = 0; i < 5; ++i)
		std::cout << "Hello main thread!" << std::endl;
	
	return 0;
}