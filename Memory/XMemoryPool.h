#ifndef __XMEMORYPOOL_H__
#define __XMEMORYPOOL_H__

#include "XMemoryBlock.h"

#include <mutex>

class XMemoryPool
{
protected:
	size_t _nCount;					//�ڴ�������
	size_t _nSize;					//�ڴ���С��
	void* _pBuffer;					//�ڴ���׵�ַ��-- ʹ��malloc��free��ʱ�����ʹ��void*�����ʹ��new char��ʽ�����ʹ��char*���ͣ�������linux��delete void*���о���ġ�
	XMemoryBlock* _pCur;			//��ǰ�����ڴ���ַ��

	std::mutex _mutex;				//����

public:
	XMemoryPool();
	~XMemoryPool();

	int Init();
	int Done();
	void* AllocMemory(size_t nSize);
	void FreeMemory(void* pMem);
};

template <size_t nCount, size_t nSize>
class XTMemoryPool : public XMemoryPool
{
public:
	XTMemoryPool()
	{
		size_t n = sizeof(void*);
		_nCount = nCount;
		_nSize = nSize / n * n + (nSize % n ? n : 0);	//ʹ���ڴ�����С��

		XLog("XTMemoryPool() �� count = %d, size = %d \n", (int)_nCount, (int)_nSize);

		XMemoryPool::Init();
	}

	~XTMemoryPool()
	{
		XLog("~XTMemoryPool() �� count = %d, size = %d \n", (int)_nCount, (int)_nSize);

		XMemoryPool::Done();
	}
};

#endif