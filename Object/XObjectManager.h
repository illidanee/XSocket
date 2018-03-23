#ifndef __XOBJECT_MANAGER_H__
#define __XOBJECT_MANAGER_H__

#include "XObjectPool.h"

template <size_t nCount, typename Type>
class XObjectManager
{
public:
	void* operator new(size_t nSize)
	{
		return XObjectManager::GetObjectPool().AllocObjectMemory(nSize);
	}

	void operator delete(void* pMem)
	{
		XObjectManager::GetObjectPool().FreeObjectMemory(pMem);
	}

	void* operator new[](size_t nSize)
	{
		return XObjectManager::GetObjectPool().AllocObjectMemory(nSize);
	}

	void operator delete[](void* pMem)
	{
		XObjectManager::GetObjectPool().FreeObjectMemory(pMem);
	}

	template <typename... Args>
	static Type* CreateObject(Args... args)
	{
		Type* pObj = new Type(args...);
		return pObj;
	}

	static void DeleteObject(Type* pObj)
	{
		delete pObj;
	}

private:
	static XObjectPool<nCount, Type>& GetObjectPool()
	{
		static XObjectPool<nCount, Type> objPool;
		return objPool;
	}
};

class AAA : public XObjectManager<100003, AAA>
{
public:
	AAA()
	{
		//printf("AAA \n");
	}
	~AAA()
	{
		//printf("~AAA \n");
	}
};

#endif
