#ifndef __XOBJECT_MANAGER_H__
#define __XOBJECT_MANAGER_H__

#include "XObjectPool.h"

template <typename T, size_t nCount>
class XObjectManager
{
public:
	void* operator new(size_t nSize)
	{
		return XObjectManager::GetObjectPool().AllocMemory(nSize);
	}

	void operator delete(void* pMem)
	{
		XObjectManager::GetObjectPool().FreeMemory(pMem);
	}

	void* operator new[](size_t nSize)
	{
		return XObjectManager::GetObjectPool().AllocMemory(nSize);
	}

	void operator delete[](void* pMem)
	{
		ObjectManager::GetObjectPool().FreeMemory(pMem);
	}

	template <typename... Args>
	static T* CreateObject(Args... args)
	{
		T* pObj = new T(args...);
		return pObj;
	}

	static void DeleteObject(T* pObj)
	{
		delete pObj;
	}

private:
	static XObjectPool<T, nCount>& GetObjectPool()
	{
		static XObjectPool<T, nCount> objPool;
		return objPool;
	}
};

#endif
