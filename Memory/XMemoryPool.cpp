#include "XMemoryPool.h"
#include <assert.h>
#include <stdlib.h>

XMemoryPool::XMemoryPool()
{
	_nCount = 0;
	_nSize = 0;
	_pBuffer = nullptr;
	_pCur = nullptr;
}

XMemoryPool::~XMemoryPool()
{

}

int XMemoryPool::Init()
{
	assert(nullptr == _pBuffer);

	std::lock_guard<std::mutex> lock(_mutex);

	size_t nBlockSize = sizeof(XMemoryBlock) + _nSize;

	_pBuffer = malloc(_nCount * nBlockSize);

	//��ʼ���ڴ�顣
	_pCur = (XMemoryBlock*)_pBuffer;
	_pCur->_nID = 0;
	_pCur->_nSize = _nSize;
	_pCur->_pMemoryPool = this;
	_pCur->_pNext = nullptr;
	_pCur->_nRef = 0;

	XMemoryBlock* pLast = _pCur;
	for (int i = 1; i < _nCount; ++i)
	{
		XMemoryBlock* pCur = (XMemoryBlock*)((char*)_pBuffer + i * nBlockSize);
		pCur->_nID = i;
		pCur->_nSize = _nSize;
		pCur->_pMemoryPool = this;
		pCur->_pNext = nullptr;
		pCur->_nRef = 0;

		pLast->_pNext = pCur;
		pLast = pCur;
	}

	return 0;
}

int XMemoryPool::Done()
{
	assert(nullptr != _pBuffer);

	std::lock_guard<std::mutex> lock(_mutex);

	_nCount = 0;
	_nSize = 0;
	free(_pBuffer);
	_pBuffer = nullptr;

	return 0;
}

void* XMemoryPool::AllocMemory(size_t nSize)
{
	assert(nullptr != _pBuffer);

	XMemoryBlock* pMemoryBlock = nullptr;
	
	std::lock_guard<std::mutex> lock(_mutex);
	if (nullptr == _pCur)
	{	
		//��ǰ�ڴ��û�п����ڴ�飬���ڴ������롣
		pMemoryBlock = (XMemoryBlock*)malloc(sizeof(XMemoryBlock) + nSize);
		pMemoryBlock->_nID = -1;
		pMemoryBlock->_nSize = nSize;
		pMemoryBlock->_pMemoryPool = nullptr;
		pMemoryBlock->_pNext = nullptr;
		pMemoryBlock->_nRef = 1;

		XPrint("--------------------------------------------- AllocMemory : Addr = %p, ID = %d, Size = %d \n", pMemoryBlock, (int)pMemoryBlock->_nID, (int)pMemoryBlock->_nSize);
	}
	else
	{
		//ʹ���ڴ���е��ڴ�顣
		pMemoryBlock = _pCur;
		pMemoryBlock->_nRef += 1;
		_pCur = _pCur->_pNext;
	}

	return (char*)pMemoryBlock + sizeof(XMemoryBlock);
}

void XMemoryPool::FreeMemory(void* pMem)
{
	assert(nullptr != _pBuffer);
	assert(nullptr != pMem);

	XMemoryBlock* pMemoryBlock = (XMemoryBlock*)((char*)pMem - sizeof(XMemoryBlock));

	std::lock_guard<std::mutex> lock(_mutex);
	if (-1 == pMemoryBlock->_nID)
	{
		XPrint("--------------------------------------------- FreeMemory : Addr = %p, ID = %d, Size = %d \n", pMemoryBlock, (int)pMemoryBlock->_nID, (int)pMemoryBlock->_nSize);

		//ʹ��ϵͳ������ڴ�顣
		free(pMemoryBlock);
	}
	else
	{
		//ʹ���ڴ���е��ڴ�顣 
		pMemoryBlock->_pNext = _pCur;
		pMemoryBlock->_nRef -= 1;
		_pCur = pMemoryBlock;
	}

}