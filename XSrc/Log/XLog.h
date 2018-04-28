#ifndef __XLOG_H__
#define __XLOG_H__

#include <assert.h>
#include <stdio.h>
#include <mutex>
#include <condition_variable>
#include <list>
#include <functional>
#include <thread>

class XLog
{
public:
	static XLog& GetInstance();
	
	static void SetFile(const char* pFile, const char* pMode);

	template <typename... Args>
	static void Info(Args... args)
	{
		//输出到终端
		printf(args...);

		//输出到文件
		XLog& log = GetInstance();

		//log 必须使用引用。否则会阻塞住。
		log.AddTask([&log, args...]() {
			fprintf(log._File, args...);
			fflush(log._File);
		});
	}

private:
	XLog();
	~XLog();

	void Start();
	void Stop();
	void Run();

	void AddTask(std::function<void()> pTask);

private:
	FILE* _File;												//日志文件

	bool _Run;													//当前线程是否正在运行
	std::mutex _ThreadlMutex;									//线程锁

	std::condition_variable _CV;								//条件变量
	std::mutex _SignalMutex;									//锁
	int _WaitNum;												//等待计数
	int _WakeNum;												//唤醒计数

	std::list<std::function<void()>> _Tasks;					//任务列表
	std::list<std::function<void()>> _TasksCache;				//任务缓冲区
	std::mutex _TasksCacheMutex;								//任务缓冲区锁
};

//调试宏
#ifndef XInfo
#ifdef _DEBUG
#define XInfo(...) XLog::Info(__VA_ARGS__)
#else
#define XInfo(...) //XLog::Info(__VA_ARGS__)
#endif
#endif

#endif