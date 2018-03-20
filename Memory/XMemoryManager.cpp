#include "XMemoryManager.h"
#include <stdlib.h>

void XMemoryManager::InitMemoryPools(size_t nBegin, size_t nEnd, XMemoryPool* pPool)
{
	for (size_t i = nBegin; i <= nEnd; ++i)
	{
		_AllMemoryPools[i] = pPool;
	}
}

XMemoryManager::XMemoryManager()
{
	XError("XMemoryManager() \n");

	InitMemoryPools(0 + 1, 16, &_MemoryPool16);
	InitMemoryPools(16 + 1, 32, &_MemoryPool32);
	InitMemoryPools(32 + 1, 64, &_MemoryPool64);
	InitMemoryPools(64 + 1, 128, &_MemoryPool128);
	InitMemoryPools(128 + 1, 256, &_MemoryPool256);
	InitMemoryPools(256 + 1, 512, &_MemoryPool512);
	InitMemoryPools(512 + 1, 1024, &_MemoryPool1024);
	InitMemoryPools(1024 + 1, 16384, &_MemoryPool16384);
	InitMemoryPools(16384 + 1, 32768, &_MemoryPool32768);
	
}

XMemoryManager::XMemoryManager(const XMemoryManager& that)
{

}

XMemoryManager::~XMemoryManager()
{

}

XMemoryManager& XMemoryManager::operator=(const XMemoryManager& that)
{

	return *this;
}

XMemoryManager& XMemoryManager::GetInstance()
{
	static XMemoryManager mgr;
	return mgr;
}

void* XMemoryManager::AllocMemory(size_t nSize)
{
	if (nSize <= _MAX_MEMORY_POOL_SIZE_)
	{
		//ʹ���ڴ�ء�
		return _AllMemoryPools[nSize]->AllocMemory(nSize);
	}
	else
	{
		//û�ж�Ӧ���ڴ�ؿ���ʹ�á�
		XMemoryBlock* pMemoryBlock = (XMemoryBlock*)malloc(sizeof(XMemoryBlock) + nSize);
		pMemoryBlock->_nID = -1;
		pMemoryBlock->_nSize = nSize;
		pMemoryBlock->_pMemoryPool = nullptr;
		pMemoryBlock->_pNext = nullptr;
		pMemoryBlock->_nRef = 1;

		XError("AllocMemory : Addr = %p, ID = %d, Size = %d \n", pMemoryBlock, (int)pMemoryBlock->_nID, (int)pMemoryBlock->_nSize);

		return (char*)pMemoryBlock + sizeof(XMemoryBlock);
	}
}

void XMemoryManager::FreeMemory(void* pMem)
{
	XMemoryBlock* pMemoryBlock = (XMemoryBlock*)((char*)pMem - sizeof(XMemoryBlock));
	//XError("FreeMemory : Addr = %p, ID = %d, Size = %d \n", pMemoryBlock, pMemoryBlock->_nID, pMemoryBlock->_nSize);
	if (pMemoryBlock->_pMemoryPool)
	{
		//ʹ���ڴ�ء�
		pMemoryBlock->_pMemoryPool->FreeMemory(pMem);
	}
	else
	{
		//û���ڴ�ؿ���ʹ�á�
		free(pMemoryBlock);
	}
}