#ifndef __XMEMORYBLOCK_H__
#define __XMEMORYBLOCK_H__

#ifdef _DEBUG
	#include <stdio.h>
	#define XPrint(...) printf(__VA_ARGS__)
#else
	#define XPrint(...)
#endif

class XMemoryPool;

class XMemoryBlock
{
public:
	size_t _nID;						//�ڴ��ID��- id Ϊ -1 ˵�������ڴ���С�
	size_t _nSize;						//�ڴ���С��
	XMemoryPool* _pMemoryPool;			//�����ڴ�ء�- _pMemoryPool Ϊ nullptr ˵�������ڴ���С�
	XMemoryBlock* _pNext;				//��һ�������ڴ���ַ��
	size_t _nRef;						//��ǰ�ڴ�鱻���ô�����
};

#endif