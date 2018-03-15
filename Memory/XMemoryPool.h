#ifndef __XMEMORYPOOL_H__
#define __XMEMORYPOOL_H__

#include "XMemoryBlock.h"

#include <mutex>

class XMemoryPool
{
protected:
	size_t _nCount;					//内存块个数。
	size_t _nSize;					//内存块大小。
	void* _pBuffer;					//内存块首地址。
	XMemoryBlock* _pCur;			//当前空闲内存块地址。

	std::mutex _mutex;				//锁。

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
		_nSize = nSize / n * n + (nSize % n ? n : 0);	//使用内存对齐大小。

		XError("XTMemoryPool() ： count = %d, size = %d \n", (int)_nCount, (int)_nSize);

		XMemoryPool::Init();
	}

	~XTMemoryPool()
	{
		XError("~XTMemoryPool() ： count = %d, size = %d \n", (int)_nCount, (int)_nSize);

		XMemoryPool::Done();
	}
};

#endif