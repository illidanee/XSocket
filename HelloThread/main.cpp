#include <iostream>
#include <thread>
#include <mutex>

std::mutex m;

void OtherThread(int id)
{
	for (int i = 0; i < 1000; ++i)
	{
		m.lock();
		std::cout << id << "Hello other thread!" << i << std::endl;
		m.unlock();
	}
}

/****************************************************************************************************************
 *
 *    Brief   : 多线程的两种启动方式的比较。
		* detach() 启动线程并与主线程同时运行。
		* join() 启动线程并等待线程执行完成后执行主线程。

		1- 使用锁的时候需要使用join()方式启动，保证程序结束的时候没有使用锁，否则回报错。如果不使用join()方式，需要保证程序结束的时候没有使用锁。
		2- 使用锁的时候尽量使临界区小，否则相当于不适用多线程。锁使异步操作变成同步操作。
		3- 多线程执行的时候线程间的执行顺序不确定。
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
		//t[i].detach();
		t[i].join();
	}

	for (int i = 0; i < 5; ++i)
		std::cout << "Hello main thread!" << std::endl;
	
	return 0;
}