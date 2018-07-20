#ifndef __XMEMORYMANAGER_H__
#define __XMEMORYMANAGER_H__

#include "XMemoryPool.h"

#define _MAX_MEMORY_POOL_SIZE_ 10240

class XMemoryManager
{
private:
	//配置的内存池
	XTMemoryPool<1024, 16>		_MemoryPool16;
	XTMemoryPool<10240, 32>		_MemoryPool32;
	XTMemoryPool<20480, 64>		_MemoryPool64;
	XTMemoryPool<10240, 128>	_MemoryPool128;
	XTMemoryPool<8, 256>		_MemoryPool256;
	XTMemoryPool<20480, 512>		_MemoryPool512;
	XTMemoryPool<8, 1024>		_MemoryPool1024;
	XTMemoryPool<20480, 10240>	_MemoryPool10240;

	XMemoryPool* _AllMemoryPools[_MAX_MEMORY_POOL_SIZE_ + 1];

	void InitMemoryPools(size_t nBegin, size_t nEnd, XMemoryPool* pPool);

private:
	XMemoryManager();
	XMemoryManager(const XMemoryManager&);
	XMemoryManager& operator= (const XMemoryManager&);
	~XMemoryManager();

public:
	static XMemoryManager& GetInstance();

	void* AllocMemory(size_t nSize);
	void FreeMemory(void* pMem);
};

#endif