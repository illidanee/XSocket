#ifndef __XMEMORYBLOCK_H__
#define __XMEMORYBLOCK_H__

#ifndef XPrint
#include <stdio.h>
#ifdef _DEBUG
#define XPrint(...) printf(__VA_ARGS__)
#else
#define XPrint(...) printf(__VA_ARGS__)
#endif
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