#ifndef __XMEMORYMANAGER_H__
#define __XMEMORYMANAGER_H__

#include "XMemoryPool.h"

#define _MAX_MEMORY_POOL_SIZE_ 1024

class XMemoryManager
{
private:
	XTMemoryPool<100000, 64> _MemoryPool64;
	XTMemoryPool<100000, 128> _MemoryPool128;
	XTMemoryPool<100000, 256> _MemoryPool256;
	XTMemoryPool<100000, 512> _MemoryPool512;
	XTMemoryPool<100000, 1024> _MemoryPool1024;
	XMemoryPool* _AllMemoryPools[_MAX_MEMORY_POOL_SIZE_ + 1];

	void InitMemoryPools(size_t nBegin, size_t nEnd, XMemoryPool* pPool);

private:
	XMemoryManager();
	XMemoryManager(const XMemoryManager& that);
	XMemoryManager& operator= (const XMemoryManager& that);
	~XMemoryManager();

public:
	static XMemoryManager& GetInstance();
	void* AllocMemory(size_t nSize);
	void FreeMemory(void* pMem);
};

#endif