#include <iostream>
#include <thread>
#include <mutex>

#include <chrono>

const int tCount = 5;

std::mutex m;
int sum = 0;

void OtherThread(int id)
{
	for (int i = 0; i < 2000000; ++i)
	{
		m.lock();
		sum++;
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
		4- 多线程并行计算的时候如果不使用临界区锁，结果不确定。
		5- 测试枷锁的性能消耗，在大规模计算时频繁的加锁与解锁将消耗大量的性能。
 *
 ****************************************************************************************************************/
int main()
{
	std::thread t[tCount];
	for (int i = 0; i < tCount; ++i)
	{
		t[i] = std::thread(OtherThread, i);
	}

	//使用多线程测试-加锁
	sum = 0;
	std::chrono::time_point<std::chrono::high_resolution_clock> begin1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < tCount; ++i)
	{
		//t[i].detach();
		t[i].join();
	}
	std::chrono::time_point<std::chrono::high_resolution_clock> end1 = std::chrono::high_resolution_clock::now();
	std::chrono::microseconds time1 = std::chrono::duration_cast<std::chrono::microseconds>(end1 - begin1);
	std::cout << "time = " << time1.count() * 0.000001 << "  :  sum = " << sum << std::endl;

	//使用单线程测试-不加锁
	sum = 0;
	std::chrono::time_point<std::chrono::high_resolution_clock> begin2 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 10000000; ++i)
	{
		sum++;
	}
	std::chrono::time_point<std::chrono::high_resolution_clock> end2 = std::chrono::high_resolution_clock::now();
	std::chrono::microseconds time2 = std::chrono::duration_cast<std::chrono::microseconds>(end2 - begin2);
	std::cout << "time = " << time2.count() * 0.000001 << "  :  sum = " << sum << std::endl;

	//主线程任务
	std::cout << "Hello main thread!" << std::endl;
	
	return 0;
}