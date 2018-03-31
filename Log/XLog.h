#ifndef __XLOG_H__
#define __XLOG_H__

#include <assert.h>
#include <stdio.h>
#include <mutex>
#include <condition_variable>
#include <list>
#include <functional>

class XLog
{
public:
	static XLog& GetInstance();
	
	static void SetFile(const char* pFile, const char* pMode);

	template <typename... Args>
	static void Info(Args... args)
	{
		//������ն�
		printf(args...);

		//������ļ�
		XLog& log = GetInstance();

		//log ����ʹ�����á����������ס��
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
	FILE* _File;												//��־�ļ�

	bool _Run;													//��ǰ�߳��Ƿ���������
	std::mutex _ThreadlMutex;									//�߳���

	std::condition_variable _CV;								//��������
	std::mutex _SignalMutex;									//��
	int _WaitNum;												//�ȴ�����
	int _WakeNum;												//���Ѽ���

	std::list<std::function<void()>> _Tasks;					//�����б�
	std::list<std::function<void()>> _TasksCache;				//���񻺳���
	std::mutex _TasksCacheMutex;								//���񻺳�����
};

#endif