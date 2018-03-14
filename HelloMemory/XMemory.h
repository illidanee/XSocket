#ifndef __MEMORY_H__
#define __MEMORY_H__

void* operator new(size_t nSize);
void operator delete(void* pMem);

void* operator new[](size_t nSize);
void operator delete[](void* pMem);

#endif