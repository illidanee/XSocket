#ifndef __XOBJECTPOOL_H__
#define __XOBJECTPOOL_H__

#include <assert.h>
#include <mutex>

#ifndef XError
#include <stdio.h>
#define XError(...) printf(__VA_ARGS__)
#endif

template <size_t nCount, typename Type>
class XObjectPool
{
public:
	class XObjectBlock
	{
	public:
		size_t _nID;						//�����ID��- id Ϊ -1 ˵�����ڶ�����С�
		size_t _nSize;						//������С��
		XObjectPool* _pObjectPool;			//��������ء�- _pMemoryPool Ϊ nullptr ˵�����ڶ�����С�
		XObjectBlock* _pNext;				//��һ�����ö�����ַ��
		size_t _nRef;						//��ǰ����鱻���ô�����
	};

private:
	size_t _nCount;					//����������
	size_t _nSize;					//������С��
	void* _pBuffer;					//������׵�ַ��
	XObjectBlock* _pCur;			//��ǰ���ж�����ַ��

	std::mutex _mutex;				//����

public:
	XObjectPool()
	{
		_nCount = nCount;
		_nSize = sizeof(Type);
		_pBuffer = nullptr;
		_pCur = nullptr;
		Init();
	}

	~XObjectPool()
	{
		Done();
	}

	int Init()
	{
		assert(nullptr == _pBuffer);

		XError("XObjectPool Init : _nCount = %d, _nSize = %d \n", (int)_nCount, (int)_nSize);

		std::lock_guard<std::mutex> lock(_mutex);

		size_t nBlockSize = sizeof(XObjectBlock) + _nSize;

		_pBuffer = new char[_nCount * nBlockSize];

		//��ʼ���ڴ�顣
		_pCur = (XObjectBlock*)_pBuffer;
		_pCur->_nID = 0;
		_pCur->_nSize = _nSize;
		_pCur->_pObjectPool = this;
		_pCur->_pNext = nullptr;
		_pCur->_nRef = 0;

		XObjectBlock* pLast = _pCur;
		for (int i = 1; i < _nCount; ++i)
		{
			XObjectBlock* pCur = (XObjectBlock*)((char*)_pBuffer + i * nBlockSize);
			pCur->_nID = i;
			pCur->_nSize = _nSize;
			pCur->_pObjectPool = this;
			pCur->_pNext = nullptr;
			pCur->_nRef = 0;

			pLast->_pNext = pCur;
			pLast = pCur;
		}

		return 0;
	}

	int Done()
	{
		assert(nullptr != _pBuffer);

		std::lock_guard<std::mutex> lock(_mutex);

		_nCount = 0;
		_nSize = 0;
		delete[] _pBuffer;
		_pBuffer = nullptr;

		return 0;
	}

	void* AllocObjectMemory(size_t nSize)
	{
		assert(nullptr != _pBuffer);

		XObjectBlock* pObjectBlock = nullptr;

		std::lock_guard<std::mutex> lock(_mutex);
		if (nullptr == _pCur)
		{
			//��ǰ�ڴ��û�п����ڴ�飬���ڴ������롣
			pObjectBlock = (XObjectBlock*)new char[sizeof(XObjectBlock) + nSize];
			pObjectBlock->_nID = -1;
			pObjectBlock->_nSize = nSize;
			pObjectBlock->_pObjectPool = nullptr;
			pObjectBlock->_pNext = nullptr;
			pObjectBlock->_nRef = 1;

			XError("AllocObjectMemory : Addr = %p, ID = %d, Size = %d \n", pObjectBlock, (int)pObjectBlock->_nID, (int)pObjectBlock->_nSize);
		}
		else
		{
			//ʹ���ڴ���е��ڴ�顣
			pObjectBlock = _pCur;
			pObjectBlock->_nRef += 1;
			_pCur = _pCur->_pNext;
		}

		return (char*)pObjectBlock + sizeof(XObjectBlock);
	}

	void FreeObjectMemory(void* pMem)
	{
		assert(nullptr != _pBuffer);
		assert(nullptr != pMem);

		XObjectBlock* pObjectBlock = (XObjectBlock*)((char*)pMem - sizeof(XObjectBlock));

		std::lock_guard<std::mutex> lock(_mutex);
		if (-1 == pObjectBlock->_nID)
		{
			XError("FreeObjectMemory : Addr = %p, ID = %d, Size = %d \n", pObjectBlock, (int)pObjectBlock->_nID, (int)pObjectBlock->_nSize);
			//ʹ��ϵͳ������ڴ�顣
			delete[] pObjectBlock;
		}
		else
		{
			//ʹ���ڴ���е��ڴ�顣 
			pObjectBlock->_pNext = _pCur;
			pObjectBlock->_nRef -= 1;
			_pCur = pObjectBlock;
		}
	}
};

#endif // ifndef
