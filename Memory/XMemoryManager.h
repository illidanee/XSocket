#ifndef __XMEMORYMANAGER_H__
#define __XMEMORYMANAGER_H__

#include "XMemoryPool.h"

#define _MAX_MEMORY_POOL_SIZE_ 32768

class XMemoryManager
{
private:
	XTMemoryPool<1024000 * 20, 32> _MemoryPool32;
	XTMemoryPool<1024000 * 20, 64> _MemoryPool64;
	XTMemoryPool<8, 128> _MemoryPool128;
	XTMemoryPool<8, 256> _MemoryPool256;
	XTMemoryPool<8, 512> _MemoryPool512;
	XTMemoryPool<8, 1024> _MemoryPool1024;
	XTMemoryPool<10240, 32768> _MemoryPool32768;
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